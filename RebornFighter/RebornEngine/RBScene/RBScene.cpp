#include "RBScene.h"
#include <sstream> 
namespace RebornEngine
{
	void RBScene::Initialize()
	{
		cbPerObject.Initialize();
		cbScene.Initialize();
		cbBoneMatrices.Initialize();
		cbLight.Initialize();
		cbMaterial.Initialize();
		cbGlobal.Initialize();
	}

	void RBScene::Release()
	{
		DestroyAllObjects();

		cbPerObject.Release();
		cbScene.Release();
		cbBoneMatrices.Release();
		cbLight.Release();
		cbMaterial.Release();
		cbGlobal.Release();

	}

	RBSceneMeshObject * RBScene::CreateMeshObject(const char * meshName)
	{
		RBMesh* mesh = RBResourceManager::Instance().FindMesh(meshName);
		assert(mesh);

		return CreateMeshObject(mesh);

	}

	RBSceneMeshObject * RBScene::CreateMeshObject(RBMesh * mesh)
	{
		RBSceneMeshObject* meshObject = new RBSceneMeshObject();
		meshObject->SetMesh(mesh);

		m_SceneObjects.push_back(meshObject);

		return meshObject;

	}

	RBSceneObject * RBScene::CloneObject(RBSceneObject * obj)
	{
		RBSceneObject* tObj = obj->Clone();
		AddObjectToScene(tObj);

		return obj->Clone();
	}

	RBSceneObject * RBScene::FindObject(const char * name) const
	{
		for (UINT i = 0; i < m_SceneObjects.size(); i++)
		{
			if (m_SceneObjects[i]->GetName() == name)
				return m_SceneObjects[i];
		}

		return nullptr;
	}

	bool RBScene::AddObjectToScene(RBSceneObject * obj)
	{
		std::vector<RBSceneObject*>::iterator iter = find(m_SceneObjects.begin(), m_SceneObjects.end(), obj);
		if (iter == m_SceneObjects.end())
		{
			m_SceneObjects.push_back(obj);
			return true;
		}

		return false;

	}

	void RBScene::RemoveObjectFromScene(RBSceneObject * obj)
	{
		std::vector<RBSceneObject*>::iterator iter = find(m_SceneObjects.begin(), m_SceneObjects.end(), obj);
		if (iter != m_SceneObjects.end())
		{
			m_SceneObjects.erase(iter);
		}

	}

	void RBScene::DestroyObject(RBSceneObject * obj)
	{
		for (UINT i = 0; i < m_SceneObjects.size(); i++)
		{
			delete m_SceneObjects[i];
		}

		m_SceneObjects.clear();

	}

	void RBScene::DestroyAllObjects()
	{
		for (UINT i = 0; i < m_SceneObjects.size(); i++)
		{
			delete m_SceneObjects[i];
		}

		m_SceneObjects.clear();
	}

	void RBScene::LoadFromFile(const char * filename)
	{
		tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
		if (doc->LoadFile(filename) == tinyxml2::XML_SUCCESS)
		{
			tinyxml2::XMLElement* root = doc->RootElement();
			tinyxml2::XMLElement* elem_obj = root->FirstChildElement("SceneObject");
			while (elem_obj)
			{
				const char* name = elem_obj->Attribute("Name");
				const char* script = elem_obj->Attribute("Script");

				tinyxml2::XMLElement* elem_transform = elem_obj->FirstChildElement("Transform");
				RBMatrix4x4 transform;
				int n = 0;
				std::stringstream ss(elem_transform->GetText());

				while (ss >> ((float*)&transform)[n++] && n < 16)
				{
					if (ss.peek() == ',')
						ss.ignore();
				}

				RBSceneObject* sceneObj = nullptr;

				string obj_type = elem_obj->Attribute("Type");
				if (obj_type == "MeshObject")
				{
					const char* resPath = elem_obj->Attribute("Mesh");
					RBMesh* mesh = RBResourceManager::Instance().FindMesh(resPath);

					if (!mesh)
					{
						mesh = RBResourceManager::Instance().LoadFbxMesh(resPath, RB_RLM_Immediate);
					}

					RBSceneMeshObject* meshObj = CreateMeshObject(resPath);
					meshObj->SetTransform(transform);
					sceneObj = meshObj;

					tinyxml2::XMLElement* elem_mat = elem_obj->FirstChildElement("Material");
					vector<RBMaterial> xmlMaterials;

					if (elem_mat)
					{
						for (int i = 0; i < meshObj->GetMeshElementCount(); i++)
						{
							xmlMaterials.push_back(*meshObj->GetMaterial(i));
						}
					}

					while (elem_mat)
					{
						int index = elem_mat->IntAttribute("Index");

						tinyxml2::XMLElement* elem = elem_mat->FirstChildElement("MeshElement");
						while (elem)
						{
							const char* shaderName = elem->Attribute("Shader");
							RBMaterial material = { nullptr, 0 };
							material.Shader = RBShaderManager::Instance().GetShaderResource(shaderName);

							tinyxml2::XMLElement* elem_tex = elem->FirstChildElement();
							while (elem_tex)
							{
								const char* textureName = elem_tex->GetText();

								RBTexture* texture = RBResourceManager::Instance().FindTexture(textureName);

								if (!texture)
								{
									texture = RBResourceManager::Instance().LoadDDSTexture(RBResourceManager::GetResourcePath(textureName).data(), RB_RLM_Immediate);
								}

								material.Textures[material.TextureNum++] = texture;
								elem_tex = elem_tex->NextSiblingElement();
							}

							xmlMaterials[index] = material;
							elem = elem->NextSiblingElement();
						}

						elem_mat = elem_mat->NextSiblingElement("Material");
					}

					if (xmlMaterials.size())
					{
						meshObj->SetMaterial(xmlMaterials.data(), (int)xmlMaterials.size());
					}
				}

				if (name)
					sceneObj->SetName(name);

				if (script)
					sceneObj->SetScript(script);

				elem_obj = elem_obj->NextSiblingElement();
			}
		}

		delete doc;
	}

	void RBScene::SaveToFile(const char * filename)
	{
		tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
		tinyxml2::XMLElement* elem_scene = doc->NewElement("Scene");

		for (vector<RBSceneObject*>::iterator iter = m_SceneObjects.begin(); iter != m_SceneObjects.end(); iter++)
		{
			tinyxml2::XMLElement* elem_obj = doc->NewElement("SceneObject");

			if ((*iter)->GetName() != "")
			{
				elem_obj->SetAttribute("Name", (*iter)->GetName().c_str());
			}

			if ((*iter)->GetScript() != "")
			{
				elem_obj->SetAttribute("Script", (*iter)->GetScript().c_str());
			}

			if ((*iter)->GetType() == RB_SO_MeshObject)
			{
				elem_obj->SetAttribute("Type", "MeshObject");

				RBSceneMeshObject* meshObj = (RBSceneMeshObject*)(*iter);
				RBMesh* mesh = meshObj->GetMesh();
				elem_obj->SetAttribute("Mesh", mesh->GetPath().c_str());

				// Save materials
				for (int i = 0; i < meshObj->GetMeshElementCount(); i++)
				{
					RBMaterial meshMaterial = mesh->GetMaterial(i);
					RBMaterial* objMaterial = meshObj->GetMaterial(i);
					bool exportMaterial = false;

					if (meshMaterial.Shader != objMaterial->Shader ||
						meshMaterial.TextureNum != objMaterial->TextureNum)
					{
						exportMaterial = true;
					}

					if (!exportMaterial)
					{
						for (int j = 0; j < meshMaterial.TextureNum; j++)
						{
							if (meshMaterial.Textures[j] != objMaterial->Textures[j])
							{
								exportMaterial = true;
								break;
							}
						}
					}

					if (exportMaterial)
					{
						tinyxml2::XMLElement* elem_mat = doc->NewElement("Material");
						elem_mat->SetAttribute("Index", i);
						meshObj->SerializeMaterialsToXML(doc, elem_mat);
						elem_obj->InsertEndChild(elem_mat);
					}
				}
			}

			// Save transformation
			const RBMatrix4x4& t = (*iter)->GetNodeTransform();

			tinyxml2::XMLElement* elem_trans = doc->NewElement("Transform");

			char msg_buf[512];
			sprintf_s(msg_buf, 512,
				"%f, %f, %f, %f, "
				"%f, %f, %f, %f, "
				"%f, %f, %f, %f, "
				"%f, %f, %f, %f",
				t._m11, t._m12, t._m13, t._m14,
				t._m21, t._m22, t._m23, t._m24,
				t._m31, t._m32, t._m33, t._m34,
				t._m41, t._m42, t._m43, t._m44);

			elem_trans->SetText(msg_buf);
			elem_obj->InsertEndChild(elem_trans);
			elem_scene->InsertEndChild(elem_obj);
		}

		doc->InsertEndChild(elem_scene);
		doc->SaveFile(filename);
	}

	RBVector3 RBScene::TestMovingAabbWithScene(const RBAABB& aabb, const RBVector3& moveVec)
	{
		RBVector3 v = moveVec;

		for (vector<RBSceneObject*>::iterator iter = m_SceneObjects.begin(); iter != m_SceneObjects.end(); iter++)
		{
			if ((*iter)->GetType() == RB_SO_MeshObject)
			{
				RBSceneMeshObject* meshObj = (RBSceneMeshObject*)(*iter);

				if (aabb.GetSweptAabb(v).TestIntersectionWithAabb(meshObj->GetAabb()))
				{
					for (int i = 0; i < meshObj->GetMeshElementCount(); i++)
					{
						RBAABB elemAabb = meshObj->GetMeshElementAabb(i).GetTransformedAabb(meshObj->GetNodeTransform());
						v = aabb.TestDynamicCollisionWithAabb(v, elemAabb);
					}
				}
			}
		}

		return v;

	}

	void RBScene::Render(const RBFrustum * pFrustum)
	{
		for (vector<RBSceneObject*>::iterator iter = m_SceneObjects.begin(); iter != m_SceneObjects.end(); iter++)
		{
			if (pFrustum && !RBCollision::TestAABBInsideFrustum(*pFrustum, (*iter)->GetAabb()))
				continue;

			SHADER_OBJECT_BUFFER cbObject;
			cbObject.worldMatrix = (*iter)->GetNodeTransform();
			cbPerObject.UpdateContent(&cbObject);
			cbPerObject.ApplyToShaders();
			(*iter)->Draw();
		}
	}

	void RBScene::RenderDepthPass(const RBFrustum * pFrustum)
	{
		for (vector<RBSceneObject*>::iterator iter = m_SceneObjects.begin(); iter != m_SceneObjects.end(); iter++)
		{
			if (pFrustum && !RBCollision::TestAABBInsideFrustum(*pFrustum, (*iter)->GetAabb()))
				continue;

			SHADER_OBJECT_BUFFER cbObject;
			cbObject.worldMatrix = (*iter)->GetNodeTransform();
			cbPerObject.UpdateContent(&cbObject);
			cbPerObject.ApplyToShaders();
			(*iter)->DrawDepthPass();
		}

	}

	vector<RBSceneObject*>& RBScene::GetSceneObjects()
	{
		return m_SceneObjects;
	}
}
