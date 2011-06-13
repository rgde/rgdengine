#pragma once

#include <rgde/render/effect.h>
#include <rgde/render/geometry.h>

namespace particles
{
	class renderer
	{
	public:
		struct particle_t
		{
			math::vec3f		pos;
			math::vec2f		size;
			float			spin;
			unsigned long	color;
			unsigned int    tile; // texture tile
		};

		typedef std::vector< particle_t > particles_t;
		typedef particles_t::iterator particle_it;

		renderer();
		virtual ~renderer();

		void update();
		void render(render::texture_ptr texture, math::frame_ptr transform);

		inline particles_t& particles() { return m_particles; }

		void texture_tiling(int rows, int columns_total, int rows_total);

	protected:
		particles_t		m_particles;
		unsigned long	m_reserved_size; ///> Число частиц, для которого зарезервированы буферы

	private:
		render::effect_ptr		m_effect;

		typedef render::effect::parameter* EffectParam;
		EffectParam m_paramUpVec;
		EffectParam m_paramRightVec;
		EffectParam m_paramParticleTexture;
		EffectParam m_paramTransformMatrix;

		render::effect::technique* m_pRenderTechnique;

		typedef render::indexed_geometry<vertex::PositionColoredTextured2, false> geometry;
		geometry			m_geometry;

		int m_rows;
		float m_fInvRows;
		float m_fInvTotalColumns;
		float m_fInvTotalRows;
	};

	typedef boost::shared_ptr<renderer> renderer_ptr;
}