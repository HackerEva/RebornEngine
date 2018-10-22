#ifndef _RMESH_H
#define _RMESH_H
//Bug
#include "RBShaderManager.h"
#include "RBIResource.h"
#include "RBTexture.h"
#include "../RBMath/RBMath.h"
#include "../RBSystem/RBSerializer.h"
#include "../RBPhysics/RBPhysics.h"
#include "../RBSHader/RBConstBufferVS.h"	// MAX_BONE_COUNT
#include "RBMeshElement.h"
#include "RBAnimation.h"
#include "RBResourceManager.h"
#include "RBMaterial.h"
#include <map>

namespace RebornEngine
{
	struct RBMaterial;

	struct BoneMatrices
	{
		RBMatrix4x4 boneMatrix[MAX_BONE_COUNT];
	};

	class RBMesh : public RBIResource
	{
	public:
		RBMesh(string path);
		RBMesh(string path, const vector<RBMeshElement>& meshElements, const vector<RBMaterial>& materials);
		RBMesh(string path, RBMeshElement* meshElements, int numElement, RBMaterial* materials, int numMaterial);
		~RBMesh();

		void Serialize(RBSerializer& serializer);

		RBMaterial GetMaterial(int index) const;
		vector<RBMaterial>& GetMaterials();

		vector<RBMeshElement>& GetMeshElements();
		int GetMeshElementCount() const;

		void SetMeshElements(RBMeshElement* meshElements, UINT numElement);
		void SetMaterials(RBMaterial* materials, UINT numMaterial);

		void UpdateAABB();
		const RBAABB& GetLocalSpaceAABB() const;
		const RBAABB& GetMeshElementAABB(int index) const;

		void SetAnimation(RBAnimation* anim);
		RBAnimation* GetAnimation() const;

		void SetBoneInitInvMatrices(vector<RBMatrix4x4>& bonePoses);
		const RBMatrix4x4& GetBoneInitInvMatrices(int index) const { return m_BoneInitInvMatrices[index]; }

		void SetBoneNameList(const vector<string>& boneNameList);
		const string& GetBoneName(int boneId) const;
		int GetBoneCount() const;

		void CacheAnimation(RBAnimation* anim);
		int GetCachedAnimationNodeId(RBAnimation* anim, int boneId);

		void SetResourceTimestamp(float time);
		float GetResourceTimestamp();
	private:
		vector<RBMeshElement>	m_MeshElements;

		vector<RBMaterial>		m_Materials;
		RBAABB					m_Aabb;
		float					m_LoadingFinishTime;

		RBAnimation*				m_Animation;
		vector<RBMatrix4x4>		m_BoneInitInvMatrices;
		vector<string>			m_BoneIdToName;
		std::map<RBAnimation*, vector<int>> m_AnimationNodeCache;
	};
}


#endif
