#pragma once

namespace audio
{
	namespace internal{
		class base_audio;
	}
	
	// Interface class to receive notifications that an audio
	// piece has finished playing 
	class listener
	{
	public:
		virtual void audio_finished(internal::base_audio* p) = 0;
	};

	struct vec3
	{
		vec3(float fx, float fy, float fz);
		float x, y, z;
	};

	// A class representing the player's view camera, 
	// with the position orientation information exposed
	// for the audio_manager
	struct camera
	{
		virtual vec3& get_position() = 0;
		virtual vec3& get_front() = 0;
		virtual vec3& get_up() = 0;
	};


	// This class represents an object in the game world 
	// (with position and velocity). 3D sounds have corresponding
	// WorldObjects, so they know how the sound is moving in the game. 
	class world_object 
	{
	public:
		world_object();

		void get_position(float& x, float& y, float& z) const;
		const vec3& get_position() const;
		void set_position(float x, float y, float z);

		const vec3& get_velocity() const;
		void set_velocity(float x, float y, float z);

	protected:
		vec3 m_pos;
		vec3 m_vel;
	};

	class system
	{
	public:
		system(void* window_handle /* hwnd on win32 */);
		~system();

		void update(int ms_elapsed);

		void stop_all();
		void set_volume(int volume);

		bool load(xml::node node);
		bool load(const char* file);

		int get_num_tags();
		const char* get_tag_name(size_t tag_index);

		void set_camera(camera* cam);

		void play(const char* tag_name);
		void play(size_t tag_index);

	private:
		class audio_manager* manager;
	};

//////////////////////////////////////////////////////////////////////////

	inline vec3::vec3(float fx, float fy, float fz)
		: x(fx), y(fy), z(fz)
	{
	}

//////////////////////////////////////////////////////////////////////////

	inline world_object::world_object()
		:	m_pos(0.0f, 0.0f, 0.0f),
		m_vel(0.0f, 0.0f, 0.0f)
	{
	}

	inline void world_object::get_position(float& x, float& y, float& z) const
	{
		x = m_pos.x;
		y = m_pos.y;
		z = m_pos.z;
	}

	inline const vec3& world_object::get_position() const
	{
		return m_pos;
	}

	inline void world_object::set_position(float x, float y, float z)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}

	inline const vec3& world_object::get_velocity() const
	{
		return m_vel;
	}

	inline void world_object::set_velocity(float x, float y, float z)
	{
		m_vel.x = x;
		m_vel.y = y;
		m_vel.z = z;
	}
}