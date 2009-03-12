#pragma once

#include <boost/shared_ptr.hpp>

namespace rgde
{
	namespace render
	{
		typedef boost::shared_ptr<class device> device_ptr;
		typedef boost::shared_ptr<class font> font_ptr;
		typedef boost::shared_ptr<class resource> resource_ptr;
		typedef boost::shared_ptr<class surface> surface_ptr;
		typedef boost::shared_ptr<class base_texture> texture_ptr;
		typedef boost::shared_ptr<class vertex_declaration> vertex_declaration_ptr;
		typedef boost::shared_ptr<class vertex_buffer> vertex_buffer_ptr;
		typedef boost::shared_ptr<class index_buffer> index_buffer_ptr;
	}
}