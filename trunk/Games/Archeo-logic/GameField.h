#pragma once
#include <engine.h>

#define GLASS_HEIGHT 28
#define BLOCK_SIZE 20

class GameField 
{
public:
	GameField();
	~GameField(void);

	bool LoadLevelFromFile(const std::string& levelfile);

	
	void Draw();

private:
	render::PTexture m_border1,m_border2;
	render::PTexture m_emptyBlock;

	std::vector<render::PTexture> m_filledBlocks;

	std::vector<DWORD> m_levelData;
	unsigned int m_levelWidth;
	unsigned int m_levelHeight;

	render::CSpriteManager m_spriteRenderer;

	void DrawBorderRect(math::Vec2f& topLeft, math::Vec2f& bottomRight,float width, render::PTexture border, render::PTexture corner);
	void RenderBorders();
};
