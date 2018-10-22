#ifndef RBSCENEMESHOBJECT_H
#define RBSCENEMESHOBJECT_H
#include "../RBRender/RBMesh.h"
#include "../tinyxml2/tinyxml2.h"
#include "RBSceneObject.h"
namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}
namespace RebornEngine
{
	class RBSceneMeshObject : public RBSceneObject
	{
	public:
		RBSceneMeshObject();
		~RBSceneMeshObject();

		RBSceneObjectType GetType() const override { return RB_SO_MeshObject; }
		RBSceneObject* Clone() const;

		void SetMesh(RBMesh* mesh);
		RBMesh* GetMesh() const;

		int GetMeshElementCount() const;
		void SetMaterial(RBMaterial* materials, int materialNum);
		RBMaterial* GetMaterial(int index);

		void SaveMaterialsToFile();
		void SerializeMaterialsToXML(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* elem_mat);

		void SetOverridingShader(RBShader* shader, int features = -1);

		RBAABB GetAabb() const;
		const RBAABB& GetMeshElementAabb(int index) const;

		void Draw();
		void Draw(bool instanced, int instanceCount);
		void DrawDepthPass();
		void DrawDepthPass(bool instanced, int instanceCount);
		void DrawWithShader(RBShader* shader, bool instanced = false, int instanceCount = 0);

		float GetResourceTimestamp();
	protected:
		void UpdateMaterialsFromResource();

		RBMesh*					m_Mesh;
		vector<RBMaterial>		m_Materials;
		RBShader*				m_OverridingShader;
		int						m_OverridingShaderFeatures;
		bool					m_bNeedUpdateMaterial;
	};

}

#endif
