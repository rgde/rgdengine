#include "precompiled.h"

#include <rgde/render/screen_fader.h>
#include <rgde/render/sprites.h>

#include <rgde/base/log.h>
#include <rgde/base/xml_helpers.h>

namespace render
{
	fader::fader()
	{
		init();
	}

	fader::fader(const std::string &file_name)
	{
		init();
		load_from_xml(file_name);
	}

	void fader::init()
	{
		m_fTimeElapsed = 0.0f;
		m_fFadingTime = 0.0f;
		m_fUnfadingTime = 0.0f;
		m_mode = Fade;
		m_state = Disabled;
	}

	void fader::setColor(const math::Color &color)
	{
		m_color = color;
	}

	void fader::setFadingTime(float fFadingTime)
	{
		m_fFadingTime = fFadingTime;
	}

	void fader::setUnfadingTime(float fUnfadingTime)
	{
		m_fUnfadingTime = fUnfadingTime;
	}

	void fader::activate(Mode mode)
	{
		m_mode = mode;

		switch (m_mode)
		{
		case Fade:
		case FadeUnfade:
			m_state = Fading;
			break;

		case Unfade:
		case UnfadeFade:
			m_state = Unfading;
			break;
		};
	}


	void fader::load_from_xml(const std::string &file_name)
	{
		TiXmlDocument fader;
		{
			io::path_add_scoped p	("Faders/");
			if (!base::load_xml(file_name, fader))
			{
				base::lerr << "Can't load fader \"" << file_name << "\".";
				return;
			}
		}

		TiXmlHandle root(&fader);

		root = root.FirstChildElement("fader");

		m_fFadingTime = base::safe_read(root, "fading_time", 0.0f);
		m_fUnfadingTime = base::safe_read(root, "unfading_time", 0.0f);

		char r	= base::safe_read_attr(root, "color", "r", 0);
		char g	= base::safe_read_attr(root, "color", "g", 0);
		char b	= base::safe_read_attr(root, "color", "b", 0);
		char a	= base::safe_read_attr(root, "color", "a", 0);

		m_color = math::Color(r, g, b, a);
	}

	void fader::disable()
	{
		m_fTimeElapsed = 0.0f;
		m_state = Disabled;
	}

	void fader::render()
	{
		sprite sprite;
		sprite.color = m_color;
		float fAlphaFactor	= 1.0f;

		if (m_state == Unfading)
			fAlphaFactor = 1.0f - m_fTimeElapsed / m_fUnfadingTime;
		else
			fAlphaFactor = m_fTimeElapsed / m_fFadingTime;

		sprite.color.a = (char)((float)sprite.color.a * fAlphaFactor);
		sprite.pos = math::Vec2f(400.0f, 300.0f);
		sprite.rect = math::Rect(0.0f, 0.0f, 1.0f, 1.0f);
		sprite.size = math::Vec2f(800.0f, 600.0f);
		sprite.spin = 0.0f;
		sprite.priority = 20;

		//TODO: create own sprite renderer
		//TheSpriteManager::get().add_sprite(sprite);
	}

	void fader::update(float dt)
	{
		if (Disabled == m_state)
			return;

		m_fTimeElapsed += dt;

		switch (m_state)
		{
		case Fading:
			if (m_fTimeElapsed > m_fFadingTime)
				if (m_mode == Fade || m_mode == UnfadeFade)
					disable();
				else
				{
					m_fTimeElapsed -= m_fFadingTime;
					m_state = Unfading;
				}
			break;

		case Unfading:
			if (m_fTimeElapsed > m_fUnfadingTime)
				if (m_mode == Unfade || m_mode == FadeUnfade)
					disable();
				else
				{
					m_fTimeElapsed -= m_fUnfadingTime;
					m_state = Fading;
				}
			break;
		};

		if (Disabled != m_state)
			render();
	}
}