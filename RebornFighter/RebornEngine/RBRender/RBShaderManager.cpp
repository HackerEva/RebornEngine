#include "RBShaderManager.h"
#include "../RBShader/RBSharedDefines.h"
#include "RBRenderer.h"
#include <fstream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"D3D10.lib")
#include "RBDirectXUtility.h"
using namespace RebornEngine;
RebornEngine::RBShaderManager::RBShaderManager()
{
}
RebornEngine::RBShaderManager::~RBShaderManager()
{
}

void RebornEngine::RBShaderManager::LoadShaders(const char * path)
{	// Set working directory to shader folder for compiling
	char pWorkingPath[1024];
	GetCurrentDirectoryA(1024, pWorkingPath);
	SetCurrentDirectoryA(path);

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;

	// Load shader
	std::string resFindingPath = "*.hlsl";
	hFind = FindFirstFileA(resFindingPath.data(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				std::string filename(FindFileData.cFileName);
				std::string fileFullPath = filename;
				ID3DBlob* pShaderCode = nullptr;
				ID3DBlob* pErrorMsg = nullptr;

				// Get actual shader name
				std::string shaderName = filename.substr(0, filename.length() - 8);

				if (m_Shaders.find(shaderName) == m_Shaders.end())
				{
					m_Shaders[shaderName] = RBShader();
				}

				// Read shader text from .hlsl
				std::ifstream fin;
				fin.open(fileFullPath, std::ios_base::binary);

				if (!fin.is_open())
					continue;

				fin.seekg(0, std::ios_base::end);
				int fileSize = (int)fin.tellg();
				char* pBuffer = new char[fileSize];
				
				fin.seekg(0);
				fin.read(pBuffer, fileSize);

				int shaderCompileFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
				shaderCompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
				HRESULT hr = 0;

#define HANDLE_SHADER_COMPILE_ERROR() \
	if (FAILED(hr)) \
	{ \
		OutputDebugStringA((char*)pErrorMsg->GetBufferPointer()); \
		OutputDebugStringA("\n"); \
	}

				// Detect shader type by file name suffix
				if (filename.find("_VS.hlsl") != std::string::npos)
				{
					if (SUCCEEDED(hr = D3DCompile(pBuffer, fileSize, filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", shaderCompileFlag, 0, &pShaderCode, &pErrorMsg)))
					{

						RBRENDERER.D3DDevice()->CreateVertexShader((DWORD*)pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &m_Shaders[shaderName].VertexShader);
#ifdef _DEBUG
						m_Shaders[shaderName].VertexShader->SetPrivateData(WKPDID_D3DDebugObjectName, filename.size(), filename.c_str());
#endif
					}
					HANDLE_SHADER_COMPILE_ERROR();

					if (strstr(pBuffer, "USE_SKINNING"))
					{
						filename = std::string("Skinned") + filename;

						D3D_SHADER_MACRO skinnedShaderMacro[] = { "USE_SKINNING", "1", NULL, NULL };

						if (SUCCEEDED(hr = D3DCompile(pBuffer, fileSize, filename.c_str(), skinnedShaderMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", shaderCompileFlag, 0, &pShaderCode, &pErrorMsg)))
						{
							RBRENDERER.D3DDevice()->CreateVertexShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &m_Shaders[shaderName].VertexShader_Skinned);
#ifdef _DEBUG
							m_Shaders[shaderName].VertexShader_Skinned->SetPrivateData(WKPDID_D3DDebugObjectName, filename.size(), filename.c_str());
#endif
						}
						HANDLE_SHADER_COMPILE_ERROR();
					}

					if (strstr(pBuffer, "USE_INSTANCING"))
					{
						filename = std::string("Instanced") + filename;

						D3D_SHADER_MACRO instancedShaderMacro[] = { "USE_INSTANCING", "1", NULL, NULL };

						if (SUCCEEDED(hr = D3DCompile(pBuffer, fileSize, filename.c_str(), instancedShaderMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", shaderCompileFlag, 0, &pShaderCode, &pErrorMsg)))
						{
							RBRENDERER.D3DDevice()->CreateVertexShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &m_Shaders[shaderName].VertexShader_Instanced);
#ifdef _DEBUG
							m_Shaders[shaderName].VertexShader_Instanced->SetPrivateData(WKPDID_D3DDebugObjectName, filename.size(), filename.c_str());
#endif
						}
						HANDLE_SHADER_COMPILE_ERROR();
					}
				}
				else if (filename.find("_PS.hlsl") != std::string::npos)
				{
					if (SUCCEEDED(hr = D3DCompile(pBuffer, fileSize, filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", shaderCompileFlag, 0, &pShaderCode, &pErrorMsg)))
					{
						RBRENDERER.D3DDevice()->CreatePixelShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &m_Shaders[shaderName].PixelShader);
#ifdef _DEBUG
						m_Shaders[shaderName].PixelShader->SetPrivateData(WKPDID_D3DDebugObjectName, filename.size(), filename.c_str());
#endif
					}
					HANDLE_SHADER_COMPILE_ERROR();

					if (strstr(pBuffer, "USE_DEFERRED_SHADING"))
					{
						filename = std::string("Deferred") + filename;

						D3D_SHADER_MACRO deferredShaderMacro[] = { "USE_DEFERRED_SHADING", "1", NULL, NULL };

						if (SUCCEEDED(hr = D3DCompile(pBuffer, fileSize, filename.c_str(), deferredShaderMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", shaderCompileFlag, 0, &pShaderCode, &pErrorMsg)))
						{
							RBRENDERER.D3DDevice()->CreatePixelShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &m_Shaders[shaderName].PixelShader_Deferred);
#ifdef _DEBUG
							m_Shaders[shaderName].PixelShader_Deferred->SetPrivateData(WKPDID_D3DDebugObjectName, filename.size(), filename.c_str());
#endif
						}
						HANDLE_SHADER_COMPILE_ERROR();
					}
				}
				else if (filename.find("_GS.hlsl") != std::string::npos)
				{
					if (SUCCEEDED(hr = D3DCompile(pBuffer, fileSize, filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_4_0", shaderCompileFlag, 0, &pShaderCode, &pErrorMsg)))
					{
						RBRENDERER.D3DDevice()->CreateGeometryShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &m_Shaders[shaderName].GeometryShader);
#ifdef _DEBUG
						m_Shaders[shaderName].GeometryShader->SetPrivateData(WKPDID_D3DDebugObjectName, filename.size(), filename.c_str());
#endif
					}
					HANDLE_SHADER_COMPILE_ERROR();
				}

				delete[] pBuffer;
				RB_SAFE_RELEASE(pShaderCode);
				RB_SAFE_RELEASE(pErrorMsg);
			}

		} while (FindNextFileA(hFind, &FindFileData) != 0);
	}

	// Restore working directory
	SetCurrentDirectoryA(pWorkingPath);

}

void RebornEngine::RBShaderManager::UnloadAllShaders()
{
}

bool RebornEngine::RBShaderManager::AddShader(const char * shaderName, 
				const void * pixelShaderBytecode, 
				SIZE_T pixelBytecodeLength, 
				const void * vertexShaderBytecode, 
				SIZE_T vertexBytecodeLength, 
				const void * geometryShaderBytecode, 
				SIZE_T geometryBytecodeLength)
{
	if (m_Shaders.find(shaderName) != m_Shaders.end())
		return false;

	RBShader shader = RBShader();

	if (pixelShaderBytecode && pixelBytecodeLength)
	{
		RBRENDERER.D3DDevice()->CreatePixelShader(pixelShaderBytecode, pixelBytecodeLength, NULL, &shader.PixelShader);
	}

	if (vertexShaderBytecode && vertexBytecodeLength)
	{
		RBRENDERER.D3DDevice()->CreateVertexShader(vertexShaderBytecode, vertexBytecodeLength, NULL, &shader.VertexShader);
	}

	if (geometryShaderBytecode && geometryBytecodeLength)
	{
		RBRENDERER.D3DDevice()->CreateGeometryShader(geometryShaderBytecode, geometryBytecodeLength, NULL, &shader.GeometryShader);
	}

	m_Shaders[shaderName] = shader;

	return true;
}

RebornEngine::RBShader * RebornEngine::RBShaderManager::GetShaderResource(const char * shaderName)
{
	std::map<std::string, RebornEngine::RBShader>::iterator iter = m_Shaders.find(shaderName);
	if (iter != m_Shaders.end())
		return &iter->second;
	return nullptr;

}

std::string RebornEngine::RBShaderManager::GetShaderName(const RBShader * shader) const
{
	std::map<std::string, RebornEngine::RBShader>::const_iterator iter = m_Shaders.begin();
	for (; iter != m_Shaders.end(); iter++)
	{
		if (iter->second == *shader)
		{
			return iter->first;
		}
	}

	return "";
}


