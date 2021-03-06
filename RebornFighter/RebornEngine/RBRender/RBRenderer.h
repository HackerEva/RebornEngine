#ifndef RBRENDERER_H
#define RBRENDERER_H
#include "../RBSystem/RBSingleton.h"
#include <Windows.h>
#include "../RBMath/RBMath.h"
#include<d3d11.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

enum RBBlendState
{
	RBBlend_Opaque,
	RBBlend_AlphaBlending,
	RBBlend_AlphaToCoverage,
	RBBlend_Additive,
	
	RBBlendStateCount,
};

enum RBSamplerState
{
	RBSamplerState_Texture,
	RBSamplerState_ShadowDepthComparison,
	RBSamplerStateCount,
};

namespace RebornEngine
{
	struct RBShader;
	class RBRenderer : public RBSingleton<RBRenderer>
	{
		friend class RBSingleton<RBRenderer>;
	public:
		bool Initialize(HWND hWnd, int client_width, int client_height, bool enable4xMsaa, bool enableGammaCorrection = true);
		void Shutdown();

		float AspectRatio() const;
		void ResizeClient(int width, int height);

		void Clear(bool clearColor = true, const RBColor& color = RBColor(0.69f, 0.77f, 0.87f, 1.0f), bool clearDepth = true, float depth = 1.0f, bool clearStencil = true, UINT8 stencil = 0);
		void ClearRenderTarget(ID3D11RenderTargetView* rtv, const RBColor& color = RBColor(0.69f, 0.77f, 0.87f, 1.0f));
		void Present();

		ID3D11Device*			D3DDevice() { return m_pD3DDevice; }
		ID3D11DeviceContext*	D3DImmediateContext() { return m_pD3DImmediateContext; }

		int	GetClientWidth() const { return m_ClientWidth; }
		int	GetClientHeight() const { return m_ClientHeight; }
		const TCHAR* GetAdapterName() const { return m_AdapterName; }

		void SetRenderTargets(UINT numViews = 1, ID3D11RenderTargetView* const* renderTargetView = &DefaultRenderTargetView, ID3D11DepthStencilView* depthStencilView = DefaultDepthStencilView);

		static ID3D11RenderTargetView* DefaultRenderTargetView;
		static ID3D11DepthStencilView* DefaultDepthStencilView;

		void SetBlendState(RBBlendState state);
		void SetSamplerState(int slot, RBSamplerState state);

		bool UsingGammaCorrection() const { return m_UseGammaCorrection; }

		void SetDefferedShading(bool deferred) { m_bIsDeferred = deferred; }
		bool UseDeferredShading() const { return m_bIsDeferred; }

		void SetVertexShader(ID3D11VertexShader* vertexShader);
		void SetPixelShader(ID3D11PixelShader* pixelShader);
		void SetGeometryShader(ID3D11GeometryShader* geometryShader);
	protected:
		RBRenderer();
		~RBRenderer();

		void CreateRenderTargetView();
		void CreateDepthStencilBufferAndView();

		int						m_ClientWidth, m_ClientHeight;
		bool					m_Enable4xMsaa;
		bool					m_UseGammaCorrection;
		UINT					m_4xMsaaQuality;
		TCHAR*					m_AdapterName;

		ID3D11Device*			m_pD3DDevice;
		ID3D11DeviceContext*	m_pD3DImmediateContext;
		IDXGISwapChain*			m_SwapChain;
		ID3D11RenderTargetView*	m_RenderTargetView;
		ID3D11Texture2D*		m_DepthStencilBuffer;
		ID3D11DepthStencilView*	m_DepthStencilView;

		UINT					m_RenderTargetViewNum;
		ID3D11RenderTargetView*	m_CurrentRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		ID3D11DepthStencilView*	m_CurrentDepthStencilView;

		ID3D11BlendState*		m_BlendState[RBBlendStateCount];
		RBBlendState				m_CurrBlendState;

		ID3D11SamplerState*		m_SamplerState[RBSamplerStateCount];

		bool					m_bIsDeferred;
	};
#define RBRENDERER RBRenderer::Instance()

}
#endif // !RBRENDERER_H
