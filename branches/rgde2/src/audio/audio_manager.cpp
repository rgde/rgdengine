#include "stdafx.h"

#include "audio_tag.h"
#include "sound3d.h"
#include "music.h"
#include "audio_manager.h"
#include "wavefile.h"
#include "wavefile_factory.h"

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define CLAMP(var, min, max)   (((var) < (min)) ? (var) = (min) : ((var) > (max)) ? (var) = (max) : (var))

namespace rgde
{
	namespace audio
	{


		audio_manager* audio_manager::sm_instance = NULL;  // singleton instance


		//-----------------------------------------------------------------------------
		// Name: audio_manager::Initialize()
		// Desc: Initializes the IDirectSound object and also sets the primary buffer
		//       format.  This function must be called before any others.
		//-----------------------------------------------------------------------------
		audio_manager::audio_manager(core::windows::handle handle, 
			DWORD dwPrimaryChannels, 
			DWORD dwPrimaryFreq, 
			DWORD dwPrimaryBitRate,
			hrtf hrtf)
			:	m_pDS(NULL),
			m_pDSBPrimary(NULL),
			m_pDSListener(NULL),
			m_pListenerCam(NULL),
			m_frequency(22050),
			m_bitRate(16),
			m_hrtf(hrtf_full),
			m_distanceFactor(DS3D_DEFAULTDISTANCEFACTOR),
			m_rolloffFactor(DS3D_DEFAULTROLLOFFFACTOR),
			m_dopplerFactor(DS3D_DEFAULTDOPPLERFACTOR),
			m_volume(DSBVOLUME_MAX-2000)
		{
			ZeroMemory(m_effectBufs, sizeof(Buffer)*max_effect_buffers);
			ZeroMemory(m_musicBufs, sizeof(Buffer)*max_music_buffers);

			new wave_file_factory();

			HRESULT hr;
			int i;

			m_frequency = dwPrimaryFreq;
			m_bitRate = dwPrimaryBitRate;
			m_hrtf = hrtf;

			SAFE_RELEASE(m_pDS);

			// Create IDirectSound using the primary sound device
			if (FAILED(hr = DirectSoundCreate8(NULL, &m_pDS, NULL)))
			{
				DXTRACE_ERR(TEXT("audio_manager::Init, DirectSoundCreate8"), hr);
				return;
			}

			// Set DirectSound coop level 
			if (FAILED(hr = m_pDS->SetCooperativeLevel((HWND)handle.vp, DSSCL_PRIORITY)))
			{
				DXTRACE_ERR(TEXT("audio_manager::Init, SetCooperativeLevel"), hr);
				return;
			}

			DSBUFFERDESC dsbdesc;

			// Obtain primary buffer, asking it for 3D control
			ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
			dsbdesc.dwSize = sizeof(DSBUFFERDESC);
			dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_PRIMARYBUFFER;

			if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbdesc, &m_pDSBPrimary, NULL)))
			{
				DXTRACE_ERR(TEXT("audio_manager::Init, CreateSoundBuffer"), hr);
				throw std::exception("audio_manager::Init, CreateSoundBuffer");
			}

			if (FAILED(hr = m_pDSBPrimary->QueryInterface(IID_IDirectSound3DListener, 
				(VOID**)&m_pDSListener)))
			{
				DXTRACE_ERR(TEXT("audio_manager::Init, QueryInterface"), hr );
				throw std::exception("audio_manager::Init, QueryInterface");
			}

			SetPrimaryBufferFormat(dwPrimaryChannels, dwPrimaryFreq, dwPrimaryBitRate);

			for (i = 0; i < max_effect_buffers; ++i)
			{
				CreateBuffer(&(m_effectBufs[i]), effect_buffer_size, true);
			}

			for (i = 0; i < max_music_buffers; ++i)
			{
				CreateBuffer(&(m_musicBufs[i]), music_buffer_size, false);
			}

			sm_instance = this;
		}


		audio_manager::~audio_manager()
		{
			int i;

			for (i = 0; i < max_effect_buffers; ++i)
			{
				delete m_effectBufs[i].audio;
				SAFE_RELEASE(m_effectBufs[i].pDSBuf);
				SAFE_RELEASE(m_effectBufs[i].pDSBuf3D);
			}

			for (i = 0; i < max_music_buffers; ++i)
			{
				delete m_musicBufs[i].audio;
				SAFE_RELEASE(m_musicBufs[i].pDSBuf);
			}

			clear_audio_tags();

			m_pListenerCam = NULL;

			delete wave_file_factory::instance();

			SAFE_RELEASE(m_pDSListener);
			SAFE_RELEASE(m_pDSBPrimary);
			SAFE_RELEASE(m_pDS); 
		}



		//-----------------------------------------------------------------------------
		// Name: audio_manager::SetPrimaryBufferFormat()
		// Desc: Set primary buffer to a specified format 
		//       For example, to set the primary buffer format to 22kHz stereo, 16-bit
		//       then:   dwPrimaryChannels = 2
		//               dwPrimaryFreq     = 22050, 
		//               dwPrimaryBitRate  = 16
		//-----------------------------------------------------------------------------
		HRESULT audio_manager::SetPrimaryBufferFormat(DWORD dwPrimaryChannels, 
			DWORD dwPrimaryFreq, 
			DWORD dwPrimaryBitRate )
		{
			HRESULT hr;

			if (NULL == m_pDS || NULL == m_pDSBPrimary)
				return CO_E_NOTINITIALIZED;

			WAVEFORMATEX wfx;
			ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
			wfx.wFormatTag      = WAVE_FORMAT_PCM; 
			wfx.nChannels       = (WORD) dwPrimaryChannels; 
			wfx.nSamplesPerSec  = dwPrimaryFreq; 
			wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
			wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
			wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

			if( FAILED(hr = m_pDSBPrimary->SetFormat(&wfx) ) )
				return DXTRACE_ERR( TEXT("SetFormat"), hr );

			return S_OK;
		}

		bool audio_manager::load_audio_tags(xml::node audio_db_node)
		{
			audio_tag* tag = NULL;

			for(xml::node child_el = audio_db_node.first_child(); child_el.next_sibling(); 
				child_el = child_el.next_sibling())
			{
				tag = NULL;

				std::string tag_name = child_el.name();

				if ( _stricmp("Effect", tag_name.c_str()) == 0)
				{
					tag = new AudioEffectTag();
				}
				else if ( _stricmp("music", tag_name.c_str()) == 0)
				{
					tag = new AudioMusicTag();
				}
				else if ( _stricmp("Ambient", tag_name.c_str()) == 0)
				{
					tag = new AudioAmbientTag();
				}
				else if ( _stricmp("Composition", tag_name.c_str()) == 0)
				{
					tag = new AudioCompositionTag();
				}
				else if ( _stricmp("Group", tag_name.c_str()) == 0)
				{
					tag = new AudioGroupTag();
				}
				else if ( _stricmp("Random", tag_name.c_str()) == 0)
				{
					tag = new AudioRandomTag();
				}
				else
				{
					ASSERT(!"Bad internal::base_audio Tag");
				}

				if (tag)
				{
					if (!tag->load_tag(child_el))
					{
						delete tag;
						return false;
					}

					std::string  tag_name = child_el["ID"].value();
					if (tag_name.empty())
					{
						delete tag;
						return false;
					}

					m_tags.insert(AudioTagMap::value_type(tag_name.c_str(), tag));
				}
			}

			return true;
		}


		bool audio_manager::load_audio_tags(const char* file_name)
		{
			xml::document doc;
			bool success = doc.load_file(file_name);

			if (!success)
			{
				MessageBoxA(NULL, "Unable to process the XML document.", "Error", MB_OK);
				return false;
			}

			xml::node audio_db_node = doc("AudioTagDatabase");

			if (!audio_db_node)
			{
				MessageBoxA(NULL, "Unable to process the XML document.", "Error", MB_OK);
				return false;
			}

			return load_audio_tags(audio_db_node);
		}


		void audio_manager::clear_audio_tags()
		{
			AudioTagIterator it = m_tags.begin();
			AudioTagIterator end = m_tags.end();

			for (; it != end; ++it)
			{
				delete it->second;
			}

			m_tags.clear();
		}

		audio_tag* audio_manager::get_audio_tag(const char* tag_name)
		{
			AudioTagIterator foundTag = m_tags.find(tag_name);

			if (foundTag != m_tags.end())
			{
				return foundTag->second;
			}
			else
			{
				return NULL;
			}
		}


		bool audio_manager::CreateBuffer(Buffer* buff, DWORD size, bool b3D)
		{
			ASSERT(size >= DSBSIZE_MIN && size <= DSBSIZE_MAX);
			ASSERT(b3D ? (size >= (DSBSIZE_FX_MIN * m_frequency * (m_bitRate / 8)) / 1000) : true);

			HRESULT hr;
			DSBUFFERDESC dsbd;
			WAVEFORMATEX wfx;
			ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
			dsbd.dwSize          = sizeof(DSBUFFERDESC);
			dsbd.dwBufferBytes   = size;
			dsbd.dwFlags         = DSBCAPS_GLOBALFOCUS | 
				DSBCAPS_CTRLVOLUME |
				DSBCAPS_GETCURRENTPOSITION2;
			wfx.wFormatTag = WAVE_FORMAT_PCM;

			if (b3D)
			{
				dsbd.dwFlags    |= DSBCAPS_CTRL3D | 
					DSBCAPS_CTRLFX |
					DSBCAPS_CTRLFREQUENCY;
				switch (m_hrtf)
				{
				case hrtf_full: dsbd.guid3DAlgorithm = DS3DALG_HRTF_FULL; break;
				case hrtf_light: dsbd.guid3DAlgorithm = DS3DALG_HRTF_LIGHT; break;
				case hrtf_none: dsbd.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION ; break;
				default: ASSERT(false); break;
				}
				wfx.nChannels = 1; 
			}
			else
			{
				dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
				wfx.nChannels       = 2; 
			}

			wfx.nSamplesPerSec  = m_frequency; 
			wfx.wBitsPerSample  = (WORD)m_bitRate; 
			wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
			wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
			wfx.cbSize = 0;
			dsbd.lpwfxFormat     = &wfx; 

			buff->volume = m_volume;

			if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &(buff->pDSBuf), NULL)))
			{
				DXTRACE_ERR(TEXT("CreateSoundBuffer"), hr);
				return false;
			}

			buff->pDSBuf3D = 0;
			if (b3D)
			{
				if (FAILED(hr = buff->pDSBuf->QueryInterface(IID_IDirectSound3DBuffer, (VOID**)&(buff->pDSBuf3D))))
				{
					DXTRACE_ERR(TEXT("QueryInterface for IDirectSound3DBuffer"), hr);
					return false;
				}
			}

			return true;
		}

		bool audio_manager::RestoreBuffer(Buffer* buff)
		{
			if( !buff || !buff->pDSBuf)
				return true;

			HRESULT hr;
			DWORD dwStatus;

			if(FAILED(hr = buff->pDSBuf->GetStatus(&dwStatus)))
			{
				DXTRACE_ERR(TEXT("GetStatus"), hr);
				return false;
			}


			if (dwStatus & DSBSTATUS_BUFFERLOST)
			{
				// Since the app could have just been activated, then
				// DirectSound may not be giving us control yet, so 
				// the restoring the buffer may fail.  
				// If it does, sleep until DirectSound gives us control.
				do 
				{
					hr = buff->pDSBuf->Restore();
					if (hr == DSERR_BUFFERLOST)
						Sleep( 10 );
				}
				while (hr = buff->pDSBuf->Restore());

				return true;
			}
			else
			{
				return false;
			}
		}


		void audio_manager::play(const char* tag_name,
			world_object* obj,
			int ms_duration,
			int ms_delay,
			audio::listener* notify)
		{
			audio_tag* tag = get_audio_tag(tag_name);
			//ASSERT(tag);

			if (tag)
			{
				if (ms_delay > 0)
				{
					AudioWaitingToBePlayed wait;
					wait.tag = tag;
					wait.ms_duration = ms_duration;
					wait.obj = obj;
					wait.ms_delay = ms_delay;
					wait.notify = notify;
					m_toBePlayed.push_back(wait);
				}
				else
				{
					play(tag, obj, ms_duration, ms_delay, notify);
				}
			}
		}

		void audio_manager::play(audio_tag* tag,
			world_object* obj,
			int ms_duration,
			int ms_delay,
			audio::listener* notify)
		{
			ASSERT(tag);

			if (ms_delay > 0)
			{
				AudioWaitingToBePlayed wait;
				wait.tag = tag;
				wait.ms_duration = ms_duration;
				wait.obj = obj;
				wait.ms_delay = ms_delay;
				wait.notify = notify;
				m_toBePlayed.push_back(wait);
			}
			else
			{
				internal::base_audio* audio = tag->create_audio(obj, ms_duration, ms_delay, notify);

				if (audio)			// some tags don't create any actual audio
				{
					switch (audio->get_type())
					{
					case internal::base_audio::sound:
						{
							sound* pSound = (sound*)(audio);
							return;
						}

					case internal::base_audio::sound3d:
						{
							sound3d* pSound3D = (sound3d*)(audio);
							if (obj)
							{
								pSound3D->set_world_object(obj);
							}

							PlayEffect(tag, pSound3D, ms_duration);
							return;
						}

					case internal::base_audio::music:
						{
							music* pMusic = (music*)(audio);
							PlayMusic(tag, pMusic, ms_duration);
							return;
						}

					default:
						ASSERT(!"Bad internal::base_audio type");
						break;
					}
				}
			}
		}

		void audio_manager::PlayEffect(audio_tag* tag, sound3d* pSound3D,
			int ms_duration)
		{
			int availableIndex = -1;
			int lowPriority = tag->get_priority();
			int lowestPriorityIndex = -1;
			int i, numTimesPlaying = 0;
			wave_file* pWaveFile = pSound3D->get_wave_file();

			// pick the appropriate buffer to use
			for (i = 0; i < max_effect_buffers && -1 == availableIndex; ++i)
			{
				if (m_effectBufs[i].tag == tag)
				{
					++numTimesPlaying;
				}

				// try and find one that's not currently playing
				if (!m_effectBufs[i].bPlaying)
				{
					availableIndex = i;
				}
				else
				{
					// otherwise, if we can't find an empty slot, find one based on the 
					// priority of the audio tag

					// todo add another test, in case of multiple lower priority choices
					// like the buffer that's closest to completion, or check for similar sounds first
					if (m_effectBufs[i].tag->get_priority() <= lowPriority)
					{
						lowPriority = m_effectBufs[i].tag->get_priority();
						lowestPriorityIndex = i;
					}
				}
			}

			if (-1 == availableIndex)
			{
				if (-1 == lowestPriorityIndex)
				{
					// sound is not high enough priority to play, so quit
					return;
				}
				else
				{
					StopBuffer(&m_effectBufs[lowestPriorityIndex]);
					availableIndex = lowestPriorityIndex;
				}
			}

			int cascaded_num = ((AudioEffectTag*)tag)->GetCascadeNumber();
			if (numTimesPlaying >= cascaded_num)
			{
				for (i = 0; i < max_effect_buffers; ++i)
				{
					if (m_effectBufs[i].tag == tag)
					{
						StopBuffer(&m_effectBufs[i]);
					}
				}

				play(((AudioEffectTag*)tag)->GetCascadeTag(), pSound3D->get_world_object());
				delete pSound3D; // not going to play this sound, clean it up
			}
			else 
			{
				// get rid of old internal::base_audio object from last use
				delete m_effectBufs[availableIndex].audio;

				// Load wavefile into sound buffer
				// Set up 3d data, position and velocity
				m_effectBufs[availableIndex].pDSBuf->SetCurrentPosition(0);
				m_effectBufs[availableIndex].audio = pSound3D;
				m_effectBufs[availableIndex].tag = tag;
				m_effectBufs[availableIndex].volume = m_volume + tag->get_volume_adjust();
				CLAMP(m_effectBufs[availableIndex].volume, DSBVOLUME_MIN, DSBVOLUME_MAX);
				m_effectBufs[availableIndex].curVolume = m_effectBufs[availableIndex].volume;
				m_effectBufs[availableIndex].volumeTransitionAdjust = 0;
				m_effectBufs[availableIndex].pDSBuf->SetVolume(m_effectBufs[availableIndex].volume);
				m_effectBufs[availableIndex].bPlaying = true;
				m_effectBufs[availableIndex].dwLastWritePos = 0;
				m_effectBufs[availableIndex].dwNextWritePos = effect_initial_read_size;
				m_effectBufs[availableIndex].pDSBuf3D->SetMaxDistance(10000.0f, DS3D_DEFERRED);
				m_effectBufs[availableIndex].pDSBuf3D->SetMinDistance(100.0f, DS3D_DEFERRED);
				m_effectBufs[availableIndex].ms_duration = ms_duration;
				m_effectBufs[availableIndex].msLifetime = 0;

				if (pSound3D->get_world_object())
				{
					D3DXVECTOR3 v(*(D3DXVECTOR3*)&pSound3D->get_world_object()->get_position());
					m_effectBufs[availableIndex].pDSBuf3D->SetPosition(v.x, v.y, v.z, DS3D_DEFERRED);
					v = *(D3DXVECTOR3*)&pSound3D->get_world_object()->get_velocity();
					m_effectBufs[availableIndex].pDSBuf3D->SetVelocity(v.x, v.y, v.z, DS3D_DEFERRED);
					m_pDSListener->CommitDeferredSettings();
				}

				// todo check if wav file doesn't need loading into buf
				m_effectBufs[availableIndex].bMoreInBuffer = 
					pSound3D->fill_bufer(m_effectBufs[availableIndex].pDSBuf,
					0, effect_initial_read_size,
					&(m_effectBufs[availableIndex].dwNextWritePos));

				m_effectBufs[availableIndex].pDSBuf->Play(0, 0, DSBPLAY_LOOPING);
			}
		}

		void audio_manager::PlayMusic(audio_tag* tag, music* pMusic, int ms_duration)
		{
			int availableIndex = -1;
			int lowPriority = tag->get_priority();
			int lowestPriorityIndex = -1;
			int fadeOutMusicIndex = -1;

			// pick the appropriate buffer to use
			for (int i = 0; i < max_music_buffers && -1 == availableIndex; ++i)
			{
				// if one music tag is already playing, 
				// prepare to fade it out
				// try and find one that's not currently playing
				if (!m_musicBufs[i].bPlaying)
				{
					availableIndex = i;
				}
				else
				{
					// otherwise, if we can't find an empty slot, find one based on the 
					// priority of the audio tag

					// todo add another test, in case of multiple lower priority choices
					// like the buffer that's closest to completion, or check for similar sounds first
					if (m_musicBufs[i].tag->get_priority() < lowPriority)
					{
						lowPriority = m_musicBufs[i].tag->get_priority();
						lowestPriorityIndex = i;
					}

					// fade out the other playing music if the new one plays
					fadeOutMusicIndex = i;
				}
			}

			if (-1 == availableIndex)
			{
				if (-1 == lowestPriorityIndex)
				{
					// music is not high enough priority to play, so quit
					return;
				}
				else
				{
					StopBuffer(&m_musicBufs[lowestPriorityIndex]);
					availableIndex = lowestPriorityIndex;
				}
			}

			delete m_musicBufs[availableIndex].audio;
			m_musicBufs[availableIndex].audio = pMusic;
			m_musicBufs[availableIndex].tag = tag;
			m_musicBufs[availableIndex].volume = m_volume + tag->get_volume_adjust();
			CLAMP(m_musicBufs[availableIndex].volume, DSBVOLUME_MIN, DSBVOLUME_MAX);
			m_musicBufs[availableIndex].bMoreInBuffer = 
				m_musicBufs[availableIndex].audio->fill_bufer(m_musicBufs[availableIndex].pDSBuf, 0, music_initial_read_size);
			m_musicBufs[availableIndex].pDSBuf->SetCurrentPosition(0);
			m_musicBufs[availableIndex].bPlaying = true;
			m_musicBufs[availableIndex].dwLastWritePos = 0;
			m_musicBufs[availableIndex].dwNextWritePos = music_initial_read_size;
			m_musicBufs[availableIndex].ms_duration = ms_duration;
			m_musicBufs[availableIndex].msLifetime = 0;
			m_musicBufs[availableIndex].pDSBuf->Play(0, 0, DSBPLAY_LOOPING);

			if (fadeOutMusicIndex >= 0)
			{
				unsigned int totalTime = ((music*)(m_musicBufs[fadeOutMusicIndex].audio))->get_total_time();
				float timeLeft = ((float)totalTime) - ((float)(m_musicBufs[fadeOutMusicIndex].msLifetime));
				if (timeLeft > 0.25f)
				{	// take into account delay in updating by a max of a quarter second
					timeLeft -= 0.25;
					m_musicBufs[availableIndex].volumeTransitionAdjust = ((float)(m_musicBufs[availableIndex].volume-DSBVOLUME_MIN))/timeLeft;
					m_musicBufs[availableIndex].curVolume = DSBVOLUME_MIN;
					m_musicBufs[fadeOutMusicIndex].volume = DSBVOLUME_MIN;
					m_musicBufs[fadeOutMusicIndex].volumeTransitionAdjust = -m_musicBufs[availableIndex].volumeTransitionAdjust;
				}
				else
				{
					m_musicBufs[availableIndex].curVolume = m_musicBufs[availableIndex].volume;
					m_musicBufs[availableIndex].volumeTransitionAdjust = 0;
				}
			}
			else
			{
				m_musicBufs[availableIndex].curVolume = m_musicBufs[availableIndex].volume;
				m_musicBufs[availableIndex].volumeTransitionAdjust = 0;
			}

			m_musicBufs[availableIndex].pDSBuf->SetVolume(m_musicBufs[availableIndex].curVolume);
		}


		void audio_manager::UpdateBuffer(int msElapsed, 
			Buffer* buff,
			DWORD dwBufSize,
			DWORD dwWriteAmt)
		{
			ASSERT(buff && buff->pDSBuf);

			if (!buff->bPlaying)
				return;

			buff->msLifetime += msElapsed;

			DWORD dwPlayPos = 0;
			buff->pDSBuf->GetCurrentPosition(&dwPlayPos, NULL);

			// if duration is set to be longer than the sound, reset the next
			// write pos. This could be optimized for short effects, because most of
			// them will fit into the buffer at once, but it currently re-streams
			// the sound in the buffer to handle effects and music
			if (0 != buff->ms_duration && buff->msLifetime < buff->ms_duration)
			{
				if (!buff->bMoreInBuffer && 
					(dwPlayPos >= buff->dwLastWritePos && dwPlayPos <= buff->dwLastWritePos + dwWriteAmt))
				{
					buff->dwLastWritePos = 0;
					buff->dwNextWritePos = dwWriteAmt;
					buff->bMoreInBuffer = true;
					buff->audio->reset();
				}
			}

			// stream in next section of the sound into the buffer, if we've
			// passed the threshold point (one chunk before the end)
			if (buff->bMoreInBuffer)
			{
				if (dwPlayPos >= buff->dwLastWritePos && dwPlayPos <= buff->dwLastWritePos + dwWriteAmt)
				{
					// update the last position we wrote to the buffer
					buff->dwLastWritePos = buff->dwNextWritePos;

					// fill in more of the buffer
					DWORD dwFinalRead = 0;
					buff->bMoreInBuffer = buff->audio->fill_bufer(buff->pDSBuf, buff->dwLastWritePos, dwWriteAmt, &dwFinalRead);

					buff->dwNextWritePos += dwFinalRead;
					if (buff->dwNextWritePos >= dwBufSize)
						buff->dwNextWritePos = 0;

					if (!buff->bMoreInBuffer)	// no more sound so change over to non-looping playback
					{
						buff->pDSBuf->Play(0, 0, 0); 
						if (internal::base_audio::music == buff->audio->get_type())
						{
							buff->audio->finish();
							delete buff->audio;
							buff->audio = 0;
						}
					}
				}
			}
			else
			{
				if (dwPlayPos >= buff->dwNextWritePos && 
					dwPlayPos > buff->dwLastWritePos)
				{
					// there's no more to be read and we're past the end
					StopBuffer(buff);
				}
			}

			// update sound properties
			if (buff->bPlaying)
			{
				if (buff->pDSBuf3D && buff->audio && buff->audio->get_type() == internal::base_audio::sound3d)
				{
					sound3d* pSound3D = (sound3d*)(buff->audio);
					if (pSound3D->get_world_object())
					{
						D3DXVECTOR3 pos(*(D3DXVECTOR3*)&pSound3D->get_world_object()->get_position());
						D3DXVECTOR3 vel(*(D3DXVECTOR3*)&pSound3D->get_world_object()->get_velocity());

						// assumes settings are commited elswhere, in update()
						buff->pDSBuf3D->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED);
						buff->pDSBuf3D->SetVelocity(vel.x, vel.y, vel.z, DS3D_DEFERRED);
					}
				}
				else if (buff->audio && buff->audio->get_type() == internal::base_audio::music)
				{
					if ((buff->volumeTransitionAdjust < 0 && buff->curVolume > buff->volume) ||
						(buff->volumeTransitionAdjust > 0 && buff->curVolume < buff->volume))
					{
						buff->curVolume += (int)(buff->volumeTransitionAdjust*((float)msElapsed));
						if ((buff->volumeTransitionAdjust < 0 && buff->curVolume <= buff->volume) ||
							(buff->volumeTransitionAdjust > 0 && buff->curVolume >= buff->volume))
						{
							buff->curVolume = buff->volume;
							buff->volumeTransitionAdjust = 0;
						}
						CLAMP(buff->curVolume, DSBVOLUME_MIN, DSBVOLUME_MAX);
						buff->pDSBuf->SetVolume(buff->curVolume);
					}
				}
			}
		}


		void audio_manager::UpdateListener()
		{
			// update listener position
			if (m_pListenerCam)
			{
				DS3DLISTENER listenerInfo;
				listenerInfo.dwSize = sizeof(DS3DLISTENER);
				listenerInfo.vPosition = *(D3DXVECTOR3*)&m_pListenerCam->get_position();
				// currently camera does not keep track of velocity
				listenerInfo.vVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				listenerInfo.vOrientFront = *(D3DXVECTOR3*)&m_pListenerCam->get_front();
				listenerInfo.vOrientTop = *(D3DXVECTOR3*)&m_pListenerCam->get_up();
				listenerInfo.flDistanceFactor = m_distanceFactor;
				listenerInfo.flRolloffFactor = m_rolloffFactor;
				listenerInfo.flDopplerFactor = m_dopplerFactor;

				// assumes settings are commited elswhere, in update()
				m_pDSListener->SetAllParameters(&listenerInfo, DS3D_DEFERRED);
			}
		}


		void audio_manager::update(int msElapsed)
		{
			int i;
			AudioWaitingIter iToBePlayed = m_toBePlayed.begin();
			AudioWaitingIter endToBePlayed = m_toBePlayed.end();

			// update delayed sounds to see if we should play any yet.
			while (iToBePlayed != endToBePlayed)
			{
				if (iToBePlayed->ms_delay <= msElapsed)
				{
					play(iToBePlayed->tag, iToBePlayed->obj, iToBePlayed->ms_duration, 0, iToBePlayed->notify);
					iToBePlayed = m_toBePlayed.erase(iToBePlayed);
				}
				else
				{
					iToBePlayed->ms_delay -= msElapsed;
					++iToBePlayed;
				}
			}

			// update currently playing sounds
			for (i = 0; i < max_effect_buffers; ++i)
			{
				UpdateBuffer(msElapsed, &m_effectBufs[i], effect_buffer_size, effect_read_size);
			}

			for (i = 0; i < max_music_buffers; ++i)
			{
				UpdateBuffer(msElapsed, &m_musicBufs[i], music_buffer_size, music_read_size);
			}

			UpdateListener();

			// Apply all the updated 3D settings
			m_pDSListener->CommitDeferredSettings();
		}

		void audio_manager::set_listener_camera(camera* pCam)
		{
			m_pListenerCam = pCam;
		}


		// Normally wouldn't need the following functions,
		// but they're exposed so the sample can list all
		// the audio tags for the simple testing interface.
		int audio_manager::get_num_tags()
		{
			return (int)m_tags.size();
		}

		const char* audio_manager::get_tag_name(unsigned int index)
		{
			unsigned int i = 0;
			AudioTagIterator tagIter = m_tags.begin();

			while (i < index && i < m_tags.size())
			{
				++tagIter;
				++i;
			}

			return tagIter->first.c_str();
		}


		void audio_manager::StopAll()
		{
			int i;

			// stop buffers without called finish internal::base_audio on tags
			// (that might create more audio)
			for (i = 0; i < max_effect_buffers; ++i)
			{
				m_effectBufs[i].pDSBuf->Stop();
				m_effectBufs[i].bPlaying = false;
			}

			for (i = 0; i < max_music_buffers; ++i)
			{
				m_musicBufs[i].pDSBuf->Stop();
				m_musicBufs[i].bPlaying = false;
			}
		}

		void audio_manager::StopBuffer(Buffer* buff)
		{
			buff->pDSBuf->Stop();

			if (buff->audio)
			{
				buff->audio->finish();
			}

			buff->bPlaying = false;
		}


		unsigned int audio_manager::GetRemainingMusicPlayback()
		{
			unsigned int ms = 0;
			unsigned int totalTime;
			DWORD dwPlayPos;

			for (int i = 0; i < max_music_buffers; ++i)
			{
				if (m_musicBufs[i].bPlaying)
				{
					dwPlayPos = 0;
					m_musicBufs[i].pDSBuf->GetCurrentPosition(&dwPlayPos, NULL);
					totalTime = ((music*)(m_musicBufs[i].audio))->get_total_time();
					totalTime -= m_musicBufs[i].msLifetime % totalTime;

					if (totalTime > ms)
						ms = totalTime;
				}
			}

			return ms;
		}


		void audio_manager::SetOverallVolume(int volume)
		{
			int i;

			m_volume = volume;

			// Adjust volume for all playing buffers (taking into account
			// volume adjustments for each tag
			for (i = 0; i < max_effect_buffers; ++i)
			{
				if (m_effectBufs[i].bPlaying)
				{
					m_effectBufs[i].volume = m_volume + m_effectBufs[i].tag->get_volume_adjust();
					CLAMP(m_effectBufs[i].volume, DSBVOLUME_MIN, DSBVOLUME_MAX);
					m_effectBufs[i].pDSBuf->SetVolume(m_effectBufs[i].volume);
				}
			}

			for (i = 0; i < max_music_buffers; ++i)
			{
				if (m_musicBufs[i].bPlaying)
				{
					m_musicBufs[i].volume = m_volume + m_musicBufs[i].tag->get_volume_adjust();
					CLAMP(m_musicBufs[i].volume, DSBVOLUME_MIN, DSBVOLUME_MAX);
					m_musicBufs[i].pDSBuf->SetVolume(m_musicBufs[i].volume);
				}
			}
		}

	}
}