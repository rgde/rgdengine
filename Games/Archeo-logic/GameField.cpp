#include "GameField.h"

GameField::GameField()
{
	io::ScopePathAdd p("dagger/");
	m_border1 = render::ITexture::Create("border_1.jpg");
	m_border2 = render::ITexture::Create("border_2.png");
//	m_emptyBlock = render::ITexture::Create("empty.jpg");
}

GameField::~GameField(void)
{
}

void GameField::LoadLevelFromFile(const std::string& levelfile)
{
/* ѕолночный бред, временный вектор не нужен
	io::CFileSystem &fs	= io::TheFileSystem::Get();

	io::PReadStream in	= fs.findFile(levelfile);
	std::vector<int> data;
	io::StreamToVector(data,in);

	if(data.empty())
		return;

	if(data[0]!=0x31474144) //DAG1
		return;

	m_levelWidth = data[1];
	m_levelHeight = data[2];
	m_levelData.resize(m_levelWidth*m_levelHeight);
	std::copy(data.begin()+3,data.end(),m_levelData.begin());
*/
}

void GameField::RenderBorders()
{
	
	render::SSprite border;
	border.size = math::Vec2f(BLOCK_SIZE,BLOCK_SIZE);
	border.color = 0xFFFFFFFF;

	border.pTexture = m_border2;
	border.pos = math::Vec2f(100,100);
	m_spriteRenderer.addSprite(border);

	border.pTexture = m_border1;
	border.pos = math::Vec2f(100+BLOCK_SIZE,100);
	m_spriteRenderer.addSprite(border);


/*
	border.pTexture = m_border1;
	for(int i = 1; i<m_levelWidth; i++)
	{
		border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*i,BLOCK_SIZE/2);
		m_spriteRenderer.addSprite(border);
		border.pos = math::Vec2f(BLOCK_SIZE/2+BLOCK_SIZE*i,BLOCK_SIZE/2 + BLOCK_SIZE*GLASS_HEIGHT);
		m_spriteRenderer.addSprite(border);
	}
	border.spin = PI_OVER_2;
	for(int j = 1; j<GLASS_HEIGHT; j++)
	{
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
	RenderBorders();

}