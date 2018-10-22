#include "RBShader.h"
#include"../RBRender/RBRenderer.h"
#include"../RBRender/RBShaderManager.h"


using namespace RebornEngine;
bool RBShader::operator==(const RBShader & rhs) const
{
	return
		VertexShader == rhs.VertexShader &&
		VertexShader_Skinned == rhs.VertexShader_Skinned &&
		VertexShader_Instanced == rhs.VertexShader_Instanced &&
		PixelShader == rhs.PixelShader &&
		PixelShader_Deferred == rhs.PixelShader_Deferred &&
		GeometryShader == rhs.GeometryShader;
}

void RBShader::Bind(int featureMasks)
{
	if ((featureMasks & RB_SFM_Skinned) && VertexShader_Skinned)
		RBRENDERER.SetVertexShader(VertexShader_Skinned);
	else if ((featureMasks & RB_SFM_Instanced) && VertexShader_Instanced)
		RBRENDERER.SetVertexShader(VertexShader_Instanced);
	else
		RBRENDERER.SetVertexShader(VertexShader);

	if ((featureMasks & RB_SFM_Deferred) && PixelShader_Deferred)
		RBRENDERER.SetPixelShader(PixelShader_Deferred);
	else
		RBRENDERER.SetPixelShader(PixelShader);

	RBRENDERER.SetGeometryShader(GeometryShader);

}

std::string RBShader::GetName() const
{
	return RBShaderManager::Instance().GetShaderName(this);
}
