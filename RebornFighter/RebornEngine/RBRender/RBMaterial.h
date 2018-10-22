#ifndef RBMATERIAL_H
#define RBMATERIAL_H

#include"../RBShader/RBShader.h"
#include"../RBSystem/RBSerializer.h"
#include"RBTexture.h"
namespace RebornEngine
{
	class RBTexture;
	struct RBMaterial
	{
		RBShader*					Shader;
		int							TextureNum;
		RBTexture*					Textures[8];

		void Serialize(RBSerializer& serializer);
	};
}
#endif