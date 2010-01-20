#pragma once
#include "window.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace gui
{
	class Image;
	class Imageset;


	class  Panel :
		public BaseWindow
	{
	public:
		typedef Panel Self;
		Panel(System& sys, const std::string& name = "");
		virtual ~Panel(void);

		static const char* GetType() { return "Panel"; }
		virtual const char* getType() { return Self::GetType(); }

		virtual void render(const Rect& finalRect, const Rect& finalClip);
		virtual void init(xml::node& node);

	protected:
		void	renderFrame(const Rect& dest, const Rect& clip);

	protected:
		ImagesetPtr m_imgset;

		// frame imagery
		const Image*	m_backImg;
		const Image*	m_topImg;
		const Image*	m_toplImg;
		const Image*	m_toprImg;
		const Image*	m_lImg;
		const Image*	m_rImg;
		const Image*	m_botImg;
		const Image*	m_botlImg;
		const Image*	m_botrImg;
	};
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif