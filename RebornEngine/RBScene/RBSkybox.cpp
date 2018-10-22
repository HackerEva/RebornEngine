#include "RBSkybox.h"
using namespace RebornEngine;
RBSkybox::RBSkybox()
	: m_SkyboxTexture(nullptr), m_SkyboxIL(nullptr)
{
}

RBSkybox::~RBSkybox()
{
}

void RBSkybox::CreateSkybox(const char * skyTextureName)
{
	CreateSkybox(RBResourceManager::Instance().FindTexture(skyTextureName));
}

void RBSkybox::CreateSkybox(RBTexture * skyTexture)
{
	m_SkyboxTexture = skyTexture;

	// Create skybox buffer
	RBVertex::SKYBOX_VERTEX skyboxVertex[] =
	{
		{ RBVector3(-0.5f, -0.5f, -0.5f) },
		{ RBVector3(-0.5f,  0.5f, -0.5f) },
		{ RBVector3(0.5f,  0.5f, -0.5f) },
		{ RBVector3(0.5f, -0.5f, -0.5f) },
		  
		{ RBVector3(-0.5f, -0.5f,  0.5f) },
		{ RBVector3(-0.5f,  0.5f,  0.5f) },
		{ RBVector3(0.5f,  0.5f,  0.5f) },
		{ RBVector3(0.5f, -0.5f,  0.5f) },
	};

	UINT32 skyboxIndex[] =
	{
		0, 2, 1, 0, 3, 2,
		0, 1, 5, 0, 5, 4,
		3, 6, 2, 3, 7, 6,
		4, 5, 6, 4, 6, 7,
		1, 2, 6, 1, 6, 5,
		0, 4, 7, 0, 7, 3,
	};

	m_SkyboxIL = RBVertexDeclaration::Instance().GetInputLayout(RBVertex::SKYBOX_VERTEX::GetTypeName());
	m_SkyboxMesh.CreateVertexBuffer(skyboxVertex, sizeof(RBVertex::SKYBOX_VERTEX), sizeof(skyboxVertex) / sizeof(RBVertex::SKYBOX_VERTEX), m_SkyboxIL);
	m_SkyboxMesh.CreateIndexBuffer(skyboxIndex, sizeof(UINT32), sizeof(skyboxIndex) / sizeof(UINT32));

	m_SkyboxShader = RBShaderManager::Instance().GetShaderResource("Skybox");

	if (!m_SkyboxShader)
	{
		OutputDebugStringA("*** WARNING: Unable to find shader \'Skybox\', skybox will not be rendered properly.\n");
	}
}

void RBSkybox::Release()
{
	m_SkyboxMesh.Release();
}

void RBSkybox::Draw()
{
	if (m_SkyboxShader && m_SkyboxTexture)
	{
		RBRENDERER.D3DImmediateContext()->PSSetShaderResources(0, 1, m_SkyboxTexture->GetPtrSRV());
		m_SkyboxShader->Bind();
		m_SkyboxMesh.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}
