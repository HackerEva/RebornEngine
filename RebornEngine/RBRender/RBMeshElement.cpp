#include "RBMeshElement.h"
#include "RBDirectXUtility.h"
#include "RBRenderer.h"
using namespace RebornEngine;
RBMeshElement::RBMeshElement()
	: m_Flag(0), m_VertexComponentMask(0)
{
}

void RBMeshElement::Release()
{
	m_RenderBuffer.Release();
}

void RBMeshElement::Serialize(RBSerializer & serializer)
{
	serializer.SerializeData(m_Name);
	serializer.SerializeData(m_Flag);
	serializer.SerializeData(m_VertexComponentMask);
	serializer.SerializeVector(TriangleIndices);
	serializer.SerializeVector(PositionArray);
	serializer.SerializeVector(UV0Array);
	serializer.SerializeVector(NormalArray);
	serializer.SerializeVector(TangentArray);
	serializer.SerializeVector(UV1Array);
	serializer.SerializeVector(BoneIdArray);
	serializer.SerializeVector(BoneWeightArray);

	if (serializer.IsReading())
		UpdateRenderBuffer();

}

void RBMeshElement::SetTriangles(const std::vector<UINT>& triIndices)
{
	TriangleIndices = triIndices;
}

void RBMeshElement::SetVertices(const std::vector<RBVertex::MESH_LOADER_VERTEX>& vertices, int vertexComponentMask)
{
	m_VertexComponentMask = vertexComponentMask;

	TriangleIndices.clear();
	PositionArray.clear();
	UV0Array.clear();
	NormalArray.clear();
	TangentArray.clear();
	UV1Array.clear();
	BoneIdArray.clear();
	BoneWeightArray.clear();

	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (vertexComponentMask & RB_VCM_Pos)
			PositionArray.push_back(vertices[i].pos);
		if (vertexComponentMask & RB_VCM_UV0)
			UV0Array.push_back(vertices[i].uv0);
		if (vertexComponentMask & RB_VCM_Normal)
			NormalArray.push_back(vertices[i].normal);
		if (vertexComponentMask & RB_VCM_Tangent)
			TangentArray.push_back(vertices[i].tangent);
		if (vertexComponentMask & RB_VCM_UV1)
			UV1Array.push_back(vertices[i].uv1);
		if (vertexComponentMask & RB_VCM_BoneId)
			BoneIdArray.push_back(*((VBoneIds*)vertices[i].boneId));
		if (vertexComponentMask & RB_VCM_BoneWeights)
			BoneWeightArray.push_back(vertices[i].weight);
	}
}

void RBMeshElement::UpdateRenderBuffer()
{
	using namespace RebornEngine;
	m_RenderBuffer.Release();

	int stride = RBVertexDeclaration::GetVertexStride(m_VertexComponentMask);
	ID3D11InputLayout* inputLayout = RBVertexDeclaration::Instance().GetInputLayoutByVertexComponents(m_VertexComponentMask);

	BYTE* compactVertexData = new BYTE[PositionArray.size() * stride];

	int offset = 0;

#define COPY_VERTEX_COMPONENT(Mask, VArray, Type) \
	if (m_VertexComponentMask & Mask) \
	{ \
		memcpy(compactVertexData + offset, &VArray[i], sizeof(Type)); \
		offset += sizeof(Type); \
	}

	for (size_t i = 0; i < PositionArray.size(); i++)
	{
		COPY_VERTEX_COMPONENT(RB_VCM_BoneId, BoneIdArray, VBoneIds)
		COPY_VERTEX_COMPONENT(RB_VCM_BoneWeights, BoneWeightArray, RBVector4)
		COPY_VERTEX_COMPONENT(RB_VCM_Pos, PositionArray, RBVector3)
		COPY_VERTEX_COMPONENT(RB_VCM_UV0, UV0Array, RBVector2)
		COPY_VERTEX_COMPONENT(RB_VCM_Normal, NormalArray, RBVector3)
		COPY_VERTEX_COMPONENT(RB_VCM_Tangent, TangentArray, RBVector3)
		COPY_VERTEX_COMPONENT(RB_VCM_UV1, UV1Array, RBVector2)
	}
#undef COPY_VERTEX_COMPONENT

	m_RenderBuffer.CreateVertexBuffer(compactVertexData, stride, (UINT)PositionArray.size(), inputLayout, false, m_Name.c_str());
	m_RenderBuffer.CreateIndexBuffer(TriangleIndices.data(), sizeof(UINT), (UINT)TriangleIndices.size(), false, m_Name.c_str());

	delete[] compactVertexData;

	// Update aabb
	m_Aabb = RBAABB::Default;
	for (size_t i = 0; i < PositionArray.size(); i++)
	{
		m_Aabb.Expand(PositionArray[i]);
	}
}

void RBMeshElement::Draw(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_RenderBuffer.Draw(topology);
}

void RBMeshElement::DrawInstanced(int instanceCount, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_RenderBuffer.DrawInstanced(instanceCount, topology);
}

RBMeshRenderBuffer::RBMeshRenderBuffer()
	: m_VertexBuffer(nullptr), m_IndexBuffer(nullptr), m_InputLayout(nullptr)
{
}

void RBMeshRenderBuffer::Release()
{
	RB_SAFE_RELEASE(m_VertexBuffer);
	RB_SAFE_RELEASE(m_IndexBuffer);

}

void RBMeshRenderBuffer::CreateVertexBuffer(void * data, UINT vertexTypeSize, UINT vertexCount, ID3D11InputLayout * inputLayout, bool dynamic, const char * debugResourceName)
{
	m_InputLayout = inputLayout;

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = vertexTypeSize * vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (dynamic)
	{
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	if (data)
	{
		D3D11_SUBRESOURCE_DATA initVertexData;
		ZeroMemory(&initVertexData, sizeof(initVertexData));
		initVertexData.pSysMem = data;

		RBRENDERER.D3DDevice()->CreateBuffer(&vbd, &initVertexData, &m_VertexBuffer);
	}
	else
	{
		RBRENDERER.D3DDevice()->CreateBuffer(&vbd, NULL, &m_VertexBuffer);
	}

	m_VertexCount = vertexCount;
	m_Stride = vertexTypeSize;

#if _DEBUG
	if (debugResourceName)
		m_VertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugResourceName), debugResourceName);
#endif
}

void RBMeshRenderBuffer::CreateIndexBuffer(void * data, UINT indexTypeSize, UINT indexCount, bool dynamic, const char * debugResourceName)
{
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = indexTypeSize * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initIndexData;
	ZeroMemory(&initIndexData, sizeof(initIndexData));
	initIndexData.pSysMem = data;

	RBRENDERER.D3DDevice()->CreateBuffer(&ibd, &initIndexData, &m_IndexBuffer);
	m_IndexCount = indexCount;

#if _DEBUG
	if (debugResourceName)
		m_IndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugResourceName), debugResourceName);
#endif

}

void RBMeshRenderBuffer::UpdateDynamicVertexBuffer(void * data, UINT vertexTypeSize, UINT vertexCount)
{
	D3D11_MAPPED_SUBRESOURCE subres;
	RBRENDERER.D3DImmediateContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subres);
	memcpy(subres.pData, data, vertexTypeSize * vertexCount);
	RBRENDERER.D3DImmediateContext()->Unmap(m_VertexBuffer, 0);

	m_VertexCount = vertexCount;
}	 
	 
void RBMeshRenderBuffer::Draw(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	UINT offset = 0;

	if (m_IndexBuffer)
	{
		assert(m_InputLayout);
		RBRENDERER.D3DImmediateContext()->IASetInputLayout(m_InputLayout);
		RBRENDERER.D3DImmediateContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_Stride, &offset);
		RBRENDERER.D3DImmediateContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		RBRENDERER.D3DImmediateContext()->IASetPrimitiveTopology(topology);
		RBRENDERER.D3DImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);
	}
	else if (m_VertexBuffer)
	{
		assert(m_InputLayout);
		RBRENDERER.D3DImmediateContext()->IASetInputLayout(m_InputLayout);
		RBRENDERER.D3DImmediateContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_Stride, &offset);
		RBRENDERER.D3DImmediateContext()->IASetPrimitiveTopology(topology);
		RBRENDERER.D3DImmediateContext()->Draw(m_VertexCount, 0);
	}

}	 
	 
void RBMeshRenderBuffer::DrawInstanced(int instanceCount, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	UINT offset = 0;

	if (m_IndexBuffer)
	{
		RBRENDERER.D3DImmediateContext()->IASetInputLayout(m_InputLayout);
		RBRENDERER.D3DImmediateContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_Stride, &offset);
		RBRENDERER.D3DImmediateContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		RBRENDERER.D3DImmediateContext()->IASetPrimitiveTopology(topology);
		RBRENDERER.D3DImmediateContext()->DrawIndexedInstanced(m_IndexCount, instanceCount, 0, 0, 0);
	}
	else if (m_VertexBuffer)
	{
		RBRENDERER.D3DImmediateContext()->IASetInputLayout(m_InputLayout);
		RBRENDERER.D3DImmediateContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_Stride, &offset);
		RBRENDERER.D3DImmediateContext()->IASetPrimitiveTopology(topology);
		RBRENDERER.D3DImmediateContext()->DrawInstanced(m_VertexCount, instanceCount, 0, 0);
	}

}
