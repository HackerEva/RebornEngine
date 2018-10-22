#ifndef RBSHADERMANAGER_H
#define RBSHADERMANAGER_H
#include<map>
#include<d3d11.h>
#include<string>
#include"../RBSystem/RBSingleton.h"
#include"../RBShader/RBShader.h"
#include "RBResourceManager.h"

namespace RebornEngine
{
	class RBShaderManager : public RBSingleton<RBShaderManager>
	{
		friend class RBSingleton<RBShaderManager>;
	public:

		void LoadShaders(const char* path);
		void UnloadAllShaders();

		bool AddShader(const char* shaderName,
			const void* pixelShaderBytecode,
			SIZE_T pixelBytecodeLength,
			const void* vertexShaderBytecode,
			SIZE_T vertexBytecodeLength,
			const void* geometryShaderBytecode = nullptr,
			SIZE_T geometryBytecodeLength = 0);

		RBShader* GetShaderResource(const char* shaderName);
		std::string GetShaderName(const RBShader* shader) const;

	private:
		RBShaderManager();
		~RBShaderManager();

		std::map<std::string, RBShader>	m_Shaders;
	};
}

#endif