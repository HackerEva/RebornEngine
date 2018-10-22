#ifndef RBSHADERCONSTANTBUFFER_H
#define RBSHADERCONSTANTBUFFER_H
#include<d3d11.h>
#include"../RBRender/RBRenderer.h"
#include"../RBRender/RBDirectXUtility.h"
namespace RebornEngine
{
	enum RBConstantBufferShaderType
	{
		RB_CBST_VS = 1 << 0,
		RB_CBST_PS = 1 << 1,
		RB_CBST_GS = 1 << 2,
	};

	template <typename T, int SHADER_TYPE, int SLOT>
	class RBShaderConstantBuffer
	{
	public:
		RBShaderConstantBuffer()
			: m_ConstBuffer(nullptr)
		{}

		void Initialize()
		{
			D3D11_BUFFER_DESC cbDesc;
			ZeroMemory(&cbDesc, sizeof(cbDesc));
			cbDesc.ByteWidth = sizeof(T);
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;

			RBRENDERER.D3DDevice()->CreateBuffer(&cbDesc, NULL, &m_ConstBuffer);

			// Initialize buffer values with zero
			T buffer;
			ZeroMemory(&buffer, sizeof(T));
			UpdateContent(&buffer);
			ApplyToShaders();
		}

		void Release()
		{
			RB_SAFE_RELEASE(m_ConstBuffer);
		}

		void UpdateContent(const T* data)
		{
			D3D11_MAPPED_SUBRESOURCE subres;
			RBRENDERER.D3DImmediateContext()->Map(m_ConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subres);
			memcpy(subres.pData, data, sizeof(T));
			RBRENDERER.D3DImmediateContext()->Unmap(m_ConstBuffer, 0);
		}

		// Send constant buffer data to shaders
		void ApplyToShaders()
		{
			if (SHADER_TYPE & RB_CBST_VS)
				RBRENDERER.D3DImmediateContext()->VSSetConstantBuffers(SLOT, 1, &m_ConstBuffer);
			if (SHADER_TYPE & RB_CBST_PS)
				RBRENDERER.D3DImmediateContext()->PSSetConstantBuffers(SLOT, 1, &m_ConstBuffer);
			if (SHADER_TYPE & RB_CBST_GS)
				RBRENDERER.D3DImmediateContext()->GSSetConstantBuffers(SLOT, 1, &m_ConstBuffer);
		}

	private:
		ID3D11Buffer * m_ConstBuffer;
	};

}

#endif
