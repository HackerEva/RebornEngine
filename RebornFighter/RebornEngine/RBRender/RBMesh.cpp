#include "RBMesh.h"
#include "RBShaderManager.h"
#include "RBDirectXUtility.h"
using namespace RebornEngine;
using namespace std;
RBMesh::RBMesh(string path) 
	: RBIResource(RB_RT_Mesh, path),m_Animation(nullptr)
{
	m_LoadingFinishTime = 0.0f;
}

RBMesh::RBMesh(string path, const vector<RBMeshElement>& meshElements, const vector<RBMaterial>& materials) 
	: RBMesh(path)
{
	m_MeshElements = meshElements;
	m_Materials = materials;
}

RBMesh::RBMesh(string path, RBMeshElement * meshElements, int numElement, RBMaterial * materials, int numMaterial)
	: RBMesh(path) 
{
	assert(meshElements && numElement);
	m_MeshElements.assign(meshElements, meshElements + numElement);

	if (materials && numMaterial)
		m_Materials.assign(materials, materials + numMaterial);
	else
	{
		RBMaterial emptyMaterial;
		ZeroMemory(&emptyMaterial, sizeof(emptyMaterial));
		for (int i = 0; i < numElement; i++)
		{
			m_Materials.push_back(emptyMaterial);
		}
	}

}

RBMesh::~RBMesh()
{
	for (UINT32 i = 0; i < m_MeshElements.size(); i++)
	{
		m_MeshElements[i].Release();
	}

	RB_SAFE_DELETE(m_Animation);

}

void RBMesh::Serialize(RBSerializer & serializer)
{
	if (!serializer.EnsureHeader("RMSH", 4))
		return;

	serializer.SerializeVector(m_MeshElements, &RBSerializer::SerializeObject);
	serializer.SerializeVector(m_Materials, &RBSerializer::SerializeObject);
	serializer.SerializeObjectPtr(&m_Animation);
	serializer.SerializeVector(m_BoneInitInvMatrices);
	serializer.SerializeVector(m_BoneIdToName, &RBSerializer::SerializeData);

	if (serializer.IsReading())
	{
		UpdateAABB();
	}

}

RBMaterial RBMesh::GetMaterial(int index) const
{
	assert(index >= 0 && index < (int)m_Materials.size());
	return m_Materials[index];
}

vector<RBMaterial>& RBMesh::GetMaterials()
{
	return m_Materials;
}

vector<RBMeshElement>& RBMesh::GetMeshElements()
{
	return m_MeshElements;
}

int RBMesh::GetMeshElementCount() const
{
	return (int)m_MeshElements.size();
}

void RBMesh::SetMeshElements(RBMeshElement * meshElements, UINT numElement)
{	
	// TODO: use mutex
	assert(meshElements && numElement);
	m_MeshElements.assign(meshElements, meshElements + numElement);

}

void RBMesh::SetMaterials(RBMaterial * materials, UINT numMaterial)
{
	assert(materials && numMaterial);
	m_Materials.assign(materials, materials + numMaterial);
}

void RBMesh::UpdateAABB()
{
	m_Aabb = RBAABB::Default;
	for (UINT32 i = 0; i < m_MeshElements.size(); i++)
	{
		m_Aabb.Expand(m_MeshElements[i].GetAabb());
	}
}

const RBAABB & RBMesh::GetLocalSpaceAABB() const
{
	return m_Aabb;

}

const RBAABB & RBMesh::GetMeshElementAABB(int index) const
{
	return m_MeshElements[index].GetAabb();
}

void RBMesh::SetAnimation(RBAnimation * anim)
{
	m_Animation = anim;
}

RBAnimation * RBMesh::GetAnimation() const
{
	return m_Animation;
}

void RBMesh::SetBoneInitInvMatrices(vector<RBMatrix4x4>& bonePoses)
{
	m_BoneInitInvMatrices = move(bonePoses);
}

void RBMesh::SetBoneNameList(const vector<string>& boneNameList)
{
	m_BoneIdToName = boneNameList;
}

const string & RBMesh::GetBoneName(int boneId) const
{
	return m_BoneIdToName[boneId];
}

int RBMesh::GetBoneCount() const
{
	return (int)m_BoneIdToName.size();
}

void RBMesh::CacheAnimation(RBAnimation * anim)
{
	if (!anim || !m_BoneIdToName.size() || m_AnimationNodeCache.find(anim) != m_AnimationNodeCache.end())
		return;

	const char* rootNodeName = m_BoneIdToName[0].data();
	anim->SetRootNode(anim->GetNodeIdByName(rootNodeName));
	anim->BuildRootDisplacementArray();

	vector<int> nodeIdMap;
	nodeIdMap.resize(m_BoneIdToName.size());
	for (int i = 0; i < (int)m_BoneIdToName.size(); i++)
	{
		nodeIdMap[i] = anim->GetNodeIdByName(m_BoneIdToName[i].data());
	}

	m_AnimationNodeCache[anim] = nodeIdMap;
}

int RBMesh::GetCachedAnimationNodeId(RBAnimation * anim, int boneId)
{
	if (!anim || !m_BoneIdToName.size())
		return -1;

	map<RBAnimation*, vector<int>>::iterator iter = m_AnimationNodeCache.find(anim);
	if (iter == m_AnimationNodeCache.end())
		return -1;

	return iter->second[boneId];
}

void RBMesh::SetResourceTimestamp(float time)
{
	m_LoadingFinishTime = time;
}

float RBMesh::GetResourceTimestamp()
{
	return m_LoadingFinishTime;
}
