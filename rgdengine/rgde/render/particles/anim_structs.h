#pragma once

#include <rgde/io/io.h>

//////////////////////////////////////////////////////////////////////////
namespace particles{
namespace maya_structs{
//////////////////////////////////////////////////////////////////////////
struct static_particle
{
	float x,y,z;
	float r,g,b,a;
	float scale;
	float spin;

	// номер текстуры (или кадра в анимированной последовательности)
	int  tex_num;

	void ReScale(float _scale)
	{
		x *= _scale;
		y *= _scale;
		z *= _scale;
		scale *= _scale;
	}

	void save(io::write_stream& out, float _scale = 1.0f)
	{
		out << x * _scale << y * _scale << z * _scale
			<< r << g << b << a 
			<< scale * _scale
			<< spin
			<< tex_num;
	}

	void load(io::read_stream& in)
	{
		in	>> x >> y >> z
			>> r >> g >> b >> a 
			>> scale
			>> spin
			>> tex_num;
	}

};
//////////////////////////////////////////////////////////////////////////
struct anim_frame
{	
	unsigned int frame_number;
	unsigned int number_of_particles;
	std::vector<static_particle> particles;

	void ReScale(float scale)
	{
		for(unsigned int i = 0; i < number_of_particles; ++i)
			particles[i].ReScale(scale);
	}

	void save(io::write_stream& out, float scale = 1.0f)
	{
		out << frame_number
			<< number_of_particles;

		for(unsigned int i = 0; i < number_of_particles; ++i)
			particles[i].save(out, scale);
	}

	void load(io::read_stream& in)
	{
		in  >> frame_number
			>> number_of_particles;

		for(unsigned int i = 0; i < number_of_particles; ++i)
		{
			static_particle p; 
			p.load(in);
			particles.push_back(p);
		}
	}
};

struct animation
{
	virtual ~animation(){}
	// определяется если есть разные номера текстур
	bool isAnimatedTex;
	// вообще-то что пока не факт что надо :)
	float time_scale;
	// максимальное кол-во частиц в 1-м кадре
	unsigned int max_particles_number;
	// среднее кол-во частиц в 1м кадре, для определения общего
	// размера эфекта в памяти
	unsigned int average_particles_number;
	// список всех фреймов	
	std::vector<anim_frame> frames;

	void ReScale(float scale)
	{
		for (unsigned int i = 0; i < frames.size(); ++i)
			frames[i].ReScale(scale);
	}

	void save(io::write_stream& out, float scale = 1.0f)
	{
		char ver = 1; // версия

		time_scale = 1;
		max_particles_number = 180;
		average_particles_number = 70;

		out << ver
			<< isAnimatedTex
			<< time_scale
			<< max_particles_number
			<< average_particles_number
			<< (unsigned int)frames.size();

		for (unsigned int i = 0; i < frames.size(); ++i)
			frames[i].save(out, scale);
	}

	bool load(io::read_stream& in)
	{
		try{
			char ver;
			in >> ver;
			if (ver != 1) return false;

			in	>> isAnimatedTex
				>> time_scale
				>> max_particles_number
				>> average_particles_number;

			unsigned int frame_num;
			in >> frame_num;

			for (unsigned int i = 0; i < frame_num; ++i)
			{
				anim_frame f; 
				f.load(in);
				frames.push_back(f);
			}
			return true;
		}
		catch (...) {
			return false;
		}
	}
};
//////////////////////////////////////////////////////////////////////////
}
}