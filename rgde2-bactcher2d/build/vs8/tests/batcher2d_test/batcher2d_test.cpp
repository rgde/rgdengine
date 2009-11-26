#include "stdafx.h"
#include "BatcherApplication.h"

int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::string title("Batcher2D test");
	std::wstring wtitle( title.begin(), title.end() );

	rgde::BatcherApplication app(250, 50, 640, 480, wtitle);

	rgde::render::primitives_2d::sprite_desc sprite;
	sprite.size = rgde::math::vec2f(20, 20);

	rgde::render::primitives_2d::sprite_desc sprite2;
	sprite2.pos = rgde::math::vec2f(50, 50);
	sprite2.size = rgde::math::vec2f(20, 20);
	sprite2.color = rgde::math::color::Blue;

	rgde::render::primitives_2d::sprite_desc sprite3;
	sprite3.pos = rgde::math::vec2f(80, 80);
	sprite3.size = rgde::math::vec2f(20, 20);
	sprite3.color = rgde::math::color::Red;

	app.add_sprite(sprite);
	app.add_sprite(sprite2);
	app.add_sprite(sprite3);

	app.run();

	return 0;
}

