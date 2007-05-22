#include <rgde/engine.h>
#include <io.h>

void searchFiles(std::vector<std::string>& vFileNames, const std::string mask = "*.xml", const std::string& path = "")
{
	_finddata_t c_file;
	intptr_t hFile;

	// Find first .xml file in current directory 
	if( (hFile = _findfirst( (path + mask).c_str(), &c_file )) == -1L )
	{
		//printf( "No *.bmp files in current directory!\n" );
	}
	else{
		std::string file_name(c_file.name);
		vFileNames.push_back(file_name);

		while( _findnext( hFile, &c_file ) == 0 ){
			std::string file_name(c_file.name);
			vFileNames.push_back(file_name);
		}

		size_t size = vFileNames.size();

		_findclose( hFile );
	}
}

template<typename IndexType, typename VertexType>
void saveGeomData(std::string strFileName, std::vector<VertexType>& vb, std::vector<IndexType>& ib)
{
	std::ofstream out(strFileName.c_str(), std::ios::binary);
	uint vsize = sizeof(VertexType);
	uint vbsize = (uint)(vb.size() * vsize);
	out.write((char*)&vsize, sizeof(uint));		//Vertex size
	out.write((char*)&vbsize, sizeof(uint));	//Vertex Data Size
	out.write((char*)&(vb[0]), vbsize);				//Vertex Data

	uint isize = sizeof(IndexType);
	uint ibsize = (uint)(ib.size()*isize);

	out.write((char*)&isize, sizeof(unsigned int));	//Index size
	out.write((char*)&ibsize, sizeof(unsigned int));	//Index data size
	out.write((char*)&(ib[0]), ibsize);				//Index sata
}

typedef vertex::MeshVertex Vertex;
typedef unsigned short ushort;


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::vector<std::string> vMeshNames;

	searchFiles(vMeshNames);
	io::ScopePathChange path(".\\");

	for(unsigned int i = 0; i < vMeshNames.size(); ++i)
	{
		std::string strXmlFile = vMeshNames[i];

		std::vector<Vertex> vb;
		std::vector<ushort> ib;

		render::loadGeomDataFromXmlFile(strXmlFile, vb, ib);
		saveGeomData(strXmlFile + ".mesh", vb, ib);
	}

	return 0;
}