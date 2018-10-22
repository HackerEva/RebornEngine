#include "RBMaterial.h"
#include <string>



void RebornEngine::RBMaterial::Serialize(RBSerializer & serializer)
{
	using namespace std;
	if (serializer.IsReading())
	{
		std::string shaderName;
		serializer.SerializeData(shaderName);
		Shader = RBShaderManager::Instance().GetShaderResource(shaderName.c_str());

		serializer.SerializeData(TextureNum);

		int i;
		for (i = 0; i < TextureNum; i++)
		{
			string textureName;
			serializer.SerializeData(textureName);
			Textures[i] = RBResourceManager::Instance().FindTexture(textureName.c_str());
			if (!Textures[i])
				Textures[i] = RBResourceManager::Instance().LoadDDSTexture(textureName.c_str(), RB_RLM_Immediate);
		}

		for (; i < 8; i++)
		{
			Textures[i] = nullptr;
		}
	}
	else
	{
		string shaderName;
		if (Shader)
			shaderName = Shader->GetName();

		serializer.SerializeData(shaderName);
		serializer.SerializeData(TextureNum);

		int i;
		for (i = 0; i < TextureNum; i++)
		{
			string textureName = Textures[i]->GetPath();
			serializer.SerializeData(textureName);
		}
	}

}
