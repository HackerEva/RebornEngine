#ifndef RBSHADER_H
#define RBSHADER_H
#include<d3d11.h>
#include<string>
#include"../RBSystem/RBSingleton.h"
#include<map>
#include"../RBRender/RBRenderer.h"
namespace RebornEngine
{
	enum RBShaderFeatureMask
	{
		RB_SFM_Skinned = 1 << 0,
		RB_SFM_Instanced = 1 << 1,
		RB_SFM_Deferred = 1 << 2,
	};

	struct RBShader
	{
		ID3D11VertexShader*		VertexShader;
		ID3D11PixelShader*		PixelShader;
		ID3D11GeometryShader*	GeometryShader;

		ID3D11VertexShader*		VertexShader_Skinned;
		ID3D11VertexShader*		VertexShader_Instanced;

		ID3D11PixelShader*		PixelShader_Deferred;

		bool operator==(const RBShader& rhs) const;

		void Bind(int featureMasks = 0);
		std::string GetName() const;
	};
}

#endif