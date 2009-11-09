#pragma once

#include <rgde/render/geometry.h>

namespace render
{
	template<class Vertex, bool Use32Indexes = false>
	class Generator
	{
	public:
		typedef indexed_geometry<Vertex, Use32Indexes> geometry;
		typedef boost::shared_ptr<geometry> geometry_ptr;
		
		static void GenerateGrid(geometry_ptr pGeometry, int nXResolution, int nZResolution,
									float fXScale = 1.0f, float fZScale = 1.0f,
									const math::vec3f& CenterPos = math::vec3f(0.0f, 0.0f, 0.0f))
		{
			geometry::vertexies& vertices = pGeometry->lock_vb();
			geometry::indexies&  indices  = pGeometry->lock_ib();

			vertices.resize(nXResolution*nZResolution);
			indices.resize((nXResolution - 1)*(nZResolution - 1)*2*3);

			int nVertCnt = 0;
			int nFaceCnt = 0;

			for(int x = 0; x < nXResolution; x++)
				for(int z = 0; z < nZResolution; z++)
				{
					vertices[nVertCnt].position = math::vec3f((0.5f - (float)x/(nXResolution - 1))*fXScale, 0, (0.5f - (float)z/(nZResolution - 1))*fZScale);
					vertices[nVertCnt].normal   = math::vec3f(0.0f, 1.0f, 0.0f);

					if(x < nXResolution - 1 && z < nZResolution - 1)
					{
						setFace(indices, nFaceCnt++, nVertCnt + 0, nVertCnt + 1, nVertCnt + 1 + nZResolution);
						setFace(indices, nFaceCnt++, nVertCnt + 1 + nZResolution, nVertCnt + nZResolution, nVertCnt + 0);
					}
					nVertCnt++;
				}

			pGeometry->unlock_vb();
			pGeometry->unlock_ib();
			pGeometry->getBBox();
		}

		static void GenerateCylinder(geometry_ptr pGeometry, int nRadialSegments = 16, int nHeightSegments = 16, float fRadius = 1.0f, float fHeight = 1.0f,
										const math::vec3f& CenterPosition = math::vec3f(0.0f, 0.0f, 0.0f))
		{
			GenerateGrid(pGeometry, nRadialSegments + 1, 3 + nHeightSegments);

			float fHalfOfHeight = fHeight/2.0f;

			geometry::vertexies& vertices = pGeometry->lock_vb();
			geometry::indexies&  indices  = pGeometry->lock_ib();

			float fUnit = 2*math::Math::PI/nRadialSegments;
			int nVertCnt = 0;
			for(int i = 0; i < nRadialSegments + 1; i++)
			{
				vertices[nVertCnt].normal = math::vec3f(0, 1.0f, 0);
				vertices[nVertCnt++].position = math::vec3f(0, fHalfOfHeight, 0) + CenterPosition;

				math::vec3f v = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnit, 0, 1, 0))*math::vec3f(0, 0, 1.0f);
				math::vec3f normal = v;
				v = v*fRadius;

				for(int j = 0; j < nHeightSegments + 1; j++)
				{
					vertices[nVertCnt].normal = normal;
					vertices[nVertCnt++].position = math::vec3f(v[0], v[1] + fHalfOfHeight - fHeight*((float)j/(float)nHeightSegments), v[2]) + CenterPosition;
				}
				
				vertices[nVertCnt].normal = math::vec3f(0, -1.0f, 0);
				vertices[nVertCnt++].position = math::vec3f(0, -fHalfOfHeight, 0) + CenterPosition;
			}

			pGeometry->unlock_vb();
			pGeometry->unlock_ib();
			pGeometry->getBBox();
		}

		static geometry_ptr CreateBox(const math::vec3f& Size = math::vec3f(1.0f, 1.0f, 1.0f), const math::vec3f& CenterPos = math::vec3f(0, 0, 0))
		{
			geometry_ptr pResult(new geometry);

			math::vec3f HalfOfSize = Size/2.0f;
			math::vec3f Normal     = math::vec3f(0.5774f, 0.5774f, 0.5774f);

			geometry::vertexies& vertices = pResult->lock_vb();
			geometry::indexies&  indices  = pResult->lock_ib();

			vertices.resize(8);
			indices.resize(12*3);

			vertices[0].position = math::vec3f(-HalfOfSize[0], -HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[1].position = math::vec3f( HalfOfSize[0], -HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[2].position = math::vec3f( HalfOfSize[0], -HalfOfSize[1],  HalfOfSize[2]) + CenterPos;
			vertices[3].position = math::vec3f(-HalfOfSize[0], -HalfOfSize[1],  HalfOfSize[2]) + CenterPos;
			vertices[4].position = math::vec3f(-HalfOfSize[0],  HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[5].position = math::vec3f( HalfOfSize[0],  HalfOfSize[1], -HalfOfSize[2]) + CenterPos;
			vertices[6].position = math::vec3f( HalfOfSize[0],  HalfOfSize[1],  HalfOfSize[2]) + CenterPos;
			vertices[7].position = math::vec3f(-HalfOfSize[0],  HalfOfSize[1],  HalfOfSize[2]) + CenterPos;

			vertices[0].normal = math::vec3f(-Normal[0], -Normal[1], -Normal[2]);
			vertices[1].normal = math::vec3f( Normal[0], -Normal[1], -Normal[2]);
			vertices[2].normal = math::vec3f( Normal[0], -Normal[1],  Normal[2]);
			vertices[3].normal = math::vec3f(-Normal[0], -Normal[1],  Normal[2]);
			vertices[4].normal = math::vec3f(-Normal[0],  Normal[1], -Normal[2]);
			vertices[5].normal = math::vec3f( Normal[0],  Normal[1], -Normal[2]);
			vertices[6].normal = math::vec3f( Normal[0],  Normal[1],  Normal[2]);
			vertices[7].normal = math::vec3f(-Normal[0],  Normal[1],  Normal[2]);

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

			pResult->unlock_vb();
			pResult->unlock_ib();
			pResult->getBBox();//To rebuild BBox and BSphere

			return pResult;
		}
		
		static geometry_ptr CreateGrid(int nXResolution, int nZResolution,
									float fXScale = 1.0f, float fZScale = 1.0f,
									const math::vec3f& CenterPos = math::vec3f(0.0f, 0.0f, 0.0f))
		{
			geometry_ptr pResult(new geometry);

			GenerateGrid(pResult, nXResolution, nZResolution, fXScale, fZScale, CenterPos);

			return pResult;
		}
		
		static geometry_ptr CreateCylinder(int nRadialSegments = 16, int nHeightSegments = 16, float fRadius = 1.0f, float fHeight = 1.0f,
										const math::vec3f& CenterPosition = math::vec3f(0.0f, 0.0f, 0.0f))
		{
			geometry_ptr pResult = CreateGrid(nRadialSegments + 1, 3 + nHeightSegments);

			GenerateCylinder(pResult, nRadialSegments, nHeightSegments, fRadius, fHeight, CenterPosition);

			return pResult;
		}
		
		static geometry_ptr CreateCone(int nStep)
		{
			geometry_ptr pResult = CreateGrid(nStep + 1, 3);

			geometry::vertexies& vertices = pResult->lock_vb();
			geometry::indexies&  indices  = pResult->lock_ib();

			float fUnit = 2*math::Math::PI/nStep;
			int nVertCnt = 0;
			for(int i = 0; i < nStep + 1; i++)
			{
				vertices[nVertCnt++].position = math::vec3f(0, 1, 0);
				vertices[nVertCnt++].position = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnit, 0, 1, 0))*math::vec3f(0, 0, 1);
				vertices[nVertCnt++].position = math::vec3f(0, 0, 0);
			}

			pResult->unlock_vb();
			pResult->unlock_ib();
			pResult->getBBox();

			return pResult;
		}

		static geometry_ptr CreateSphere(int nStepLng, int nStepLat)
		{
			geometry_ptr pResult = CreateGrid(nStepLng + 1, (nStepLat/2) + 1);

			geometry::vertexies& vertices = pResult->lock_vb();
			geometry::indexies&  indices  = pResult->lock_ib();

			float fUnitLng = 2*math::Math::PI/nStepLng;
			float fUnitLat = math::Math::PI/(nStepLat/2);
			int nVertCnt = 0;
			for(int i = 0; i < nStepLng + 1; i++)
				for(int j = 0; j < (nStepLat/2) + 1; j++)
					vertices[nVertCnt++].position = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnitLng, 0, 1, 0))*math::makeRot<math::Matrix33f>(math::AxisAnglef(j*fUnitLat, 1, 0, 0))*math::vec3f(0, 1, 0);

			pResult->unlock_vb();
			pResult->unlock_ib();
			pResult->getBBox();

			return pResult;
		}

		static geometry_ptr CreateHemis(int nStepLng, int nStepLat)
		{
			geometry_ptr pResult = CreateGrid(nStepLng + 1, (nStepLat/4) + 2);

			geometry::vertexies& vertices = pResult->lock_vb();
			geometry::indexies&  indices  = pResult->lock_ib();

			float fUnitLng = 2*math::Math::PI/nStepLng;
			float fUnitLat = math::Math::PI/(nStepLat/2);
			int nVertCnt = 0;
			for(int i = 0; i < nStepLng + 1; i++)
			{
				for(int j = 0; j < (nStepLat/4) + 1; j++)
					vertices[nVertCnt++].position =  math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnitLng, 0, 1, 0))*math::makeRot<math::Matrix33f>(math::AxisAnglef(j*fUnitLat, 1, 0, 0))*math::vec3f(0, 1, 0);

				vertices[nVertCnt++].position = math::vec3f(0, 0, 0);
			}

			pResult->unlock_vb();
			pResult->unlock_ib();
			pResult->getBBox();

			return pResult;
		}

		static geometry_ptr CreateTorus(float fRadMajor, float fRadMinor, int nStepMajor, int nStepMinor)
		{
			geometry_ptr pResult = CreateGrid(nStepMajor + 1, nStepMinor + 1);

			geometry::vertexies& vertices = pResult->lock_vb();
			geometry::indexies&  indices  = pResult->lock_ib();

			float fUnitMajor = 2*math::Math::PI/nStepMajor;
			float fUnitMinor = 2*math::Math::PI/nStepMinor;
			int nVertCnt = 0;
			for(int i = 0; i < nStepMajor + 1; i++)
				for(int j = 0; j < nStepMinor + 1; j++)
					vertices[nVertCnt++].position = math::makeRot<math::Matrix33f>(math::AxisAnglef(i*fUnitMajor, 0, 1, 0))*
													math::makeTrans<math::Matrix33f>(math::vec3f(0, 0, fRadMajor))*
													math::makeRot<math::Matrix33f>(math::AxisAnglef(j*fUnitMinor, 1, 0, 0))*
													math::vec3f(0, fRadMinor, 0);

			pResult->unlock_vb();
			pResult->unlock_ib();
			pResult->getBBox();

			return pResult;
		}

		static geometry_ptr CreateOcta()
		{
			geometry_ptr pResult(new geometry);

			geometry::vertexies& vertices = pResult->lock_vb();
			geometry::indexies&  indices  = pResult->lock_ib();

			vertices.resize(6);
			indices.resize(8*3);

			vertices[0].position = math::vec3f(0, 0, 0);
			vertices[1].position = math::vec3f(1, 0, 0);
			vertices[2].position = math::vec3f(1, 0, 1);
			vertices[3].position = math::vec3f(0, 0, 1);
			vertices[4].position = math::vec3f(0.5f, -1, 0.5f);
			vertices[5].position = math::vec3f(0.5f, 1, 0.5f);

			setFace(indices, 0, 0, 1, 4);
			setFace(indices, 1, 1, 2, 4);
			setFace(indices, 2, 2, 3, 4);
			setFace(indices, 3, 3, 0, 4);
			setFace(indices, 4, 0, 5, 1);
			setFace(indices, 5, 1, 5, 2);
			setFace(indices, 6, 2, 5, 3);
			setFace(indices, 7, 3, 5, 0);

			pResult->unlock_vb();
			pResult->unlock_ib();
			pResult->getBBox();

			return pResult;
		}

		static geometry_ptr CreateTetra()
		{
			geometry_ptr pResult(new geometry);

			geometry::vertexies& vertices = pResult->lock_vb();
			geometry::indexies&  indices  = pResult->lock_ib();

			vertices.resize(4);
			indices.resize(4*3);

			vertices[0].position = math::vec3f(0, 0, 0);
			vertices[1].position = math::vec3f(1, 1, 0);
			vertices[2].position = math::vec3f(1, 1, 1);
			vertices[3].position = math::vec3f(1, 0, 1);

			setFace(indices, 0, 0, 1, 3);
			setFace(indices, 1, 0, 2, 1);
			setFace(indices, 2, 0, 3, 2);
			setFace(indices, 3, 1, 2, 3);

			pResult->unlock_vb();
			pResult->unlock_ib();
			pResult->getBBox();

			return pResult;
		}


		private:
			static void setFace(typename geometry::indexies& array, unsigned face_number, unsigned i1, unsigned i2, unsigned i3)
			{
				array[face_number*3 + 0] = i1;
				array[face_number*3 + 1] = i2;
				array[face_number*3 + 2] = i3;
			}
	};
}