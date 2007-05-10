#pragma once
#include <engine.h>

#define GLASS_HEIGHT 20
#define BLOCK_SIZE 128

class GameField 
{
public:
	GameField();
	~GameField(void);

	void LoadLevelFromFile(const std::string& levelfile);

	
	void Draw();

private:
	render::PTexture m_border1,m_border2;
	render::PTexture m_emptyBlock;

	std::vector<render::PTexture> m_filledBlocks;

	std::vector<DWORD> m_levelData;
	unsigned int m_levelWidth;
	unsigned int m_levelHeight;

	render::CSpriteManager m_spriteRenderer;
	void RenderBorders();
};
