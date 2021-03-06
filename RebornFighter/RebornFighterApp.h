#pragma once
#include "RBSystem/RBIApplication.h"
#include "RBEngine.h"
#include "PlayerController.h"
#include "RBRender/RBRenderSystem.h"
using namespace RebornEngine;

class RebornFighterApp : public RBIApplication
{
public:
	RebornFighterApp();
	~RebornFighterApp();

	bool Initialize();
	void UpdateScene(const RBTimer& timer);
	void RenderScene();

	void OnResize(int width, int height);
	const TCHAR* WindowTitle() { return L"Fighting Game Demo"; }

private:
	RBScene				m_Scene;
	RBCamera			m_Camera;
	RBShadowMap			m_ShadowMap;

	PlayerController*	m_Player;
	PlayerController*	m_AIPlayer;

	RBDebugRenderer		m_DebugRenderer;
	bool				m_DrawHitBound;
	RBText				m_Text;
};
