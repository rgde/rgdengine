#include <Windows.h>
#include <d3dx9.h>
#include "math.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9 			g_pD3D		 = NULL;
LPDIRECT3DDEVICE9		g_pd3dDevice = NULL;
LPDIRECT3DTEXTURE9		barck_texture	 = NULL;
LPDIRECT3DTEXTURE9		foliage_texture	 = NULL;

LPD3DXMESH				branch		 = NULL;
LPD3DXMESH				foliage		 = NULL;

#include "vertex.h"

#include "rand_tabke.h"

rand_table rnd;


//----------------------------------- TREE GEN --------------------------------------
//----------------------------------- TREE GEN --------------------------------------
//----------------------------------- TREE GEN --------------------------------------
#include <vector>

//branches mesh
//static vertex v[65000];
static std::vector<vertex> v(65000);
static DWORD vertices = 0;
//static WORD t[400000];
static std::vector<DWORD> t(400000);
static DWORD indices = 0;

//foliage mesh
static vertex vfl[65000];
static DWORD verticesf;
static WORD tfl[100000];
static DWORD indicesf;

struct branch_params
{
	vec3 base;	//start position of branch
	vec3 dir;	//start direction of grow
	float baser;	//start branch radius
	float topr;		//end branch radius
	float lenght;	//total lenght of branch
	float anglel;	//min branch twist per step
	float angleh;	//max branch twist per step
	float posrnd;	//grow direction randomize
	float radrnd;	//branch radius randomize
	float folimin;	//min foliages per step
	float folimax;	//max foliages per step
	float folisizemin;	//min size of foliage
	float folisizemax;	//max size of foliage
	float folimassmin;	//min mass of foliage (will pull it to grownd)
	float folimassmax;	//max mass of foliage (will pull it to grownd)
	float fatroot;	//first section radius multiplier - for root
	float branch;	//branching factor (1 will generate new branch every step,0-no branches)
	float maxthickf;//max radius of branch that foliage grow from
	int sections;	//total sections of branch
	int sectors;	//prism planes at begin of branch
	branch_params(){
		ZeroMemory( this, sizeof(branch_params));
	}
};

static int const STMAX=32;
static branch_params stack[STMAX];
static int top=0;

static void AddBranchSection(	int n, vec3 center, vec3 normal, float radius, float angle,
								vertex *vpt, float tv, float radrnd)
{
	vec3 binormal = Normalize(normal^vec3(0,0,1));
	vec3 tangent = Normalize(binormal^normal);

	float steptu =  2.0f / (n > 1 ? n-1 : 1);
	float step   =  steptu * PI;
	
	float tu=0;

	vertex *vptfirst = vpt;

	for(int i=0; i < n; i++, vpt++)
	{
		vpt->set_norm( binormal*cosf(angle)+tangent*sinf(angle) );
		*vpt = center+radius*(vpt->norm())*rnd(1.0f-radrnd,1.0f+radrnd);
		
		if(i==n-1) *vpt=*vptfirst;

		vpt->tu=tu;
		vpt->tv=tv;

		angle+=step;
		tu+=steptu;
	}
}

static DWORD SectionTriangles( DWORD index, WORD n1, WORD n2, DWORD *out )
{
	WORD count=0;
	DWORD ind2=index+n1;
	DWORD total=0;

	for(DWORD ind1=index; ind1<index+n1; ind1++)
	{
		count+=n2;

		if(count<n1)
		{
			*out++=ind1;
			*out++=ind2;
			*out++=ind1+1;
			total+=3;
		}
		else
		{
			*out++=ind1;
			*out++=ind2;
			*out++=ind1+1;
			*out++=ind1+1;
			*out++=ind2;
			*out++=ind2+1;
			ind2++;
			count-=n1;
			total+=6;
		}
	}
	return total;
}

//add 4 vertex square to branch which grows to dir at pos, weight make lean down and horizontally
static void AddFoliage( vec3 pos, vec3 dir, float size, float weight, vertex *v, WORD *t, WORD index )
{
	vec3 binormal=Normalize(dir^vec3(0,0,1));
	vec3 tangent=Normalize(binormal^dir);

	float angle = rnd(0, 2.0f*PI);
	float branch = rnd(0.5f, 0.8f);

	vec3 diry = Normalize( dir + branch*( binormal*cosf(angle) + tangent*sinf(angle) ) );
	diry.y -= weight;
	diry.Normalize();
	
	vec3 dirx = Normalize(dir^diry);
	dirx.y /= 1+weight;
	dirx.Normalize();

	tangent	= Normalize(diry^dirx);
	diry	= Normalize(dirx^tangent);
	
	//pos+=dir*rnd(0,1)*size;	

	
	vec3 dx = dirx*size*0.5f;
	vec3 dy = size*diry;
	
	vec3 n(0,1,0);

	*v++=vertex( pos - dx, n, 0, 1 );
	*v++=vertex( pos + dx, n, 1, 1 );

	float rndy = 0.6f;
	vec3 deviation = tangent*size*rnd(-rndy,rndy);
	*v++=vertex( deviation + pos + dy - dx, n, 0, 0 );
	*v++=vertex( deviation + pos + dy + dy, n, 1, 0 );
	
	//t1
	*t++=index; 
	*t++=index+2; 
	*t++=index+3; 

	//t2
	*t++=index; 
	*t++=index+3; 
	*t++=index+1; 
}

static void CreateBranch( branch_params &b )
{
	int sect=0;
	int oldsect=0;

	float angle=0;

	b.dir.Normalize();

	for(int i=0; i<=b.sections; i++)
	{
		oldsect=sect;

		float aspect = (b.sections-i)/float(b.sections);

		sect= 4 + int( (b.sectors-4) * aspect );

		if(i==b.sections) sect=1;

		float rad = rnd(-b.radrnd,b.radrnd) + b.topr + (b.baser - b.topr) * aspect;

		if(!i) 
			rad*=b.fatroot;
		
		AddBranchSection( sect, b.base, b.dir, rad, angle, &v[vertices], float(i&1), i ? b.radrnd : 0.5f );
		
		angle+=rnd(b.anglel,b.angleh);
		
		b.base += b.dir * b.lenght / b.sections;

		float _t = rnd(-b.posrnd,b.posrnd); 
		b.dir += vec3(_t, _t, _t);

		b.dir.Normalize();
		
		//foliage and child branches
		if(i>0&&i<b.sections-1)
		{
			int num=int( rnd(b.folimin,b.folimax) );	//foliages

			if(rad<b.maxthickf)
			{
				for(int j=0;j<num;j++)
				{
					AddFoliage( 
						b.base, 
						b.dir, 
						rnd(b.folisizemin,b.folisizemax), 
						rnd(b.folimassmin,b.folimassmax),
						&vfl[verticesf], 
						&tfl[indicesf], 
						verticesf 
						);

					verticesf+=4;
					indicesf+=6;
				}
			}

			if(rnd() < b.branch) 	//branch
			{
				if(top < STMAX)
				{
					stack[top]= b;
					stack[top].baser = rad * 0.7f;
					stack[top].sectors = sect;
				
					if(stack[top].sections<1) top--;

					top++;
				}
			}
		}

		if(i) indices += SectionTriangles( vertices-oldsect, oldsect, sect, &t[indices] );
		vertices += sect;
	}
}


float treehgt=0;

void CreateTree(){

	branch_params b,c;
	b.base=vec3(0,0,0);
	b.dir=vec3(0,1,0);
	//dir.z=0.01*sin(float(g_Tick)*0.001);
	b.baser				= rnd(0.05f, 0.25f);
	b.topr				= 0.05f;
	treehgt=b.lenght	= rnd(10.0f,20.0f);
	b.anglel			= 0.5f;
	b.angleh		= 1.0f;
	b.posrnd		= 0.1f;
	b.radrnd		= 0.01;
	b.sections		= rnd(10,40);
	b.sectors		= rnd(1,9);
	b.folimin		= 0.5f;
	b.folimax		= 2.0f;
	b.folisizemin	= 1.0f;
	b.folisizemax	= 2.0f;
	b.folimassmin	= rnd(0.1f, 0.5f);
	b.folimassmax	= rnd(0.7f, 5.0f);
	b.fatroot		= rnd(2.0f, 4.0f);
	b.branch		= rnd(0.5f, 1.0f);
	b.maxthickf		= 0.1f;

	float branchlenght	= 0.4f;
	float dirdiffuse	= 1.0f;

	//------------
	indices=0;
	vertices=0;
	indicesf=0;
	verticesf=0;

	b.dir=vec3(0,1,0);
	CreateBranch( b );
	while(top){
		top--;
		c=stack[top];
		c.dir+=vec3(rnd(-dirdiffuse,dirdiffuse),0,rnd(-dirdiffuse,dirdiffuse));
		c.dir.Normalize();
		c.sections=(stack[top].sections>>1);
		c.topr*=0.5f;
		c.lenght*=branchlenght;
		c.fatroot=1.0f;
		CreateBranch(c);
	}

	if(branch) branch->Release();
	D3DXCreateMeshFVF( indices/3, vertices, D3DXMESH_MANAGED|D3DXMESH_WRITEONLY|D3DXMESH_32BIT, VERTEX_FORMAT, g_pd3dDevice, &branch);
	vertex *verts;
	WORD *inds;
	branch->LockVertexBuffer(0,(void**)&verts);
	branch->LockIndexBuffer(0,(void**)&inds);
	memcpy(verts,&v[0],sizeof(vertex)*vertices);
	memcpy(inds,&t[0],sizeof(DWORD)*indices);
	branch->UnlockVertexBuffer();
	branch->UnlockIndexBuffer();

	if(foliage) foliage->Release();
	D3DXCreateMeshFVF( indicesf/3, verticesf, D3DXMESH_MANAGED|D3DXMESH_WRITEONLY, VERTEX_FORMAT, g_pd3dDevice, &foliage);
	foliage->LockVertexBuffer(0,(void**)&verts);
	foliage->LockIndexBuffer(0,(void**)&inds);
	memcpy(verts,vfl,sizeof(vertex)*verticesf);
	memcpy(inds,tfl,sizeof(WORD)*indicesf);
	foliage->UnlockVertexBuffer();
	foliage->UnlockIndexBuffer();/**/
}




//----------------------------------- TREE GEN END--------------------------------------
//----------------------------------- TREE GEN END--------------------------------------
//----------------------------------- TREE GEN END--------------------------------------






//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.Windowed = 1;
	d3dpp.hDeviceWindow=hWnd;
	d3dpp.PresentationInterval=D3DPRESENT_DONOTWAIT;

	g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice );
	if(!g_pd3dDevice)	g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
							D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice );

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	for(int i=0;i<8;i++){
		g_pd3dDevice->SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
		g_pd3dDevice->SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		g_pd3dDevice->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	}
	g_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL );
	g_pd3dDevice->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_MATERIAL );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, 1 );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHAREF, 128 );

	D3DXCreateTextureFromFileEx( g_pd3dDevice, "bark.jpg" , D3DX_DEFAULT, D3DX_DEFAULT,	D3DX_DEFAULT, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,	 D3DX_DEFAULT,	D3DX_DEFAULT,
		0, NULL, NULL, &barck_texture );	
	D3DXCreateTextureFromFileEx( g_pd3dDevice, "foliage.tga" , D3DX_DEFAULT, D3DX_DEFAULT,	D3DX_DEFAULT, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,	 D3DX_DEFAULT,	D3DX_DEFAULT,
		0, NULL, NULL, &foliage_texture );	

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Create the textures and vertex buffers
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	CreateTree();
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if( barck_texture != NULL )
		barck_texture->Release();

	if( foliage_texture != NULL )
		foliage_texture->Release();

	if( g_pd3dDevice != NULL )
		g_pd3dDevice->Release();

	if( g_pD3D != NULL )
		g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
vec3 position(35,12,0);
vec3 viewdir(-1,0,0);
VOID SetupMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matProj;
	D3DXMATRIXA16 matView;

	D3DXMatrixIdentity( &matWorld );
	//D3DXMatrixRotationY( &matWorld, timeGetTime()/1000.0f );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3 vEyePt=*((D3DXVECTOR3*)&position);
	D3DXVECTOR3 vLookatPt( 0.0f, treehgt*0.6f, 0.0f );
	//D3DXVECTOR3 vLookatPt=*((D3DXVECTOR3*)&(position+viewdir));
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	D3DXMatrixRotationY( &matWorld, timeGetTime()/1000.0f );

}

//-----------------------------------------------------------------------------
// Name: SetupLights()
// Desc: Sets up the lights and materials for the scene.
//-----------------------------------------------------------------------------
VOID SetupLights()
{
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    g_pd3dDevice->SetMaterial( &mtrl );

    D3DXVECTOR3 vecDir;
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type       = D3DLIGHT_DIRECTIONAL;
	//light.Type       = D3DLIGHT_POINT;
    light.Diffuse.r  = 1.0f;
    light.Diffuse.g  = 1.0f;
    light.Diffuse.b  = 1.0f;
    vecDir = D3DXVECTOR3(0,1.0f,1.0f);
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	light.Position=vecDir;
    light.Range       = 1000.0f;
	light.Attenuation0=0.1f;
	light.Attenuation1=0.1f;
	light.Attenuation2=0.1f;
    g_pd3dDevice->SetLight( 0, &light );
    g_pd3dDevice->LightEnable( 0, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    
    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00404040 );
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
						 D3DCOLOR_XRGB(132,170,170), 1.0f, 0 );
	g_pd3dDevice->BeginScene();

	SetupLights();
	SetupMatrices();
	g_pd3dDevice->SetTexture( 0, barck_texture );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	 D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	 D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	branch->DrawSubset(0);

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pd3dDevice->SetTexture( 0, foliage_texture );
	foliage->DrawSubset(0);
	
	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static float viewdist=35.0f;
	static float viewangle=0.0f;
	static float viewhgt=12.0f;
	switch( msg )
	{
		case WM_KEYDOWN:
			switch (wParam)
            {
			case VK_SPACE:
				CreateTree();
				break;
			case VK_UP:
				viewdist+=1.0f;
				break;
			case VK_DOWN:
				viewdist-=1.0f;
				break;
			case VK_LEFT:
				viewangle-=0.1f;
				break;
			case VK_RIGHT:
				viewangle+=0.1f;
				break;
			case VK_HOME:
				viewhgt+=1.0f;
				break;
			case VK_END:
				viewhgt-=1.0f;
				break;
            }
			position.SetYawPitch(viewangle,0);
			position*=viewdist;
			position.y=viewhgt;
            break;
		case WM_DESTROY:
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
					  GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
					  "D3D Tutorial", NULL };
	RegisterClassEx( &wc );
	HWND hWnd = CreateWindow( "D3D Tutorial", "Tree Gen",
							  WS_OVERLAPPEDWINDOW, 0, 0, 800, 600,
							  GetDesktopWindow(), NULL, wc.hInstance, NULL );
	InitD3D(hWnd);
	InitGeometry();
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while( 1 ){
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) ){
            if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else Render();
	}

	UnregisterClass( "D3D Tutorial", wc.hInstance );
	return 0;
}
