#pragma once

class CBlock
{
public:
	CBlock()
	{
		memset(m_mat,0,sizeof(int)*16);
		memset(m_matr,0,sizeof(int)*289);
		for(int i=0;i<=16;i++) { m_matr[0][i]=1; m_matr[16][i]=1; m_matr[i][0]=1; m_matr[i][16]=1;}
		x=6; y=12;
		int w[5][4][4] = { 0,0,1,0, 0,1,1,0, 0,1,0,0, 0,0,0,0, 
						0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0,
						0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0,
						0,0,1,0, 0,1,1,0, 0,0,1,0, 0,0,0,0,
						0,1,0,0, 0,1,1,0, 0,0,1,0, 0,0,0,0};	
		memcpy(m_blocks,w,sizeof(w));
		m_ipoints = 0;
		m_bstopped = true; m_bfin = false; m_ilastnum=0;

	}
	void reinit()
	{
		memset(m_mat,0,sizeof(int)*16);
		memset(m_matr,0,sizeof(int)*289);
		for(int i=0;i<=16;i++) { m_matr[0][i]=1; m_matr[16][i]=1; m_matr[i][0]=1; m_matr[i][16]=1;}

		m_ipoints = 0; m_ilastnum=0;
		m_bstopped = true; m_bfin = false;
	}
	void initBlock()
	{
		m_bstopped = false;
		x=6; y=12;
		srand(GetTickCount());
		m_icur = int(float(rand())/RAND_MAX*5);
		memcpy(m_mat,m_blocks[m_icur],sizeof(int)*16);
		
	}
	void rotate()
	{
		int m[4][4];
		memcpy(m,m_mat,sizeof(int)*16);
		int k = m_mat[1][1];
		m_mat[1][1]=m_mat[1][2]; m_mat[1][2]=m_mat[2][2]; m_mat[2][2]=m_mat[2][1]; m_mat[2][1]=k;
		int l = m_mat[0][2]; k = m_mat[0][1];
		m_mat[0][1]=m_mat[1][3]; m_mat[0][2]=m_mat[2][3];
		m_mat[1][3]=m_mat[3][2]; m_mat[2][3]=m_mat[3][1];
		m_mat[3][2]=m_mat[2][0]; m_mat[3][1]=m_mat[1][0];
		m_mat[2][0]=k; m_mat[1][0]=l;
		
		if(check(x,y)==0)
		{
			memcpy(m_mat,m,sizeof(int)*16);
		}

	}

	int checkloss()
	{
		for(int i=1;i<=15;i++) if(m_matr[15][i]!=0) return 1;
		return 0;
	}

	void deleteline(const int n)
	{
		for(int i=n;i<=14;i++)
			memcpy(m_matr[i],m_matr[i+1],sizeof(int)*17);
	}

	void checkline()
	{
		int num = 0;
		for(int j=15;j>=1;j--)
		{
			bool k = true;
			for(int i=1;i<=15;i++) if(m_matr[j][i]==0) { k=false; break;};
			if(k) { deleteline(j); m_ipoints+=15; num++;}
		}
		m_ilastnum=num;
		m_ipoints+=num*(num-1)*5;
	}

	int check(const int xx,const int yy)
	{
        for(int i=0;i<=3;i++)
			for(int j=0;j<=3;j++)
				if((m_mat[i][j]!=0)&&(m_matr[yy+i+1][xx+j+1]!=0)) return 0;
		return 1;
	}

	void stop()
	{
		for(int i=0;i<=3;i++)
			for(int j=0;j<=3;j++)
				if(m_mat[i][j]!=0) m_matr[(int)y+i+1][(int)x+j+1]=m_mat[i][j];
		checkline();
		if(checkloss()) {m_bfin = true; return;}

		m_bstopped=true;
	}

	void move(const int dir=2)
	{
		if(m_bfin)
		{
			WCHAR str[80];
			swprintf(str,L"¬аш счЄт %d \n—ыграть ещЄ?",m_ipoints);
			if(MessageBox(0,str,L"»гра закончилась",MB_YESNO|MB_ICONQUESTION)==IDYES)
				reinit(); else core::IApplication::Get()->close();
		}
		if(m_bstopped) initBlock(); else
		switch(dir)
		{
			case 0: 
				if(check((int)x-1,(int)y)) 
				{	x-=1;	}
				break;
			case 1: 
				if(check((int)x+1,(int)y)) 
				{	x+=1;	}
				break;
			case 2: 
				if((int)y==int(y*10)/10.f)
				{
					if(check((int)x,(int)y-1)) 
					{	y-=0.1f;	}
					else stop();
				} else y-=0.1f;
				break;
		}
	}

	float getx() { return x; }
	float gety() { return y; }

	int m_ipoints,m_ilastnum;
	int m_matr[17][17];
	int m_mat[4][4];
protected:
	int m_blocks[5][4][4];
	//render::PModel m_spGeom[5];
	int m_icur;
	float x,y;
	bool m_bstopped,m_bfin;
};