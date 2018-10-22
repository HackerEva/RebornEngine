#ifndef RBVERTEXDECLARATION_H
#define RBVERTEXDECLARATION_H
#include"../RBMath/RBMath.h"
#include"../RBSystem/RBSingleton.h"
#include<d3d11.h>
#include<string>
#include<map>

namespace RebornEngine {

#define VERTEX_TYPE_BEGIN(v) struct v { static const char* GetTypeName() { return #v; }
#define VERTEX_TYPE_END };
#define VERTEX_TYPE_DECLARE_LEXICOGRAPHICAL_COMPARE(v)	\
		bool operator<(const v& rhs) const				\
		{												\
		return std::lexicographical_compare(			\
			(const float*)this,							\
			(const float*)this + sizeof(v) / 4,			\
			(const float*)&rhs,							\
			(const float*)&rhs + sizeof(v) / 4);		\
		}

	enum RBVertexComponent
	{
		VertexComponent_BoneId,
		VertexComponent_BoneWeights,
		VertexComponent_Pos,
		VertexComponent_UV0,
		VertexComponent_Normal,
		VertexComponent_Tangent,
		VertexComponent_UV1,

		VertexComponent_Count,
	};

	struct RBShaderInputVertex
	{
		char Type[10];
		char Semantic[20];
	};

	enum RBVertexComponentMask
	{
		RB_VCM_BoneId = 1 << VertexComponent_BoneId,
		RB_VCM_BoneWeights = 1 << VertexComponent_BoneWeights,
		RB_VCM_Pos = 1 << VertexComponent_Pos,
		RB_VCM_UV0 = 1 << VertexComponent_UV0,
		RB_VCM_Normal = 1 << VertexComponent_Normal,
		RB_VCM_Tangent = 1 << VertexComponent_Tangent,
		RB_VCM_UV1 = 1 << VertexComponent_UV1,
	};

	enum RBVertexComponents
	{
		VC_PosNormal = RB_VCM_Pos | RB_VCM_Normal,
		VC_PosUV0Normal = RB_VCM_Pos | RB_VCM_UV0 | RB_VCM_Normal,
		VC_PosUV0NormalUV1 = RB_VCM_Pos | RB_VCM_UV0 | RB_VCM_Normal | RB_VCM_UV1,
		VC_PosUV0NormalTangent = RB_VCM_Pos | RB_VCM_UV0 | RB_VCM_Normal | RB_VCM_Tangent,
		VC_PosUV0NormalTangentUV1 = RB_VCM_Pos | RB_VCM_UV0 | RB_VCM_Normal | RB_VCM_Tangent | RB_VCM_UV1,
	};

	namespace RBVertex
	{
		VERTEX_TYPE_BEGIN(MESH_LOADER_VERTEX)
		RBVector3 pos;
		RBVector2 uv0;
		RBVector2 uv1;
		RBVector3 normal;
		RBVector3 tangent;
		int boneId[4];
		float weight[4];
		VERTEX_TYPE_DECLARE_LEXICOGRAPHICAL_COMPARE(MESH_LOADER_VERTEX)
		VERTEX_TYPE_END

		VERTEX_TYPE_BEGIN(MESH_VERTEX)
		RBVector3 pos;
		RBVector2 uv0;
		RBVector3 normal;
		RBVector3 tangent;
		RBVector2 uv1;
		VERTEX_TYPE_END

		VERTEX_TYPE_BEGIN(PRIMITIVE_VERTEX)
		RBVector4	pos;
		RBColor	color;
		VERTEX_TYPE_END


		VERTEX_TYPE_BEGIN(SKYBOX_VERTEX)
		RBVector4 pos;
		VERTEX_TYPE_END

	
		VERTEX_TYPE_BEGIN(PARTICLE_VERTEX)
		RBVector4 pos;
		RBVector4 color;
		float rot;
		RBVector4 uvScaleOffset;
		VERTEX_TYPE_END

		VERTEX_TYPE_BEGIN(FONT_VERTEX)
		RBVector4	pos;
		RBColor	color_fg;
		RBColor	color_bg;
		RBVector2	uv;
		VERTEX_TYPE_END
	}

	class RBVertexDeclaration : public RBSingleton<RBVertexDeclaration>
	{
	public:
		RBVertexDeclaration();
		~RBVertexDeclaration();

		void Initialize();
		void Release();

		ID3D11InputLayout* GetInputLayout(const std::string& vertexTypeName) const;
		ID3D11InputLayout* GetInputLayoutByVertexComponents(int vertexComponents);
		static std::string GetVertexComponentsString(int vertexComponents);
		static int GetVertexStride(int vertexComponents);
		static void CopyVertexComponents(void* out, const RBVertex::MESH_LOADER_VERTEX* in, int count, int vertexComponents);
	private:
		std::map<std::string, ID3D11InputLayout*>		m_InputLayouts;
		std::map<int, ID3D11InputLayout*>				m_VertexComponentInputLayouts;
	};

}
#endif

