#include "RBVertex.h"
#include "RBRenderer.h"
#include <utility>
#include "../RBMeshVertexSignature.csh"
#include "../RBPrimitiveVertexSignature.csh"
#include "../RBSkyboxVertexSignature.csh"
#include "../RBParticleVertexSignature.csh"
#include "../RBFontVertexSignature.csh"
#include <assert.h>
#include <Winerror.h>
#include <iostream>
#include <cstddef>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

namespace RebornEngine
{
	RBShaderInputVertex VertexSemantics[] =
	{
	{ "int4",	"BLENDINDICES", },
	{ "float4", "BLENDWEIGHT", },
	{ "float3", "POSITION", },
	{ "float2", "TEXCOORD0", },
	{ "float3", "NORMAL", },
	{ "float3", "TANGENT", },
	{ "float2", "TEXCOORD1", },
	};

	RBVertexDeclaration::RBVertexDeclaration()
	{
	}
	RBVertexDeclaration::~RBVertexDeclaration()
	{
	}
	void RBVertexDeclaration::Initialize()
	{
		ID3D11InputLayout* pInputLayout;

		// Mesh vertex
		D3D11_INPUT_ELEMENT_DESC meshVertDesc[] =
		{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		RBRENDERER.D3DDevice()->CreateInputLayout(meshVertDesc, 5, RBMeshVertexSignature, sizeof(RBMeshVertexSignature), &pInputLayout);
		m_InputLayouts.insert(std::make_pair(RBVertex::MESH_VERTEX::GetTypeName(), pInputLayout));


		// Color primitive vertex
		D3D11_INPUT_ELEMENT_DESC primitiveVertDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		RBRENDERER.D3DDevice()->CreateInputLayout(primitiveVertDesc, 2, RBPrimitiveVertexSignature, sizeof(RBPrimitiveVertexSignature), &pInputLayout);
		m_InputLayouts.insert(std::make_pair(RBVertex::PRIMITIVE_VERTEX::GetTypeName(), pInputLayout));


		// Skybox vertex
		D3D11_INPUT_ELEMENT_DESC skyboxVertDesc[] =
		{
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		RBRENDERER.D3DDevice()->CreateInputLayout(skyboxVertDesc, 1, RBSkyboxVertexSignature, sizeof(RBSkyboxVertexSignature), &pInputLayout);
		m_InputLayouts.insert(std::make_pair(RBVertex::SKYBOX_VERTEX::GetTypeName(), pInputLayout));


		// Particle vertex
		D3D11_INPUT_ELEMENT_DESC particleVertDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	1, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		RBRENDERER.D3DDevice()->CreateInputLayout(particleVertDesc, 4, RBParticleVertexSignature, sizeof(RBParticleVertexSignature), &pInputLayout);
		m_InputLayouts.insert(std::make_pair(RBVertex::PARTICLE_VERTEX::GetTypeName(), pInputLayout));

		D3D11_INPUT_ELEMENT_DESC fontVertDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		1, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		RBRENDERER.D3DDevice()->CreateInputLayout(fontVertDesc, 4, RBFontVertexSignature, sizeof(RBFontVertexSignature), &pInputLayout);
		m_InputLayouts.insert(std::make_pair(RBVertex::FONT_VERTEX::GetTypeName(), pInputLayout));

	}
	void RBVertexDeclaration::Release()
	{
		for (std::map<std::string, ID3D11InputLayout*>::iterator iter = m_InputLayouts.begin();
			iter != m_InputLayouts.end(); iter++)
		{
			iter->second->Release();
		}

		for (std::map<int, ID3D11InputLayout*>::iterator iter = m_VertexComponentInputLayouts.begin();
			iter != m_VertexComponentInputLayouts.end(); iter++)
		{
			iter->second->Release();
		}

		m_InputLayouts.clear();

	}
	ID3D11InputLayout * RBVertexDeclaration::GetInputLayout(const std::string & vertexTypeName) const
	{
		std::map<std::string, ID3D11InputLayout*>::const_iterator iter = m_InputLayouts.find(vertexTypeName);
		if (iter != m_InputLayouts.end())
		{
			return iter->second;
		}

		return nullptr;
	}
	ID3D11InputLayout * RBVertexDeclaration::GetInputLayoutByVertexComponents(int vertexComponents)
	{
		std::string msg_buf = "Input layout: ";
		msg_buf += GetVertexComponentsString(vertexComponents);
		msg_buf += "\n";
		OutputDebugStringA(msg_buf.data());

		std::map<int, ID3D11InputLayout*>::const_iterator iter = m_VertexComponentInputLayouts.find(vertexComponents);
		if (iter == m_VertexComponentInputLayouts.end())
		{
			// Create input layout based on vertex component
			static D3D11_INPUT_ELEMENT_DESC ComponentDescs[VertexComponent_Count] =
			{
			{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_SINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		1, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			ID3D11InputLayout* pInputLayout;
			int componentCount = 0;
			D3D11_INPUT_ELEMENT_DESC desc[VertexComponent_Count];
			for (int i = 0; i < VertexComponent_Count; i++)
			{
				if (vertexComponents & (1 << i))
				{
					desc[componentCount] = ComponentDescs[i];
					componentCount++;
				}
			}

			// Create shader
			std::string vertexShaderSignature = "struct INPUT_VERTEX {\n";

			for (int i = 0; i < VertexComponent_Count; i++)
			{
				if (vertexComponents & (1 << i))
				{
					std::string variableName = VertexSemantics[i].Semantic;
					for (UINT n = 1; n < variableName.length(); n++)
					{
						variableName[n] = tolower(variableName[n]);
					}

					vertexShaderSignature += std::string(VertexSemantics[i].Type) + " " + variableName + " : " + VertexSemantics[i].Semantic + ";\n";
				}
			}

			vertexShaderSignature +=
				"};\n"
				"float4 main(INPUT_VERTEX Input) : SV_POSITION { return float4(0, 0, 0, 0); }\n";

			ID3DBlob* pShaderCode = nullptr;
			ID3DBlob* pErrorMsg = nullptr;

			char filename[1024];
			sprintf_s(filename, 1024, "VS_Signature_%d.hlsl", vertexComponents);

			if (SUCCEEDED(D3DCompile(vertexShaderSignature.data(), vertexShaderSignature.size(), filename, NULL, NULL, "main", "vs_4_0", 0, 0, &pShaderCode, &pErrorMsg)))
			{
				RBRENDERER.D3DDevice()->CreateInputLayout(desc, componentCount, pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), &pInputLayout);
			}
			else
			{
				OutputDebugStringA((char*)pErrorMsg->GetBufferPointer());
				OutputDebugStringA("\n");
				assert(0);
			}

			m_VertexComponentInputLayouts[vertexComponents] = pInputLayout;

			return pInputLayout;
		}
		else
		{
			return iter->second;
		}
	}
	std::string RBVertexDeclaration::GetVertexComponentsString(int vertexComponents)
	{
		std::string str;

		for (int i = 0; i < VertexComponent_Count; i++)
		{
			if (vertexComponents & (1 << i))
			{
				str += std::string(VertexSemantics[i].Semantic) + " ";
			}
		}

		return str;

	}
	int RBVertexDeclaration::GetVertexStride(int vertexComponents)
	{
		int stride = 0;

		if (vertexComponents & RB_VCM_Pos)
			stride += sizeof(RBVector3);
		if (vertexComponents & RB_VCM_UV0)
			stride += sizeof(RBVector2);
		if (vertexComponents & RB_VCM_UV1)
			stride += sizeof(RBVector2);
		if (vertexComponents & RB_VCM_Normal)
			stride += sizeof(RBVector3);
		if (vertexComponents & RB_VCM_Tangent)
			stride += sizeof(RBVector3);
		if (vertexComponents & RB_VCM_BoneId)
			stride += sizeof(int) * 4;
		if (vertexComponents & RB_VCM_BoneWeights)
			stride += sizeof(float) * 4;

		return stride;
	}
	void RBVertexDeclaration::CopyVertexComponents(void * out, const RBVertex::MESH_LOADER_VERTEX * in, int count, int vertexComponents)
	{
		struct BR_VC_Info
		{
			int size;
			int offset;
		};

		static BR_VC_Info strides[VertexComponent_Count] =
		{
		{ GetVertexStride(RB_VCM_BoneId),		offsetof(RBVertex::MESH_LOADER_VERTEX, boneId) },
		{ GetVertexStride(RB_VCM_BoneWeights),	offsetof(RBVertex::MESH_LOADER_VERTEX, weight) },
		{ GetVertexStride(RB_VCM_Pos),			offsetof(RBVertex::MESH_LOADER_VERTEX, pos) },
		{ GetVertexStride(RB_VCM_UV0),			offsetof(RBVertex::MESH_LOADER_VERTEX, uv0) },
		{ GetVertexStride(RB_VCM_Normal),		offsetof(RBVertex::MESH_LOADER_VERTEX, normal) },
		{ GetVertexStride(RB_VCM_Tangent),		offsetof(RBVertex::MESH_LOADER_VERTEX, tangent) },
		{ GetVertexStride(RB_VCM_UV1),			offsetof(RBVertex::MESH_LOADER_VERTEX, uv1) },
		};

		int offset = 0;
		for (int i = 0; i < count; i++)
		{
			for (int n = 0; n < VertexComponent_Count; n++)
			{
				if ((1 << n) & vertexComponents)
				{
					memcpy((BYTE*)out + offset, (BYTE*)&in[i] + strides[n].offset, strides[n].size);
					offset += strides[n].size;
				}
			}
		}
	}
}

