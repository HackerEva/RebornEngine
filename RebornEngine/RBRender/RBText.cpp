#include "RBText.h"
#include "RBResourceManager.h"

using namespace RebornEngine;
RBText::RBText() 
	: m_FontShader(nullptr), m_bNeedUpdateBuffer(true)
{
}

RBText::~RBText()
{
	m_VertexBuffer.Release();
}

void RBText::Initialize(RBTexture * fontTexture, UINT rows, UINT columns)
{
	if (!fontTexture)
	{
		m_FontTexture = RBResourceManager::Instance().LoadDDSTexture("Assets/Fonts/Fixedsys_9c.DDS", RB_RLM_Immediate);
		m_Rows = m_Columns = 16;
	}
	else
	{
		m_FontTexture = fontTexture;
		m_Rows = rows;
		m_Columns = columns;
	}

	ID3D11InputLayout* pInputLayout = RBVertexDeclaration::Instance().GetInputLayout(RBVertex::FONT_VERTEX::GetTypeName());
	m_VertexBuffer.CreateVertexBuffer(nullptr, sizeof(RBVertex::FONT_VERTEX), 65536, pInputLayout, true);

	m_FontShader = RBShaderManager::Instance().GetShaderResource("Font");

}

void RBText::AddText(const char * text, UINT start_x, UINT start_y, const RBColor & fg, const RBColor & bg)
{
	float glyph_width = (float)m_FontTexture->GetWidth() / m_Columns;
	float glyph_height = (float)m_FontTexture->GetHeight() / m_Rows;

	float x0 = glyph_width * start_x, y0 = glyph_height * start_y;

	bool isEscape = false;

	for (UINT i = 0; text[i]; i++)
	{
		unsigned char ch = text[i];

		if (isEscape)
		{
			if (ch == 'n')
			{
				x0 = glyph_width * start_x;
				y0 += glyph_height;
			}
			isEscape = false;
			continue;
		}
		else
		{
			if (ch == '\\')
			{
				isEscape = true;
				continue;
			}
			else if (ch == '\n')
			{
				x0 = glyph_width * start_x;
				y0 += glyph_height;
				continue;
			}
		}

		float u0 = float(ch % m_Rows) / m_Columns;
		float v0 = float(ch / m_Rows) / m_Rows;
		float u1 = u0 + 1.0f / m_Columns;
		float v1 = v0 + 1.0f / m_Rows;

		float x1 = x0 + glyph_width;
		float y1 = y0 + glyph_height;

		RBVertex::FONT_VERTEX v[6] =
		{
			{ RBVector4(x0, y0, 0), fg, bg, RBVector2(u0, v0) },
			{ RBVector4(x1, y0, 0), fg, bg, RBVector2(u1, v0) },
			{ RBVector4(x0, y1, 0), fg, bg, RBVector2(u0, v1) },

			{ RBVector4(x0, y1, 0), fg, bg, RBVector2(u0, v1) },
			{ RBVector4(x1, y0, 0), fg, bg, RBVector2(u1, v0) },
			{ RBVector4(x1, y1, 0), fg, bg, RBVector2(u1, v1) },
		};

		for (int j = 0; j < 6; j++)
		{
			m_Vertices.push_back(v[j]);
		}

		x0 += glyph_width;
	}

	m_bNeedUpdateBuffer = true;

}

void RBText::SetText(const char * text, const RBColor & fg, const RBColor & bg)
{
	m_Vertices.clear();

	AddText(text, 0, 0, fg, bg);
}

void RBText::Render()
{
	if (m_bNeedUpdateBuffer)
	{
		m_VertexBuffer.UpdateDynamicVertexBuffer(m_Vertices.data(), sizeof(RBVertex::FONT_VERTEX), (UINT)m_Vertices.size());
		m_bNeedUpdateBuffer = false;
	}

	RBRENDERER.SetBlendState(RBBlend_AlphaBlending);
	m_FontShader->Bind();
	RBRENDERER.D3DImmediateContext()->PSSetShaderResources(0, 1, m_FontTexture->GetPtrSRV());
	m_VertexBuffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
