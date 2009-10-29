#pragma once

#include <rgde/audio/audio.h>


class TiXmlElement;

namespace audio
{

	// The base class for any audio tag to be read from the XML database
	class audio_tag : public audio::listener
	{
	public:
		audio_tag();
		virtual ~audio_tag();

		int get_priority() { return m_priority; }
		int GetVolumeAdjust(); // returns hundredths of decibels to adjust by
		int GetLoopDelay();  // returns milliseconds to delay next play of tag

		// called to audio create internal::base_audio object based on this tag's data
		virtual internal::base_audio* create_audio(world_object* pObj,
			int msDuration,
			int msDelay,
			audio::listener* pNotify) = 0;

		// called when to load tag data from XML file
		virtual bool LoadTag(TiXmlElement* pDOMElement);

		// called when audio created by this tag has finished playing
		virtual void audio_finished(internal::base_audio* pAudio);

	protected:
		int m_priority;						// priority for interrupting other audio
		int m_volAdjust, m_volAdjustRange;	// volume adjustment parameters
		int m_loopDelay, m_loopDelayRange;	// looping audio parameters
		int m_loopTimes, m_curLoop;		
	};


	typedef std::vector<audio_tag*> AudioTagVector;
	typedef AudioTagVector::iterator AudioTagIterator;


	// The EFFECT tag class - handles any basic 3D sound effect
	class AudioEffectTag : public audio_tag
	{
	public:
		AudioEffectTag();
		virtual ~AudioEffectTag();

		const char* GetFileName() { return m_fileName.c_str(); }

		float GetMinDistance() { return m_minDist; }
		float GetMaxDistance() { return m_maxDist; }
		int GetCascadeNumber() { return m_cascadeNum; }
		const char* GetCascadeTag() { return m_cascadeTag.c_str(); }

		virtual internal::base_audio* create_audio(world_object* pObj,
			int msDuration,
			int msDelay,
			audio::listener* pNotify);
		virtual bool LoadTag(TiXmlElement* pDOMElement);

	protected:
		std::string m_fileName;		// our .wav file
		float m_minDist;			// minimum distance for 3D audio culling
		float m_maxDist;			// maximum distance for 3D audio culling
		int m_cascadeNum;			// number of times to play before we switch
		std::string m_cascadeTag;	// to this cascade tag
	};


	// The AMBIENT tag class - this handles any ambient audio
	// it also allows for any of the EFFECT parameters
	class AudioAmbientTag : public AudioEffectTag
	{
	public:
		AudioAmbientTag();
		virtual ~AudioAmbientTag();

		virtual internal::base_audio* create_audio(world_object* pObj,
			int msDuration,
			int msDelay,
			audio::listener* pNotify);
		virtual bool LoadTag(TiXmlElement* pDOMElement);

		// called when audio created by this tag has finished playing
		virtual void audio_finished(internal::base_audio* pAudio);

	protected:
		void RandomizeWorldPosition();

		// members to keep track of randomized world position
		float m_xPos, m_xRange;
		float m_yPos, m_yRange;
		float m_zPos, m_zRange;
		world_object m_obj;
	};

	// The RANDOM tag controls selection of several tags with a certain
	// probability for each tag
	class AudioRandomTag : public audio_tag
	{
	public:
		AudioRandomTag();
		virtual ~AudioRandomTag();

		virtual internal::base_audio* create_audio(world_object* pObj,
			int msDuration,
			int msDelay,
			audio::listener* pNotify);
		virtual bool LoadTag(TiXmlElement* pDOMElement);

	protected:
		std::vector<std::string> m_tags;		// the tags to choose from
		std::vector<int> m_probabilities;		// their probabilities
	};


	// The MUSIC tag plays an .ogg file
	class AudioMusicTag : public audio_tag
	{
	public:
		AudioMusicTag();
		virtual ~AudioMusicTag();

		virtual internal::base_audio* create_audio(world_object* pObj,
			int msDuration,
			int msDelay,
			audio::listener* pNotify);

		virtual bool LoadTag(TiXmlElement* pDOMElement);

	protected:
		std::string m_file;		// the .ogg file
	};


	// The COMPOSITION tag plays a bunch of music tags in
	// a certain order
	class AudioCompositionTag : public audio_tag
	{
	public:
		AudioCompositionTag();
		virtual ~AudioCompositionTag();

		virtual internal::base_audio* create_audio(world_object* pObj,
			int msDuration,
			int msDelay,
			audio::listener* pNotify);

		virtual bool LoadTag(TiXmlElement* pDOMElement);

		virtual void audio_finished(internal::base_audio* pAudio);

	protected:
		enum Section
		{
			SECTION_NONE,
			SECTION_IN,
			SECTION_LOOP,
			SECTION_OUT
		};

		std::string m_inTag;			// the intro music
		std::string m_outTag;			// the exit music
		std::string m_loopTag;			// music to loop for the duration of the audio
		Section m_lastSection;			// the last section we played
		int m_crossFadeToOutTime;		// the time to crossfade between LOOP and OUT
	};


	// The GROUP tag plays a bunch of audio tags, and
	// any individual tag in the group can be run at a delay 
	// from the time the group is requested to play
	class AudioGroupTag : public audio_tag
	{
	public:
		AudioGroupTag();
		virtual ~AudioGroupTag();

		virtual internal::base_audio* create_audio(world_object* pObj,
			int msDuration,
			int msDelay,
			audio::listener* pNotify);
		virtual bool LoadTag(TiXmlElement* pDOMElement);

	protected:
		std::vector<std::string> m_tags;	// the tags in the group
		std::vector<int> m_delays;			// the delays for each tag
		std::vector<int> m_delayRanges;		// randomization for each delay
	};
}
