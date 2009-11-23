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
		typedef std::vector<renderer_ptr> renderers;
		typedef renderers::iterator ren_iters;

		typedef std::map<std::string, renderers> frames_map;
		typedef frames_map::iterator frames_iter;

		static void clear_cached_data(); 

		static_emitter();
		static_emitter(const std::string& sequence_name, const std::string texture_name);
		virtual ~static_emitter();

		void render();
		void update(float dt);
		void debug_draw();

		void reset();

		inline bool is_fading()		const	{return m_is_fading;}
		inline void set_fade(bool b)			{m_is_fading = b;}

		inline int  getTimeShift()	const	{return m_time_shift;}
		inline void setTimeShift(int t)		{m_time_shift = t;}		

		inline bool is_visible()		const	{return m_is_visible;}
		inline void	setVisible(bool b)		{m_is_visible = b;}
		
		inline bool	isCycling()		const	{return m_cycling;}
		inline void	setCycling(bool b)		{m_cycling = b;}
		
		void		setIntense(bool intense);
		inline bool	getIntense()	const	{return m_intense;}

		inline std::string getTextureName() const {return m_texture_name;}
		void set_texture(std::string tex);

	protected:
		void loadTexture();
		void loadFrames(std::string file_name);
		void render(unsigned int frame_num);

		virtual void to_stream(io::write_stream& wf) const;
		virtual void from_stream(io::read_stream& rf);

	protected:
		render::texture_ptr	m_texture;
		renderers*			m_rames;
		
		static frames_map ms_frames;
		static std::map<std::string, maya_structs::animation> ms_frame_seq;

		// state flags
		bool		m_is_seq_loaded;
		bool		m_is_texture_loaded;

		bool		m_is_visible;						// виден ли эффект
		bool		m_cycling;						// цикличен ли он 

		float		m_fScale;
		
		std::string m_Name;							// имя файла с данными
		std::string m_texture_name;
		
		// для работы с анимированной текстурой
		bool			m_is_anim_texture_used;			
		unsigned char	m_ucCol;					// кол-во колонок
		unsigned char	m_ucRow;					// кол-во строк
		unsigned char	m_ucTexFrames;				// кол-во кадров
		unsigned char	m_ucTcol;					// кол-во колонок
		char			m_texture_fps;
		unsigned char	m_ucTrow;					// кол-во строк

		float			m_fLastTime;				// время когда был отрисован последний кадр
		float			m_fLastFrame;				// последний отрисованный кадр
		int				m_time_shift;				// смещение в кадрах от начального кадра проигрывания
		bool			m_intense;
		bool			m_is_fading;
	};
}