#ifndef RBSKYBOX_H
#define RBSKYBOX_H
#include "../RBShader/RBShader.h"
#include "../RBRender/RBTexture.h"
#endif // RBSKYBOX_H
namespace RebornEngine
{
	class RBSkybox
	{
	public:
		RBSkybox();
		~RBSkybox();
		void CreateSkybox(const char* skyTextureName);
		void CreateSkybox(RBTexture* skyTexture);
		void Release();

		void Draw();

	private:
		RBMeshRenderBuffer			m_SkyboxMesh;
		RBTexture*					m_SkyboxTexture;
		ID3D11InputLayout*			m_SkyboxIL;
		RBShader*					m_SkyboxShader;

	};
}
