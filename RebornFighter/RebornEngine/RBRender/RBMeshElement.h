#ifndef RBMESHELEMENT_H
#define RBMESHELEMENT_H
#include<d3d11.h>
#include<string>
#include<vector>
#include"../RBMath/RBMath.h"
#include"../RBPhysics/RBPhysics.h"
#include"RBVertex.h"
#include"../RBSystem/RBSerializer.h"

namespace RebornEngine
{
	enum MeshElementFlag
	{
		MEF_Skinned = 1 << 0,
	};

	struct VBoneIds
	{
		int boneId[4];
	};

	class RBMeshRenderBuffer
	{
	protected:
		ID3D11Buffer * m_VertexBuffer;
		ID3D11Buffer*		m_IndexBuffer;
		ID3D11InputLayout*	m_InputLayout;

		UINT				m_Stride;
		UINT				m_VertexCount;
		UINT				m_IndexCount;
	public:
		RBMeshRenderBuffer();

		void Release();

		void CreateVertexBuffer(void* data, UINT vertexTypeSize, UINT vertexCount, ID3D11InputLayout* inputLayout, bool dynamic = false, const char* debugResourceName = nullptr);
		void CreateIndexBuffer(void* data, UINT indexTypeSize, UINT indexCount, bool dynamic = false, const char* debugResourceName = nullptr);

		void UpdateDynamicVertexBuffer(void* data, UINT vertexTypeSize, UINT vertexCount);

		void Draw(D3D11_PRIMITIVE_TOPOLOGY topology);
		void DrawInstanced(int instanceCount, D3D11_PRIMITIVE_TOPOLOGY topology);
	};

	class RBMeshElement
	{
	public:
		RBMeshElement();

		void Release();

		void Serialize(RBSerializer& serializer);
		void SetTriangles(const std::vector<UINT>& triIndices);
		void SetVertices(const std::vector<RBVertex::MESH_LOADER_VERTEX>& vertices, int vertexComponentMask);
		void SetVertexComponentMask(int mask) { m_VertexComponentMask = mask; }
		int GetVertexComponentMask() const { return m_VertexComponentMask; }
		void UpdateRenderBuffer();
		void Draw(D3D11_PRIMITIVE_TOPOLOGY topology);
		void DrawInstanced(int instanceCount, D3D11_PRIMITIVE_TOPOLOGY topology);
		void SetName(const char* name) { m_Name = name; }
		std::string GetName() const { return m_Name; }

		const RBAABB& GetAabb() const { return m_Aabb; }

		void SetFlag(int flag) { m_Flag = flag; }
		int GetFlag() const { return m_Flag; }

		std::vector<UINT>			TriangleIndices;
		std::vector<RBVector3>		PositionArray;
		std::vector<RBVector2>		UV0Array;
		std::vector<RBVector3>		NormalArray;
		std::vector<RBVector3>		TangentArray;
		std::vector<RBVector2>		UV1Array;
		std::vector<VBoneIds>		BoneIdArray;
		std::vector<RBVector4>		BoneWeightArray;

	private:
		std::string					m_Name;
		RBAABB						m_Aabb;
		UINT						m_Flag;

		RBMeshRenderBuffer			m_RenderBuffer;

		int							m_VertexComponentMask;
	};
}


#endif
