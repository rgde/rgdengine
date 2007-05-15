#include "GameField.h"

GameField::GameField()
{
	io::ScopePathAdd p("dagger/");
	m_border1 = render::ITexture::Create("border_1.jpg");
	m_border2 = render::ITexture::Create("border_2.png");
	m_levelWidth = 16;
	m_emptyBlock = render::ITexture::Create("empty.jpg");
}

GameField::~GameField(void)
{
}

bool GameField::LoadLevelFromFile(const std::string& levelfile)
{
	io::CFileSystem &fs	= io::TheFileSystem::Get();
	io::PReadStream in	= fs.findFile(levelfile);
	io::StreamToVector(m_levelData,in);
	

	if(m_levelData.empty())
		return false;

	if(m_levelData[0]!=0x31474144) //DAG1
		return false;

	m_levelWidth = m_levelData[1];
	m_levelHeight = m_levelData[2];
	//пока такая хрень тоже ахтунг как и предидущая.
	m_levelData.erase(m_levelData.begin(),m_levelData.begin()+3);
	return true;
}

void GameField::RenderBorders()
{
	
	render::SSprite border;
	border.size = math::Vec2f(BLOCK_SIZE,BLOCK_SIZE);
	border.color = 0xFFFFFFFF;

	float borderX = BLOCK_SIZE/2;
	float borderY = BLOCK_SIZE/2;

	for(int i = 1; i < m_levelWidth - 1; i++)
	{
		border.pTexture = m_border1;
		borderX = BLOCK_SIZE/2 + BLOCK_SIZE*i;

		border.pos = math::Vec2f(borderX,borderY);
		m_spriteRenderer.addSprite(border);

		border.pos = math::Vec2f(borderX,borderY+BLOCK_SIZE*(GLASS_HEIGHT+1));
		m_spriteRenderer.addSprite(border);
	}

	border.spin = PI_OVER_2;
	for(int i = 1; i<GLASS_HEIGHT-1; i++)
	{
		border.pTexture = m_border1;
		borderY = BLOCK_SIZE/2 + BLOCK_SIZE*i;

		border.pos = math::Vec2f(borderX,borderY);
		m_spriteRenderer.addSprite(border);

		border.pos = math::Vec2f(borderX+(m_levelWidth+1)*BLOCK_SIZE,borderY);
		m_spriteRenderer.addSprite(border);
	}

/*
	for(int i = 1; i<m_levelWidth; i++)
	{
		for(int k=0; k<GLASS_HEIGHT;k++)
		{
			border.pTexture = m_emptyBlock;
			border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*i,BLOCK_SIZE/2+BLOCK_SIZE*k);

		}
		border.pTexture = m_border1;
		border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*i,BLOCK_SIZE/2);
		m_spriteRenderer.addSprite(border);
		border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*i,BLOCK_SIZE/2 + BLOCK_SIZE*GLASS_HEIGHT);
		m_spriteRenderer.addSprite(border);
	}
	border.spin = PI_OVER_2;
	for(int j = 0; j<GLASS_HEIGHT; j++)
	{
		border.pTexture = m_border1;
		border.pos = math::Vec2f(BLOCK_SIZE/2,BLOCK_SIZE/2+BLOCK_SIZE*j);
		m_spriteRenderer.addSprite(border);
		border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*m_levelWidth,BLOCK_SIZE/2+BLOCK_SIZE*j);
		m_spriteRenderer.addSprite(border);
	}

	border.pTexture = m_border2;
	border.spin = 0;
	border.pos = math::Vec2f(BLOCK_SIZE/2,BLOCK_SIZE/2);
	m_spriteRenderer.addSprite(border);

	border.spin = -PI_OVER_2;
	border.pos = math::Vec2f(BLOCK_SIZE/2,BLOCK_SIZE/2+GLASS_HEIGHT*BLOCK_SIZE);
	m_spriteRenderer.addSprite(border);

	border.spin = -PI;
	border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*m_levelWidth,BLOCK_SIZE/2+GLASS_HEIGHT*BLOCK_SIZE);
	m_spriteRenderer.addSprite(border);

	border.spin = PI_OVER_2;
	border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*m_levelWidth,BLOCK_SIZE/2);
	m_spriteRenderer.addSprite(border);
*/

}

void GameField::Draw()
{
//	render::CLine2dManager& man = render::TheLine2dManager::Get();
//	man.addLine(math::Vec2f(0,0),math::Vec2f(800,600));
//	RenderBorders();

	DrawBorderRect(math::Vec2f(BLOCK_SIZE/2,BLOCK_SIZE/2),math::Vec2f(BLOCK_SIZE/2 + m_levelWidth*BLOCK_SIZE, BLOCK_SIZE/2+GLASS_HEIGHT*BLOCK_SIZE),BLOCK_SIZE,m_border1,m_border2);

}

void GameField::DrawBorderRect(math::Vec2f& topLeft, math::Vec2f& bottomRight, float width, render::PTexture border, render::PTexture corner)
{
	render::SSprite block;
	block.color = 0xFFFFFFFF;
	block.size = math::Vec2f(width,width);
	block.pTexture = corner;

	block.spin = 0;
	block.pos = topLeft;
	m_spriteRenderer.addSprite(block);

	block.spin = -PI_OVER_2;
	block.pos[1] = bottomRight[1];
	m_spriteRenderer.addSprite(block);

	block.spin = -PI;
	block.pos = bottomRight;
	m_spriteRenderer.addSprite(block);

	block.spin = PI_OVER_2;
	block.pos[1] = topLeft[1];
	m_spriteRenderer.addSprite(block);

	for(int i = 0;)
}