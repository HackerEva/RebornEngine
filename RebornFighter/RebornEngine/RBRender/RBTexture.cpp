#include "RBTexture.h"
#include "RBDirectXUtility.h"
namespace RebornEngine
{
	RBTexture::RBTexture(string path)
		: RBIResource(RB_RT_Texture, path), m_SRV(nullptr), m_Width(0), m_Height(0)
	{
		;
	}

	RBTexture::RBTexture(string path, ID3D11ShaderResourceView * srv)
		: RBIResource(RB_RT_Texture, path), m_SRV(srv), m_Width(0), m_Height(0)
	{
	}

	RBTexture::~RBTexture()
	{
		RB_SAFE_RELEASE(m_SRV);
	}
}

