#pragma once

#include "size.h"

#include <boost/shared_ptr.hpp>

namespace gui
{
	class Renderer;

	class  Texture
	{
	public:
		enum PixelFormat
		{        
			PF_RGB, //! Each pixel is 3 bytes. RGB in that order.
			PF_RGBA,//! Each pixel is 4 bytes. RGBA in that order.
			PF_COMPRESSED
		};

		explicit Texture(Renderer& owner) : m_owner(owner) {}
		virtual ~Texture() {}

		const Size& getSize() const { return m_size; }
		virtual float getWidth(void) const { return m_size.width; }
		virtual float getHeight(void) const { return m_size.height; }

		Renderer&	getRenderer(void) const { return m_owner; }

		virtual	void onDeviceLost(void) = 0;
		virtual	void onDeviceReset(void) = 0;

	protected:
		Texture& operator=(const Texture&) { return *this; }
		virtual void calculateMetrics() = 0;

	protected:
		Renderer& m_owner;
		Size m_size;
	};

	typedef boost::shared_ptr<Texture> TexturePtr;
}