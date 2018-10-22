#include "RBDebugRenderer.h"
#include "RBVertex.h"
using namespace RebornEngine;
RBDebugRenderer::RBDebugRenderer()
	: m_PrimitiveInputLayout(nullptr), m_ColorShader(nullptr), m_PrimitiveColor(0.0f, 1.0f, 0.0f), m_bDirtyBuffer(true)

{
}

RBDebugRenderer::~RBDebugRenderer()
{
}

void RBDebugRenderer::Initialize(int maxVertexCount)
{
	m_ColorShader = RBShaderManager::Instance().GetShaderResource("Color");
	m_PrimitiveInputLayout = RBVertexDeclaration::Instance().GetInputLayout(RBVertex::PRIMITIVE_VERTEX::GetTypeName());
	m_PrimitiveMeshBuffer.CreateVertexBuffer(nullptr, sizeof(RBVertex::PRIMITIVE_VERTEX), maxVertexCount, m_PrimitiveInputLayout, true);

}

void RBDebugRenderer::Release()
{
	m_PrimitiveMeshBuffer.Release();
}

void RBDebugRenderer::SetColor(const RBColor & color)
{
	m_PrimitiveColor = color;
}

void RBDebugRenderer::DrawLine(const RBVector3 & start, const RBVector3 & end)
{
	DrawLine(start, end, m_PrimitiveColor, m_PrimitiveColor);
}

void RBDebugRenderer::DrawLine(const RBVector3 & start, const RBVector3 & end, const RBColor & color1, const RBColor & color2)
{
	m_PrimitiveVertices.push_back({ RBVector4(start), color1 });
	m_PrimitiveVertices.push_back({ RBVector4(end), color2 });

	m_bDirtyBuffer = true;
}

void RBDebugRenderer::DrawAABB(const RBAABB & aabb)
{
	DrawAABB(aabb, m_PrimitiveColor);
}

void RBDebugRenderer::DrawAABB(const RBAABB & aabb, const RBColor & color)
{
	RBVector3 cornerPoints[] =
	{
		RBVector3(aabb.pMin.x, aabb.pMin.y, aabb.pMin.z),
		RBVector3(aabb.pMin.x, aabb.pMin.y, aabb.pMax.z),
		RBVector3(aabb.pMin.x, aabb.pMax.y, aabb.pMax.z),
		RBVector3(aabb.pMin.x, aabb.pMax.y, aabb.pMin.z),
		
		RBVector3(aabb.pMax.x, aabb.pMin.y, aabb.pMin.z),
		RBVector3(aabb.pMax.x, aabb.pMin.y, aabb.pMax.z),
		RBVector3(aabb.pMax.x, aabb.pMax.y, aabb.pMax.z),
		RBVector3(aabb.pMax.x, aabb.pMax.y, aabb.pMin.z),
	};

	int wiredCubeIdx[] =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7,
	};

	for (int i = 0; i < 24; i++)
	{
		RBVertex::PRIMITIVE_VERTEX v =
		{
			RBVector4(cornerPoints[wiredCubeIdx[i]]),
			RBColor(0.0f, 1.0f, 0.0f),
		};
		m_PrimitiveVertices.push_back(v);
	}
}

void RBDebugRenderer::DrawFrustum(const RBFrustum & frustum)
{
	DrawFrustum(frustum, m_PrimitiveColor);
}

void RBDebugRenderer::DrawFrustum(const RBFrustum & frustum, const RBColor & color)
{
	int wiredCubeIdx[] =
	{
		0, 1, 1, 3, 3, 2, 2, 0,
		4, 5, 5, 7, 7, 6, 6, 4,
		0, 4, 1, 5, 2, 6, 3, 7,
	};

	for (int i = 0; i < 24; i++)
	{
		RBVertex::PRIMITIVE_VERTEX v =
		{
			RBVector4(frustum.corners[wiredCubeIdx[i]]),
			color,
		};
		m_PrimitiveVertices.push_back(v);
	}

}

void RBDebugRenderer::DrawSphere(const RBVector3 & center, float radius, int segment)
{
	DrawSphere(center, radius, m_PrimitiveColor, segment);
}

void RBDebugRenderer::DrawSphere(const RBVector3 & center, float radius, const RBColor & color, int segment)
{
	for (int i = 0; i < segment; i++)
	{
		for (int j = 1; j < segment; j++)
		{
			float y = cosf(PI * j / segment) * radius + center.y;
			float r = sinf(PI * j / segment) * radius;

			float x0 = sinf(2.0f * PI * i / segment) * r + center.x;
			float z0 = cosf(2.0f * PI * i / segment) * r + center.z;

			int i1 = (i + 1) % segment;
			float x1 = sinf(2.0f * PI * i1 / segment) * r + center.x;
			float z1 = cosf(2.0f * PI * i1 / segment) * r + center.z;

			RBVertex::PRIMITIVE_VERTEX v0 = { RBVector4(x0, y, z0), color };
			RBVertex::PRIMITIVE_VERTEX v1 = { RBVector4(x1, y, z1), color };
			m_PrimitiveVertices.push_back(v0);
			m_PrimitiveVertices.push_back(v1);
		}
	}

	for (int i = 0; i < segment; i++)
	{
		for (int j = 0; j < segment; j++)
		{
			float y0 = cosf(PI * j / segment) * radius + center.y;
			float r0 = sinf(PI * j / segment) * radius;

			int j1 = j + 1;
			float y1 = cosf(PI * j1 / segment) * radius + center.y;
			float r1 = sinf(PI * j1 / segment) * radius;

			float x0 = sinf(2.0f * PI * i / segment) * r0 + center.x;
			float z0 = cosf(2.0f * PI * i / segment) * r0 + center.z;

			float x1 = sinf(2.0f * PI * i / segment) * r1 + center.x;
			float z1 = cosf(2.0f * PI * i / segment) * r1 + center.z;

			RBVertex::PRIMITIVE_VERTEX v0 = { RBVector4(x0, y0, z0), color };
			RBVertex::PRIMITIVE_VERTEX v1 = { RBVector4(x1, y1, z1), color };
			m_PrimitiveVertices.push_back(v0);
			m_PrimitiveVertices.push_back(v1);
		}
	}

}

void RBDebugRenderer::Render()
{
	if (m_bDirtyBuffer)
	{
		m_PrimitiveMeshBuffer.UpdateDynamicVertexBuffer(m_PrimitiveVertices.data(), sizeof(RBVertex::PRIMITIVE_VERTEX), (UINT)m_PrimitiveVertices.size());
		m_bDirtyBuffer = false;
	}

	//RRenderer.Clear(false);

	m_ColorShader->Bind();
	RBRENDERER.D3DImmediateContext()->IASetInputLayout(m_PrimitiveInputLayout);
	m_PrimitiveMeshBuffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}

void RBDebugRenderer::Reset()
{
	m_PrimitiveVertices.clear();
	m_bDirtyBuffer = true;

}
