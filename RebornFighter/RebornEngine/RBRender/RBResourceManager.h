#ifndef RBRESOURCEMANAGER_H
#define RBRESOURCEMANAGER_H
#include <d3d11.h>
#include <string>
#include <map>
#include <mutex>
#include <queue>

#include "RBMesh.h"
#include "RBIResource.h"
#include "../RBSystem/RBSingleton.h"
#include "RBMaterial.h"
#include "RBResourceManager.h"

namespace RebornEngine
{
	struct RBMaterial;
	class RBMesh;
	class RBTexture;
	enum RBResourceLoadingMode
	{
		RB_RLM_Immediate,
		RB_RLM_Threaded,
	};
	struct RBLoaderThreadTask
	{
		std::string			Filename;
		RBIResource*		Resource;
	};
	struct RBLoaderThreadData
	{
		std::mutex*						TaskQueueMutex;
		std::condition_variable*		TaskQueueCondition;
		std::queue<RBLoaderThreadTask>*	TaskQueue;
		bool*							ShouldQuitThread;
	};
	//
	class RBResourceManager : public RBSingleton<RBResourceManager>
	{
		//		
	public:
		friend void ResourceLoaderThread(RBLoaderThreadData* data);
		friend class RBSingleton<RBResourceManager>;

		//
		void Initialize();
		void Destroy();

		void LoadAllResources();
		void UnloadAllResources();
		void UnloadShaderResourceVeiwWrappers();
		RBMesh* LoadFbxMesh(const char* filename, RBResourceLoadingMode mode = RB_RLM_Threaded);
		RBTexture* LoadDDSTexture(const char* filename, RBResourceLoadingMode mode = RB_RLM_Threaded);

		RBTexture* FindTexture(const char* resourcePath);
		RBMesh* FindMesh(const char* resourcePath);
		const std::vector<RBMesh*>& GetMeshResources() const;

		static std::string GetAssetsBasePath();
		static std::string GetResourcePath(const string& path);

		// Wrap a d3d11 Shader Resource View and get a pointer to texture
		RBTexture* WrapShaderResourceVeiw(ID3D11ShaderResourceView* srv);

	private:
		RBResourceManager() {}
		~RBResourceManager() {}

		static void ThreadLoadFbxMeshData(RBLoaderThreadTask* task);
		static bool ThreadLoadMeshData(RBLoaderThreadTask* task);
		static void LoadMeshMaterials(const std::string& mtlFilename, std::vector<RBMaterial>& materials);

		static void ThreadLoadDDSTextureData(RBLoaderThreadTask* task);

		std::vector<RBMesh*>								m_MeshResources;
		std::vector<RBTexture*>								m_TextureResources;
		std::map<ID3D11ShaderResourceView*, RBTexture*>		m_WrapperTextureResources;

		bool												m_ShouldQuitLoaderThread;
		std::queue<RBLoaderThreadTask>							m_LoaderThreadTaskQueue;
		RBLoaderThreadData									m_LoaderThreadData;
	};

}
#endif
