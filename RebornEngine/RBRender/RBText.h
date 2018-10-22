#ifndef RBTEXT_H
#define RBTEXT_H
#include "../RBShader/RBShader.h"
#include <vector>
#include"RBVertex.h"
#include"RBTexture.h"
#include "RBMeshElement.h"
#include "RBShaderManager.h"
namespace RebornEngine
{
	class RBText
	{
	public:
		RBText();
		virtual ~RBText();

		void Initialize(RBTexture* fontTexture = nullptr, UINT rows = 0, UINT columns = 0);
		void AddText(const char* text, UINT start_x, UINT start_y, const RBColor& fg, const RBColor& bg);
		void SetText(const char* text, const RBColor& fg = RBColor(1, 1, 1), const RBColor& bg = RBColor(0, 0, 0, 1));
		virtual void Render();

	protected:
		std::vector<RBVertex::FONT_VERTEX>	m_Vertices;
		RBMeshRenderBuffer					m_VertexBuffer;
		RBTexture*							m_FontTexture;
		RBShader*							m_FontShader;
		UINT								m_Rows, m_Columns;
		bool								m_bNeedUpdateBuffer;
	};
}

#endif // !RBTEXT_H


