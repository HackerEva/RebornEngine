#ifndef RBSHADOWMAP_H
#define RBSHADOWMAP_H
#include "../RBMath/RBMath.h"
#include <d3d11.h>
#include "../RBPhysics/RBPhysics.h"
namespace RebornEngine
{
	class RBShadowMap
	{
	public:
		RBShadowMap();
		~RBShadowMap();

		void Initialize(int width, int height);
		void SetViewMatrix(const RBMatrix4x4& view) { m_ViewMatrix = view; m_bNeedUpdateFrustum = true; }
		void SetOrthogonalProjection(float viewWidth, float viewHeight, float nearZ, float farZ);
		void SetupRenderTarget();

		RBMatrix4x4 GetViewMatrix() const { return m_ViewMatrix; }
		RBMatrix4x4 GetProjectionMatrix() const { return m_ProjMatrix; }
		RBFrustum GetFrustum();

		ID3D11RenderTargetView* GetRenderTargetView() { return m_RenderTargetView; }
		ID3D11DepthStencilView* GetDepthView() { return m_DepthView; }
		ID3D11ShaderResourceView* GetRenderTargetSRV() { return m_RenderTargetSRV; }
		ID3D11ShaderResourceView* GetRenderTargetDepthSRV() { return m_RenderTargetDepthSRV; }

		// Texture slot number which matches register of ShadowDepthTexture in LightShaderCommon.hlsli
		static int ShaderResourceSlot() { return 5; }

	private:
		ID3D11Texture2D * m_RenderTargetBuffer;
		ID3D11Texture2D*			m_DepthBuffer;
		ID3D11RenderTargetView*		m_RenderTargetView;
		ID3D11DepthStencilView*		m_DepthView;
		ID3D11ShaderResourceView*	m_RenderTargetSRV;
		ID3D11ShaderResourceView*	m_RenderTargetDepthSRV;

		RBMatrix4x4					m_ViewMatrix;
		RBMatrix4x4					m_ProjMatrix;

		int							m_BufferWidth, m_BufferHeight;
		float						m_ViewWidth, m_ViewHeight;
		float						m_ViewNear, m_ViewFar;
		bool						m_bNeedUpdateFrustum;
		RBFrustum					m_Frustum;
	};
}
#endif
