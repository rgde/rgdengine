#pragma once

#include <rgde/render/geometry.h>

namespace render
{
	template<class Vertex, bool Use32Indexes = false>
	class Generator
	{
	public:
		typedef indexed_geometry<Vertex, Use32Indexes> geometry;
		typedef boost::shared_ptr<geometry> PGeometry;
		
		static void GenerateGrid(PGeometry pGeometry, int nXResolution, int nZResolution,
									float fXScale = 1.0f, float fZScale = 1.0f,
									const math::Vec3f& CenterPos = math::Vec3f(0.0f, 0.0f, 0.0f))
		{
			geometry::Vertexes& vertices = pGeometry->lockVB();
			geometry::Indexes&  indices  = pGeometry->lockIB();

			vertices.resize(nXResolution*nZResolution);
			indices.resize((nXResolution - 1)*(nZResolution - 1)*2*3);

			int nVertCnt = 0;
			int nFaceCnt = 0;

			for(int x = 0; x < nXResolution; x++)
				for(int z = 0; z < nZResolution; z++)
				{
					vertices[nVertCnt].position = math::Vec3f((0.5f - (float)x/(nXResolution - 1))*fXScale, 0, (0.5f - (float)z/(nZResolution - 1))*fZScale);
					vertices[nVertCnt].normal   = math::Vec3f(0.0f, 1.0f, 0.0f);

					if(x < nXResolution - 1 && z < nZResolution - 1)
					{
						setFace(indices, nFaceCnt++, nVertCnt + 0, nVertCnt + 1, nVertCnt + 1 + nZResolution);
						setFace(indices, nFaceCnt++, nVertCnt + 1 + nZResolution, nVertCnt + nZResolution, nVertCnt + 0);
					}
					nVertCnt++;
				}

			pGeometry->unlockVB();
			pGeometry->unlockIB();
			pGeometry->getBBox();
		}

		static void GenerateCylinder(PGeometry pGeometry, int nRadialSegments = 16, int nHeightSegments = 16, float fRadius = 1.0f, float fHeight = 1.0f,
										const math::Vec3f& CenterPosition = math::Vec3f(0.0f, 0.0f, 0.0f))
		{
			GenerateGrid(pGeometry, nRadialSegments + 1, 3 + nHeightSegments);

			float fHalfOfHeight = fHeight/2.0f;

			geometry::Vertexes& vertices = pGeometry->lockVB();
			geometry::Indexes&  indices  = pGeometry->lockIB();

			float fUnit = 2*math::Math::PI/nRadialSegments;
			int nVertCnt = 0;
			for(int i = 0; i < nRadialSegments + 1; i++)
			{
				vertices[nVertCnt].normal = math::Vec3f(0, 1.0f, 0);
				vertices[nVertCnt++].position = math::Vec3f(0, fHalfOfHeight, 0) + CenterPosition;

				math::Vec3f v = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnit, 0, 1, 0))*math::Vec3f(0, 0, 1.0f);
				math::Vec3f normal = v;
				v = v*fRadius;

				for(int j = 0; j < nHeightSegments + 1; j++)
				{
					vertices[nVertCnt].normal = normal;
					vertices[nVertCnt++].position = math::Vec3f(v[0], v[1] + fHalfOfHeight - fHeight*((float)j/(float)nHeightSegments), v[2]) + CenterPosition;
				}
				
				vertices[nVertCnt].normal = math::Vec3f(0, -1.0f, 0);
				vertices[nVertCnt++].position = math::Vec3f(0, -fHalfOfHeight, 0) + CenterPosition;
			}

			pGeometry->unlockVB();
			pGeometry->unlockIB();
			pGeometry->getBBox();
		}

		static PGeometry CreateBox(const math::Vec3f& Size = math::Vec3f(1.0f, 1.0f, 1.0f), const math::Vec3f& CenterPos = math::Vec3f(0, 0, 0))
		{
			PGeometry pResult(new geometry);

			math::Vec3f HalfOfSize = Size/2.0f;
			math::Vec3f Normal     = math::Vec3f(0.5774f, 0.5774f, 0.5774f);

			geometry::Vertexes& vertices = pResult->lockVB();
			geometry::Indexes&  indices  = pResult->lockIB();

			vertices.resize(8);
			indices.resize(12*3);

			vertices[0].position = math::Vec3f(-HalfOfSize[0], -HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[1].position = math::Vec3f( HalfOfSize[0], -HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[2].position = math::Vec3f( HalfOfSize[0], -HalfOfSize[1],  HalfOfSize[2]) + CenterPos;
			vertices[3].position = math::Vec3f(-HalfOfSize[0], -HalfOfSize[1],  HalfOfSize[2]) + CenterPos;
			vertices[4].position = math::Vec3f(-HalfOfSize[0],  HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[5].position = math::Vec3f( HalfOfSize[0],  HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[6].position = math::Vec3f( HalfOfSize[0],  HalfOfSize[1],  HalfOfSize[2]) + CenterPos;
			vertices[7].position = math::Vec3f(-HalfOfSize[0],  HalfOfSize[1],  HalfOfSize[2]) + CenterPos;

			vertices[0].normal = math::Vec3f(-Normal[0], -Normal[1], -Normal[2]);
			vertices[1].normal = math::Vec3f( Normal[0], -Normal[1], -Normal[2]);
			vertices[2].normal = math::Vec3f( Normal[0], -Normal[1],  Normal[2]);
			vertices[3].normal = math::Vec3f(-Normal[0], -Normal[1],  Normal[2]);
			vertices[4].normal = math::Vec3f(-Normal[0],  Normal[1], -Normal[2]);
			vertices[5].normal = math::Vec3f( Normal[0],  Normal[1], -Normal[2]);
			vertices[6].normal = math::Vec3f( Normal[0],  Normal[1],  Normal[2]);
			vertices[7].normal = math::Vec3f(-Normal[0],  Normal[1],  Normal[2]);

			setFace(indices, 0,  0, 4, 5);
			setFace(indices, 1,  5, 1, 0);
			setFace(indices, 2,  1, 5, 6);
			setFace(indices, 3,  6, 2, 1);
			setFace(indices, 4,  2, 6, 7);
			setFace(indices, 5,  7, 3, 2);
			setFace(indices, 6,  3, 7, 4);
			setFace(indices, 7,  4, 0, 3);
			setFace(indices, 8,  0, 1, 2);
			setFace(indices, 9,  2, 3, 0);
			setFace(indices, 10, 5, 4, 7);
			setFace(indices, 11, 7, 6, 5);

			pResult->unlockVB();
			pResult->unlockIB();
			pResult->getBBox();//To rebuild BBox and BSphere

			return pResult;
		}
		
		static PGeometry CreateGrid(int nXResolution, int nZResolution,
									float fXScale = 1.0f, float fZScale = 1.0f,
									const math::Vec3f& CenterPos = math::Vec3f(0.0f, 0.0f, 0.0f))
		{
			PGeometry pResult(new geometry);

			GenerateGrid(pResult, nXResolution, nZResolution, fXScale, fZScale, CenterPos);

			return pResult;
		}
		
		static PGeometry CreateCylinder(int nRadialSegments = 16, int nHeightSegments = 16, float fRadius = 1.0f, float fHeight = 1.0f,
										const math::Vec3f& CenterPosition = math::Vec3f(0.0f, 0.0f, 0.0f))
		{
			PGeometry pResult = CreateGrid(nRadialSegments + 1, 3 + nHeightSegments);

			GenerateCylinder(pResult, nRadialSegments, nHeightSegments, fRadius, fHeight, CenterPosition);

			return pResult;
		}
		
		static PGeometry CreateCone(int nStep)
		{
			PGeometry pResult = CreateGrid(nStep + 1, 3);

			geometry::Vertexes& vertices = pResult->lockVB();
			geometry::Indexes&  indices  = pResult->lockIB();

			float fUnit = 2*math::Math::PI/nStep;
			int nVertCnt = 0;
			for(int i = 0; i < nStep + 1; i++)
			{
				vertices[nVertCnt++].position = math::Vec3f(0, 1, 0);
				vertices[nVertCnt++].position = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnit, 0, 1, 0))*math::Vec3f(0, 0, 1);
				vertices[nVertCnt++].position = math::Vec3f(0, 0, 0);
			}

			pResult->unlockVB();
			pResult->unlockIB();
			pResult->getBBox();

			return pResult;
		}

		static PGeometry CreateSphere(int nStepLng, int nStepLat)
		{
			PGeometry pResult = CreateGrid(nStepLng + 1, (nStepLat/2) + 1);

			geometry::Vertexes& vertices = pResult->lockVB();
			geometry::Indexes&  indices  = pResult->lockIB();

			float fUnitLng = 2*math::Math::PI/nStepLng;
			float fUnitLat = math::Math::PI/(nStepLat/2);
			int nVertCnt = 0;
			for(int i = 0; i < nStepLng + 1; i++)
				for(int j = 0; j < (nStepLat/2) + 1; j++)
					vertices[nVertCnt++].position = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnitLng, 0, 1, 0))*math::makeRot<math::Matrix33f>(math::AxisAnglef(j*fUnitLat, 1, 0, 0))*math::Vec3f(0, 1, 0);

			pResult->unlockVB();
			pResult->unlockIB();
			pResult->getBBox();

			return pResult;
		}

		static PGeometry CreateHemis(int nStepLng, int nStepLat)
		{
			PGeometry pResult = CreateGrid(nStepLng + 1, (nStepLat/4) + 2);

			geometry::Vertexes& vertices = pResult->lockVB();
			geometry::Indexes&  indices  = pResult->lockIB();

			float fUnitLng = 2*math::Math::PI/nStepLng;
			float fUnitLat = math::Math::PI/(nStepLat/2);
			int nVertCnt = 0;
			for(int i = 0; i < nStepLng + 1; i++)
			{
				for(int j = 0; j < (nStepLat/4) + 1; j++)
					vertices[nVertCnt++].position =  math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnitLng, 0, 1, 0))*math::makeRot<math::Matrix33f>(math::AxisAnglef(j*fUnitLat, 1, 0, 0))*math::Vec3f(0, 1, 0);

				vertices[nVertCnt++].position = math::Vec3f(0, 0, 0);
			}

			pResult->unlockVB();
			pResult->unlockIB();
			pResult->getBBox();

			return pResult;
		}

		static PGeometry CreateTorus(float fRadMajor, float fRadMinor, int nStepMajor, int nStepMinor)
		{
			PGeometry pResult = CreateGrid(nStepMajor + 1, nStepMinor + 1);

			geometry::Vertexes& vertices = pResult->lockVB();
			geometry::Indexes&  indices  = pResult->lockIB();

			float fUnitMajor = 2*math::Math::PI/nStepMajor;
			float fUnitMinor = 2*math::Math::PI/nStepMinor;
			int nVertCnt = 0;
			for(int i = 0; i < nStepMajor + 1; i++)
				for(int j = 0; j < nStepMinor + 1; j++)
					vertices[nVertCnt++].position = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnitMajor, 0, 1, 0))*
													math::makeTrans<math::Matrix33f>(math::Vec3f(0, 0, fRadMajor))*
													math::makeRot<math::Matrix33f>(math::AxisAnglef(j*fUnitMinor, 1, 0, 0))*
													math::Vec3f(0, fRadMinor, 0);

			pResult->unlockVB();
			pResult->unlockIB();
			pResult->getBBox();

			return pResult;
		}

		static PGeometry CreateOcta()
		{
			PGeometry pResult(new geometry);

			geometry::Vertexes& vertices = pResult->lockVB();
			geometry::Indexes&  indices  = pResult->lockIB();

			vertices.resize(6);
			indices.resize(8*3);

			vertices[0].position = math::Vec3f(0, 0, 0);
			vertices[1].position = math::Vec3f(1, 0, 0);
			vertices[2].position = math::Vec3f(1, 0, 1);
			vertices[3].position = math::Vec3f(0, 0, 1);
			vertices[4].position = math::Vec3f(0.5f, -1, 0.5f);
			vertices[5].position = math::Vec3f(0.5f, 1, 0.5f);

			setFace(indices, 0, 0, 1, 4);
			setFace(indices, 1, 1, 2, 4);
			setFace(indices, 2, 2, 3, 4);
			setFace(indices, 3, 3, 0, 4);
			setFace(indices, 4, 0, 5, 1);
			setFace(indices, 5, 1, 5, 2);
			setFace(indices, 6, 2, 5, 3);
			setFace(indices, 7, 3, 5, 0);

			pResult->unlockVB();
			pResult->unlockIB();
			pResult->getBBox();

			return pResult;
		}

		static PGeometry CreateTetra()
		{
			PGeometry pResult(new geometry);

			geometry::Vertexes& vertices = pResult->lockVB();
			geometry::Indexes&  indices  = pResult->lockIB();

			vertices.resize(4);
			indices.resize(4*3);

			vertices[0].position = math::Vec3f(0, 0, 0);
			vertices[1].position = math::Vec3f(1, 1, 0);
			vertices[2].position = math::Vec3f(1, 1, 1);
			vertices[3].position = math::Vec3f(1, 0, 1);

			setFace(indices, 0, 0, 1, 3);
			setFace(indices, 1, 0, 2, 1);
			setFace(indices, 2, 0, 3, 2);
			setFace(indices, 3, 1, 2, 3);

			pResult->unlockVB();
			pResult->unlockIB();
			pResult->getBBox();

			return pResult;
		}


		private:
			static void setFace(typename geometry::Indexes& array, unsigned face_number, unsigned i1, unsigned i2, unsigned i3)
			{
				array[face_number*3 + 0] = i1;
				array[face_number*3 + 1] = i2;
				array[face_number*3 + 2] = i3;
			}
	};
}