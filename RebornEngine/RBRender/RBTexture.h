#ifndef RBTEXTURE_H
#define RBTEXTURE_H
#include "RBIResource.h"
#include<string>
#include<d3d11.h>
#include"RBResourceManager.h"

namespace RebornEngine
{
	class RBTexture : public RBIResource
	{	
		friend class RBResourceManager;
	private:
			
			UINT						m_Width, m_Height;
			ID3D11ShaderResourceView*	m_SRV;
	public:

		RBTexture(std::string path);
		RBTexture(std::string path, ID3D11ShaderResourceView* srv);
		~RBTexture();

		ID3D11ShaderResourceView* GetSRV() { return m_SRV; }
		ID3D11ShaderResourceView** GetPtrSRV() { return &m_SRV; }
		UINT GetWidth() const { return m_Width; }
		UINT GetHeight() const { return m_Height; }


	};
}


#endif
