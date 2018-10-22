#ifndef RBDEBUGRENDERER_H
#define RBDEBUGRENDERER_H
#include"../RBMath/RBMath.h"
#include"../RBPhysics/RBPhysics.h"
#include "RBRenderer.h"
#include"RBVertex.h"
#include<vector>
#include"RBMeshElement.h"
#include "RBShaderManager.h"
struct RBShader;
namespace RebornEngine
{
	
	class RBDebugRenderer
	{
	public:
		RBDebugRenderer();
		~RBDebugRenderer();

		void Initialize(int maxVertexCount = 65536);
		void Release();

		void SetColor(const RBColor& color);
		void DrawLine(const RBVector3& start, const RBVector3& end);
		void DrawLine(const RBVector3& start, const RBVector3& end, const RBColor& color1, const RBColor& color2);
		void DrawAABB(const RBAABB& aabb);
		void DrawAABB(const RBAABB& aabb, const RBColor& color);
		void DrawFrustum(const RBFrustum& frustum);
		void DrawFrustum(const RBFrustum& frustum, const RBColor& color);
		void DrawSphere(const RBVector3& center, float radius, int segment = 12);
		void DrawSphere(const RBVector3& center, float radius, const RBColor& color, int segment = 12);

		// Present primitive to the screen
		void Render();
		void Reset();
	private:
		RBMeshRenderBuffer		m_PrimitiveMeshBuffer;
		ID3D11InputLayout*		m_PrimitiveInputLayout;
		RBShader*				m_ColorShader;

		RBColor					m_PrimitiveColor;
		std::vector<RBVertex::PRIMITIVE_VERTEX>
			m_PrimitiveVertices;
		bool					m_bDirtyBuffer;

	};

}
#endif