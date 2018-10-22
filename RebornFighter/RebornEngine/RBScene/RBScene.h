#ifndef RBSCENE_H
#define RBSCENE_H
#include"../RBShader/RBShader.h"
#include "RBSceneMeshObject.h"
#include "../RBMath/RBMath.h"
#include "../RBPhysics/RBPhysics.h"
#include "../RBShader/RBShaderConstantBuffer.h"
#include "../RBShader/RBConstBufferPS.h"
#include "../RBRender/RBMesh.h"
#include "RBCamera.h"
#include "RBSkybox.h"

namespace RebornEngine
{
	class RBScene
	{
	public:
		void Initialize();
		void Release();

		RBSceneMeshObject* CreateMeshObject(const char* meshName);
		RBSceneMeshObject* CreateMeshObject(RBMesh* mesh);

		RBSceneObject* CloneObject(RBSceneObject* obj);
		RBSceneObject* FindObject(const char* name) const;

		bool AddObjectToScene(RBSceneObject* obj);
		void RemoveObjectFromScene(RBSceneObject* obj);
		void DestroyObject(RBSceneObject* obj);
		void DestroyAllObjects();

		void LoadFromFile(const char* filename);
		void SaveToFile(const char* filename);

		RBVector3 TestMovingAabbWithScene(const RBAABB& aabb, const RBVector3& moveVec);
		void Render(const RBFrustum* pFrustum = nullptr);
		void RenderDepthPass(const RBFrustum* pFrustum = nullptr);

		std::vector<RBSceneObject*>& GetSceneObjects();

		// Constant buffers
		RBShaderConstantBuffer<SHADER_SCENE_BUFFER,		RB_CBST_VS | RB_CBST_GS | RB_CBST_PS, 0>	cbScene;
		RBShaderConstantBuffer<SHADER_GLOBAL_BUFFER,	RB_CBST_VS | RB_CBST_PS, 1>					cbGlobal;
		RBShaderConstantBuffer<SHADER_OBJECT_BUFFER,	RB_CBST_VS, 2>								cbPerObject;
		RBShaderConstantBuffer<SHADER_SKINNED_BUFFER,	RB_CBST_VS, 4>								cbBoneMatrices;
		RBShaderConstantBuffer<SHADER_LIGHT_BUFFER,		RB_CBST_PS, 2>								cbLight;
		RBShaderConstantBuffer<SHADER_MATERIAL_BUFFER,	RB_CBST_PS, 3>								cbMaterial;
	private:

		vector<RBSceneObject*>		m_SceneObjects;
	};
}


#endif // !RBScene
