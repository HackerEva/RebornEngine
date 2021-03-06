#include "RBShadowMap.h"
#include "RBDirectXUtility.h"
#include "RBRenderer.h"
RebornEngine::RBShadowMap::RBShadowMap()
	: m_RenderTargetBuffer(nullptr), m_RenderTargetView(nullptr), m_RenderTargetSRV(nullptr),
	m_DepthBuffer(nullptr), m_DepthView(nullptr), m_bNeedUpdateFrustum(true)

{
	m_ViewMatrix = RBMatrix4x4::IDENTITY;
	m_ProjMatrix = RBMatrix4x4::IDENTITY;

}

RebornEngine::RBShadowMap::~RBShadowMap()
{
	RB_SAFE_RELEASE(m_RenderTargetDepthSRV);
	RB_SAFE_RELEASE(m_DepthBuffer);
	RB_SAFE_RELEASE(m_DepthView);
	RB_SAFE_RELEASE(m_RenderTargetSRV);
	RB_SAFE_RELEASE(m_RenderTargetView);
	RB_SAFE_RELEASE(m_RenderTargetBuffer);

}

void RebornEngine::RBShadowMap::Initialize(int width, int height)
{
	m_BufferWidth = width;
	m_BufferHeight = height;

	D3D11_TEXTURE2D_DESC renderTextureDesc;
	renderTextureDesc.Width = width;
	renderTextureDesc.Height = height;
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.ArraySize = 1;
	renderTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// No MSAA on depth buffer
	renderTextureDesc.SampleDesc.Count = 1;
	renderTextureDesc.SampleDesc.Quality = 0;

	renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTextureDesc.CPUAccessFlags = 0;
	renderTextureDesc.MiscFlags = 0;

	RBRENDERER.D3DDevice()->CreateTexture2D(&renderTextureDesc, 0, &m_RenderTargetBuffer);
	RBRENDERER.D3DDevice()->CreateRenderTargetView(m_RenderTargetBuffer, NULL, &m_RenderTargetView);

	// Create depth buffer for render target
	renderTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	renderTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	RBRENDERER.D3DDevice()->CreateTexture2D(&renderTextureDesc, 0, &m_DepthBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	RBRENDERER.D3DDevice()->CreateDepthStencilView(m_DepthBuffer, &depthStencilViewDesc, &m_DepthView);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	RBRENDERER.D3DDevice()->CreateShaderResourceView(m_DepthBuffer, &shaderResourceViewDesc, &m_RenderTargetDepthSRV);

	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	RBRENDERER.D3DDevice()->CreateShaderResourceView(m_RenderTargetBuffer, &shaderResourceViewDesc, &m_RenderTargetSRV);

}

void RebornEngine::RBShadowMap::SetOrthogonalProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
{
	m_ViewWidth = viewWidth;
	m_ViewHeight = viewHeight;
	m_ViewNear = nearZ;
	m_ViewFar = farZ;

	m_ProjMatrix = RBMatrix4x4::CreateOrthographicProjectionLH(viewWidth, viewHeight, nearZ, farZ);
	m_bNeedUpdateFrustum = true;

}

void RebornEngine::RBShadowMap::SetupRenderTarget()
{
	ID3D11RenderTargetView* shadowRenderTargetView = GetRenderTargetView();
	RBRENDERER.SetRenderTargets(1, &shadowRenderTargetView, GetDepthView());

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(m_BufferWidth);
	vp.Height = static_cast<float>(m_BufferHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	RBRENDERER.D3DImmediateContext()->RSSetViewports(1, &vp);

}

RBFrustum RebornEngine::RBShadowMap::GetFrustum()
{
	if (m_bNeedUpdateFrustum)
	{
		RBMatrix4x4 mat = m_ViewMatrix.FastInverse();
		RBVector3 eyePos = mat.GetTranslation();
		RBVector3 viewForward = mat.GetForward();
		RBVector3 viewRight = mat.GetRight();
		RBVector3 viewUp = mat.GetUp();
		RBVector3 nearPoint = eyePos + viewForward * m_ViewNear;
		RBVector3 farPoint = eyePos + viewForward * m_ViewFar;

		m_Frustum.corners[0] = farPoint - viewRight * m_ViewWidth * 0.5f + viewUp * m_ViewHeight * 0.5f;	// RBVector3(-1,  1,  1);
		m_Frustum.corners[1] = farPoint + viewRight * m_ViewWidth * 0.5f + viewUp * m_ViewHeight * 0.5f;	// RBVector3( 1,  1,  1);
		m_Frustum.corners[2] = farPoint - viewRight * m_ViewWidth * 0.5f - viewUp * m_ViewHeight * 0.5f;	// RBVector3(-1, -1,  1);
		m_Frustum.corners[3] = farPoint + viewRight * m_ViewWidth * 0.5f - viewUp * m_ViewHeight * 0.5f;	// RBVector3( 1, -1,  1);
		m_Frustum.corners[4] = nearPoint - viewRight * m_ViewWidth * 0.5f + viewUp * m_ViewHeight * 0.5f;	// RBVector3(-1,  1,  0);
		m_Frustum.corners[5] = nearPoint + viewRight * m_ViewWidth * 0.5f + viewUp * m_ViewHeight * 0.5f;	// RBVector3( 1,  1,  0);
		m_Frustum.corners[6] = nearPoint - viewRight * m_ViewWidth * 0.5f - viewUp * m_ViewHeight * 0.5f;	// RBVector3(-1, -1,  0);
		m_Frustum.corners[7] = nearPoint + viewRight * m_ViewWidth * 0.5f - viewUp * m_ViewHeight * 0.5f;	// RBVector3( 1, -1,  0);
		m_Frustum.BuildPlanesFromCorners();

		m_bNeedUpdateFrustum = false;
	}

	return m_Frustum;

}
