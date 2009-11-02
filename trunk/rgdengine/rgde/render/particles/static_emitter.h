/**--------------------------------------------------------------------------------------

									RGDE Engine

	-----------------------------------------------------------------------------
\author Denis V. Ovod, 2003

\author Andrew "Zlobnik" Chouprina, 2005
mail:	dxprg@mail.ru


Created:			1:9:2003 (Denis V. Ovod)
Modified for RGDE:	march-april 2005 (Zlobnik)
---------------------------------------------------------------------------------------*/
#pragma once

#include <rgde/render/particles/emitter.h>
#include <rgde/render/particles/anim_structs.h>
#include <rgde/render/particles/tank.h>


namespace particles{

	class  static_emitter : public emitter
	{
		friend class effect;

		static const unsigned file_version = 1002;
	public:
		typedef std::vector<renderer_ptr> PTanks;
		typedef PTanks::iterator PTanksIter;

		typedef std::map<std::string, PTanks> frames_map;
		typedef frames_map::iterator PFramesIter;

		static void ClearCachedData(); 

		static_emitter() : emitter(emitter::Static) {};
		static_emitter( const std::string& sequence_name, const std::string texture_name);
		virtual ~static_emitter();

		void render();
		void update(float dt);
		void debugDraw();

		void reset()
		{
			m_fLastFrame = 0;
			m_fLastTime = 0;
			m_is_visible = true;
			m_bIsFading = false;
		}

		inline bool isFading()		const	{return m_bIsFading;}
		inline void setFade(bool b)			{m_bIsFading = b;}

		inline int  getTimeShift()	const	{return m_time_shift;}
		inline void setTimeShift(int t)		{m_time_shift = t;}		

		inline bool isVisible()		const	{return m_is_visible;}
		inline void	setVisible(bool b)		{m_is_visible = b;}
		
		inline bool	isCycling()		const	{return m_bCycling;}
		inline void	setCycling(bool b)		{m_bCycling = b;}
		
		void		setIntense(bool intense);
		inline bool	getIntense()	const	{return m_bIntense;}

		inline std::string getTextureName() const {return m_TexName;}
		void setTexture(std::string tex);

	protected:
		void loadTexture();
		void loadFrames(std::string file_name);
		void render(unsigned int frame_num);

		virtual void toStream(io::write_stream& wf) const;
		virtual void fromStream(io::read_stream& rf);

	protected:
		render::texture_ptr	m_texture;
		PTanks*				m_Frames;
		
		static frames_map ms_PFrames;
		static std::map<std::string, maya_structs::animation> m_FrSeq;

		// state flags
		bool		m_bIsSeqLoaded;
		bool		m_bIsTexLoaded;

		bool		m_is_visible;						// виден ли эффект
		bool		m_bCycling;						// цикличен ли он 

		float		m_fScale;
		
		std::string m_Name;							// имя файла с данными
		std::string m_TexName;
		
		// для работы с анимированной текстурой
		bool			m_bIsAnimTextureUsed;			
		unsigned char	m_ucCol;					// кол-во колонок
		unsigned char	m_ucRow;					// кол-во строк
		unsigned char	m_ucTexFrames;				// кол-во кадров
		unsigned char	m_ucTcol;					// кол-во колонок
		char			m_cTexFps;
		unsigned char	m_ucTrow;					// кол-во строк

		float			m_fLastTime;				// время когда был отрисован последний кадр
		float			m_fLastFrame;				// последний отрисованный кадр
		int				m_time_shift;				// смещение в кадрах от начального кадра проигрывания
		bool			m_bIntense;
		bool			m_bIsFading;
	};
}