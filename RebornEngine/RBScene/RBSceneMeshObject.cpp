#include "RBSceneMeshObject.h"
#include "../RBEngine.h"
#include "../RBShader/RBShader.h"
namespace RebornEngine
{
	RBSceneMeshObject::RBSceneMeshObject()
		: RBSceneObject(), m_Mesh(nullptr), m_OverridingShader(nullptr), m_OverridingShaderFeatures(-1), m_bNeedUpdateMaterial(true)
	{
	}

	RBSceneMeshObject::~RBSceneMeshObject()
	{
	}

	RBSceneObject * RBSceneMeshObject::Clone() const
	{
		RBSceneMeshObject* pClone = new RBSceneMeshObject(*this);
		return pClone;

	}

	void RBSceneMeshObject::SetMesh(RBMesh * mesh)
	{
		m_Mesh = mesh;
		m_bNeedUpdateMaterial = true;

	}

	RBMesh * RBSceneMeshObject::GetMesh() const
	{
		return m_Mesh;
	}



	int RBSceneMeshObject::GetMeshElementCount() const
	{
		if (m_Mesh)
			return (int)m_Mesh->GetMeshElementCount();
		return 0;
	}

	void RBSceneMeshObject::SetMaterial(RBMaterial * materials, int materialNum)
	{
		m_Materials.clear();

		for (int i = 0; i < materialNum; i++)
		{
			m_Materials.push_back(materials[i]);
		}

		m_bNeedUpdateMaterial = false;

	}

	RBMaterial * RBSceneMeshObject::GetMaterial(int index)
	{
		UpdateMaterialsFromResource();

		return &m_Materials[index];
	}

	void RBSceneMeshObject::SaveMaterialsToFile()
	{
		if (!m_Mesh)
			return;

		while (!m_Mesh->IsResourceReady())
		{
			Sleep(10);
		}

		UpdateMaterialsFromResource();

		tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
		doc->InsertEndChild(doc->NewComment((string("Mesh path: ") + m_Mesh->GetPath()).c_str()));

		tinyxml2::XMLElement* elem_mat = doc->NewElement("Material");

		SerializeMaterialsToXML(doc, elem_mat);
		doc->InsertEndChild(elem_mat);

		string filepath = m_Mesh->GetPath();
		filepath = filepath.substr(0, filepath.length() - 3);
		filepath += "rmtl";

		doc->SaveFile(filepath.c_str());

		delete doc;

	}

	void RBSceneMeshObject::SerializeMaterialsToXML(tinyxml2::XMLDocument * doc, tinyxml2::XMLElement * elem_mat)
	{
		for (int i = 0; i < m_Mesh->GetMeshElementCount(); i++)
		{
			tinyxml2::XMLElement* elem_submesh = doc->NewElement("MeshElement");
			elem_submesh->SetAttribute("Name", m_Mesh->GetMeshElements()[i].GetName().c_str());

			if (i < (int)m_Materials.size())
			{
				RBMaterial& material = m_Materials[i];
				string shaderName = material.Shader->GetName();

				elem_submesh->SetAttribute("Shader", shaderName.c_str());
				for (int t = 0; t < material.TextureNum; t++)
				{
					string texturePath = "";

					if (material.Textures[t])
						texturePath = material.Textures[t]->GetPath();

					tinyxml2::XMLElement* elem_texture = doc->NewElement("Texture");
					elem_texture->SetAttribute("Slot", t);
					elem_texture->SetText(texturePath.c_str());
					elem_submesh->InsertEndChild(elem_texture);
				}
			}
			elem_mat->InsertEndChild(elem_submesh);
		}
	}

	void RBSceneMeshObject::SetOverridingShader(RBShader * shader, int features)
	{
		m_OverridingShader = shader;
		m_OverridingShaderFeatures = features;
	}

	RBAABB RBSceneMeshObject::GetAabb() const
	{
		if (m_Mesh)
			return m_Mesh->GetLocalSpaceAABB().GetTransformedAabb(m_NodeTransform);
		return RBAABB::Default;
	}

	const RBAABB & RBSceneMeshObject::GetMeshElementAabb(int index) const
	{
		if (m_Mesh)
			return m_Mesh->GetMeshElementAABB(index);
		return RBAABB::Default;
	}

	void RBSceneMeshObject::Draw()
	{
		Draw(false, 0);
	}

	void RBSceneMeshObject::Draw(bool instanced, int instanceCount)
	{
		if (!m_Mesh || !m_Mesh->IsResourceReady())
			return;

		UpdateMaterialsFromResource();

		for (UINT32 i = 0; i < m_Mesh->GetMeshElements().size(); i++)
		{
			RBShader* shader = nullptr;

			if (m_OverridingShader)
				shader = m_OverridingShader;
			else if (i < m_Materials.size())
				shader = m_Materials[i].Shader;

			if (shader)
			{
				int flag = m_Mesh->GetMeshElements()[i].GetFlag();

				int shaderFeatureMask = 0;
				if ((flag & MEF_Skinned) && !RBEngine::Instance()->IsEditor())
				{
					shaderFeatureMask |= RB_SFM_Skinned;
				}
				else if (instanced)
					shaderFeatureMask |= RB_SFM_Instanced;

				if (RBRENDERER.UseDeferredShading())
					shaderFeatureMask |= RB_SFM_Deferred;

				if (m_OverridingShader && m_OverridingShaderFeatures != -1)
					shader->Bind(m_OverridingShaderFeatures);
				else
					shader->Bind(shaderFeatureMask);

				// Hack: for shaders bound separately, consider textures loaded from mesh
				if (m_OverridingShader)
				{
					for (int t = 0; t < m_Mesh->GetMaterial(i).TextureNum; t++)
					{
						RBRENDERER.D3DImmediateContext()->PSSetShaderResources(t, 1, m_Mesh->GetMaterial(i).Textures[t]->GetPtrSRV());
					}
				}
				else
				{
					for (int t = 0; t < m_Materials[i].TextureNum; t++)
					{
						RBTexture* texture = m_Materials[i].Textures[t];
						ID3D11ShaderResourceView* srv[] = { nullptr };
						RBRENDERER.D3DImmediateContext()->PSSetShaderResources(t, 1, texture ? texture->GetPtrSRV() : srv);
					}
				}
			}

			if (instanced)
				m_Mesh->GetMeshElements()[i].DrawInstanced(instanceCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			else
				m_Mesh->GetMeshElements()[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	void RBSceneMeshObject::DrawDepthPass()
	{
		DrawDepthPass(false, 0);
	}

	void RBSceneMeshObject::DrawDepthPass(bool instanced, int instanceCount)
	{
		if (!m_Mesh || !m_Mesh->IsResourceReady())
			return;

		static RBShader* DefaultShader = RBShaderManager::Instance().GetShaderResource("Depth");

		for (UINT32 i = 0; i < m_Mesh->GetMeshElements().size(); i++)
		{
			RBShader* shader = DefaultShader;

			int flag = m_Mesh->GetMeshElements()[i].GetFlag();
			int shaderFeatureMask = 0;

			if (flag & MEF_Skinned)
				shaderFeatureMask |= RB_SFM_Skinned;
			else if (instanced)
				shaderFeatureMask |= RB_SFM_Instanced;

			shader->Bind(shaderFeatureMask);

			if (instanced)
				m_Mesh->GetMeshElements()[i].DrawInstanced(instanceCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			else
				m_Mesh->GetMeshElements()[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	void RBSceneMeshObject::DrawWithShader(RBShader * shader, bool instanced, int instanceCount)
	{
		if (!m_Mesh || !m_Mesh->IsResourceReady() || !shader)
			return;

		//RRenderer.D3DImmediateContext()->IASetInputLayout(m_Mesh->GetInputLayout());

		for (UINT32 i = 0; i < m_Mesh->GetMeshElements().size(); i++)
		{
			int flag = m_Mesh->GetMeshElements()[i].GetFlag();
			int shaderFeatureMask = 0;

			if (flag & MEF_Skinned)
				shaderFeatureMask |= RB_SFM_Skinned;
			else if (instanced)
				shaderFeatureMask |= RB_SFM_Instanced;

			if (RBRENDERER.UseDeferredShading())
				shaderFeatureMask |= RB_SFM_Deferred;

			shader->Bind(shaderFeatureMask);

			if (instanced)
				m_Mesh->GetMeshElements()[i].DrawInstanced(instanceCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			else
				m_Mesh->GetMeshElements()[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	float RBSceneMeshObject::GetResourceTimestamp()
	{
		if (m_Mesh)
			return m_Mesh->GetResourceTimestamp();

		return 0.0f;
	}

	void RBSceneMeshObject::UpdateMaterialsFromResource()
	{
		if (m_bNeedUpdateMaterial)
		{
			m_Materials = m_Mesh->GetMaterials();

			for (unsigned int i = 0; i < m_Materials.size(); i++)
			{
				if (m_Materials[i].Shader == nullptr)
					m_Materials[i].Shader = RBShaderManager::Instance().GetShaderResource("Default");
			}

			m_bNeedUpdateMaterial = false;
		}
	}

}

