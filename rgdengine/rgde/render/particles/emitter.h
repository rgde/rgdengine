/**--------------------------------------------------------------------------------------

									RGDE Engine

-----------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:	dxprg@mail.ru

Created: 7.06.2005
---------------------------------------------------------------------------------------*/
#pragma once

#include <rgde/io/io.h>

#include <rgde/math/transform.h>


namespace particles
{
	class emitter : public io::serialized_object//, public core::meta_class
	{
	public:
		enum Type {	Static, Spherical, Box };

		emitter(Type _type);
		virtual ~emitter();

		virtual void reset() = 0;
		virtual void update (float dt) = 0;

		virtual void render() = 0;
		virtual void debug_draw() = 0;

		virtual void set_fade (bool bFade) = 0;

		// getters
		inline math::frame_ptr get_transform() { return m_transform; }
		inline Type get_type() const { return m_type; }

	protected:
		virtual void to_stream(io::write_stream& wf) const;
		virtual void from_stream(io::read_stream& rf);

	protected:
		const Type		m_type;					// emitter type
		math::frame_ptr	m_transform;			// emitter transform node
	};

}