#include "RebornFighterApp.h"
#include "RBRender/RBRenderer.h"
#include "RBSystem/RBScriptSystem.h"
RebornFighterApp::RebornFighterApp()
	: m_Player(nullptr), m_DrawHitBound(false)
{

}

RebornFighterApp::~RebornFighterApp()
{
	RB_SAFE_DELETE(m_Player);
	RB_SAFE_DELETE(m_AIPlayer);
	m_Scene.Release();
	m_DebugRenderer.Release();
	RBShaderManager::Instance().UnloadAllShaders();
	RBResourceManager::Instance().Destroy();
	RBSCRIPT.Shutdown();
}

bool RebornFighterApp::Initialize()
{
	RBSCRIPT.Initialize();
	RBSCRIPT.Start();

	RBResourceManager::Instance().Initialize();
	//RResourceManager::Instance().LoadAllResources();
	RBShaderManager::Instance().LoadShaders("../Shaders");

	m_DebugRenderer.Initialize();

	m_Scene.Initialize();
	m_Scene.LoadFromFile("../Assets/ScriptTestMap.rmap");

	m_ShadowMap.Initialize(1024, 1024);

	RBMatrix4x4 cameraMatrix = RBMatrix4x4::CreateXAxisRotation(0.09f * 180 / PI) * RBMatrix4x4::CreateYAxisRotation(3.88659930f * 180 / PI);
	cameraMatrix.SetTranslation(RBVector3(407.023712f, 339.007507f, 876.396484f));
	m_Camera.SetTransform(cameraMatrix);
	m_Camera.SetupView(65.0f, RBRENDERER.AspectRatio(), 1.0f, 10000.0f);

	//RSceneObject* player = m_Scene.FindObject("Player");
	//for (UINT i = 0; i < m_Scene.GetSceneObjects().size(); i++)
	//{
	//	RSceneObject* obj = m_Scene.GetSceneObjects()[i];
	//	obj->SetScript("UpdateObject");
	//}

	m_Player = new PlayerController();
	m_Player->SetScene(&m_Scene);
	m_Player->SetPosition(RBVector3(0, 100.0f, 0));
	m_Player->Cache();

	m_AIPlayer = new PlayerController();
	m_AIPlayer->SetScene(&m_Scene);
	m_AIPlayer->SetPosition(RBVector3(-465, 50, -760));
	m_AIPlayer->Cache();

	m_Text.Initialize(RBResourceManager::Instance().LoadDDSTexture("../Assets/Fonts/Fixedsys_9c.DDS", RB_RLM_Immediate), 16, 16);
	return true;
}

void RebornFighterApp::UpdateScene(const RBTimer& timer)
{
	// Update light constant buffer
	SHADER_LIGHT_BUFFER cbLight;
	ZeroMemory(&cbLight, sizeof(cbLight));

	// Setup ambient color
	cbLight.HighHemisphereAmbientColor = RBVector4(1.0f, 1.0f, 1.0f, 0.4f);
	cbLight.LowHemisphereAmbientColor = RBVector4(0.2f, 0.2f, 0.2f, 1.0f);

	RBVector4 dirLightVec = RBVector4(RBVector3(0.25f, 1.0f, 0.5f).GetNormalizedVec3(), 1.0f);

	RBVector3 sunVec = RBVector3(sinf(1.0f) * 0.5f, 0.25f, cosf(1.0) * 0.5f).GetNormalizedVec3() * 2000.0f;
	RBMatrix4x4 shadowViewMatrix = RBMatrix4x4::CreateLookAtViewLH(sunVec, RBVector3(0.0f, 0.0f, 0.0f), RBVector3(0.0f, 1.0f, 0.0f));

	cbLight.DirectionalLightCount = 1;
	cbLight.DirectionalLight[0].Color = RBVector4(1.0f, 1.0f, 0.8f, 2.0f);
	cbLight.DirectionalLight[0].Direction = RBVector4(sunVec.GetNormalizedVec3(), 1.0f);

	cbLight.CascadedShadowCount = 1;


	// Update scene constant buffer
	SHADER_SCENE_BUFFER cbScene;
	ZeroMemory(&cbScene, sizeof(cbScene));

#if 1
	cbScene.viewMatrix = m_Camera.GetViewMatrix();
	cbScene.projMatrix = m_Camera.GetProjectionMatrix();
	cbScene.viewProjMatrix = cbScene.viewMatrix * cbScene.projMatrix;
	cbScene.cameraPos = m_Camera.GetPosition();

	cbLight.CameraPos = m_Camera.GetPosition();
#else
	RBMatrix4x4 cameraMatrix = RBMatrix4x4::CreateXAxisRotation(0.09f * 180 / PI) * RBMatrix4x4::CreateYAxisRotation(3.88659930f * 180 / PI);
	cameraMatrix.SetTranslation(RBVector3(407.023712f, 339.007507f, 876.396484f));

	cbScene.viewMatrix = RBMatrix4x4::CreateLookAtViewLH(RBVector3(407.023712f, 339.007507f, 876.396484f), m_Player->GetPosition(), RBVector3(0, 1, 0));
	cbScene.projMatrix = RBMatrix4x4::CreatePerspectiveProjectionLH(65.0f, RBRENDERER.AspectRatio(), 1.0f, 10000.0f);
	cbScene.viewProjMatrix = cbScene.viewMatrix * cbScene.projMatrix;
	cbScene.cameraPos = cameraMatrix.GetTranslation();

	cbLight.CameraPos = cameraMatrix.GetTranslation();
#endif

	m_ShadowMap.SetViewMatrix(shadowViewMatrix);
	m_ShadowMap.SetOrthogonalProjection(5000.0f, 5000.0f, 0.1f, 5000.0f);

	RBMatrix4x4 shadowTransform(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	RBMatrix4x4 shadowViewProjMatrix = m_ShadowMap.GetViewMatrix() * m_ShadowMap.GetProjectionMatrix();
	cbScene.shadowViewProjMatrix[0] = shadowViewProjMatrix;
	shadowViewProjMatrix *= shadowTransform;
	cbScene.shadowViewProjBiasedMatrix[0] = shadowViewProjMatrix;

	m_Scene.cbScene.UpdateContent(&cbScene);
	m_Scene.cbScene.ApplyToShaders();

	m_Scene.cbLight.UpdateContent(&cbLight);
	m_Scene.cbLight.ApplyToShaders();

	SHADER_MATERIAL_BUFFER cbMaterial;
	ZeroMemory(&cbMaterial, sizeof(cbMaterial));

	cbMaterial.SpecularColorAndPower = RBVector4(1.0f, 1.0f, 1.0f, 512.0f);
	cbMaterial.GlobalOpacity = 1.0f;

	m_Scene.cbMaterial.UpdateContent(&cbMaterial);
	m_Scene.cbMaterial.ApplyToShaders();

	if (RBINPUT.GetKeyState('P') == KEY_PRESS)
		m_DrawHitBound = !m_DrawHitBound;

	SHADER_GLOBAL_BUFFER cbScreen;
	ZeroMemory(&cbScreen, sizeof(cbScreen));

	cbScreen.ScreenSize = RBVector4((float)RBRENDERER.GetClientWidth(), (float)RBRENDERER.GetClientHeight(),
		1.0f / (float)RBRENDERER.GetClientWidth(), 1.0f / (float)RBRENDERER.GetClientHeight());
	cbScreen.UseGammaCorrection = RBRENDERER.UsingGammaCorrection();

	m_Scene.cbGlobal.UpdateContent(&cbScreen);
	m_Scene.cbGlobal.ApplyToShaders();

	if (m_Player)
	{
		if (RBINPUT.GetKeyState('R') == KEY_DOWN)
		{
			m_Player->SetPosition(RBVector3(0, 100.0f, 0));
			m_AIPlayer->SetPosition(RBVector3(-465, 50, -760));
		}

		m_Player->PreUpdate(timer);

		RBVector3 moveVec = RBVector3(0, 0, 0);

		RBVector3 charRight = m_Camera.GetNodeTransform().GetRight();
		RBVector3 charForward = charRight.Cross(RBVector3(0, 1, 0));

		if (m_Player->GetBehavior() == BHV_Running ||
			m_Player->GetBehavior() == BHV_Idle)
		{
			if (RBINPUT.IsKeyDown('W')) moveVec += charForward;
			if (RBINPUT.IsKeyDown('S')) moveVec -= charForward;
			if (RBINPUT.IsKeyDown('A')) moveVec -= charRight;
			if (RBINPUT.IsKeyDown('D')) moveVec += charRight;

			if (moveVec.SquaredMagitude() > 0.0f)
			{
				moveVec.Normalize();
				moveVec *= timer.DeltaTime() * 500.0f;

				m_Player->SetBehavior(BHV_Running);
			}
			else
			{
				m_Player->SetBehavior(BHV_Idle);
			}

			if (RBINPUT.GetKeyState(VK_SPACE) == KEY_DOWN)
			{
				m_Player->SetBehavior(BHV_SpinAttack);
			}

			if (RBINPUT.GetKeyState('J') == KEY_DOWN)
			{
				m_Player->SetBehavior(BHV_Punch);
			}
		}

		if (m_Player->GetBehavior() == BHV_Running ||
			m_Player->GetBehavior() == BHV_Idle ||
			m_Player->GetBehavior() == BHV_Kick)
		{
			if (RBINPUT.GetKeyState('K') == KEY_PRESS)
			{
				m_Player->SetBehavior(BHV_Kick);
			}
		}

		if (RBINPUT.GetKeyState('C') == KEY_PRESS)
			m_Player->SetBehavior(BHV_HitDown);

		moveVec += RBVector3(0, -1000.0f * timer.DeltaTime(), 0);
		m_Player->UpdateMovement(timer, moveVec);

		RBMatrix4x4 playerTranslation = RBMatrix4x4::CreateTranslation(m_Player->GetNodeTransform().GetTranslation());
		RBMatrix4x4 cameraTransform = RBMatrix4x4::CreateTranslation(0.0f, 500.0f, 300.0f) * playerTranslation;

		RBAABB camAabb;
		RBVector3 lookTarget = m_Player->GetPosition() + RBVector3(0, 125, 0);
		camAabb.pMin = RBVector3(-5, -5, -5) + lookTarget;
		camAabb.pMax = RBVector3(5, 5, 5) + lookTarget;

		RBVector3 camVec = cameraTransform.GetTranslation() - lookTarget;
		camVec = m_Scene.TestMovingAabbWithScene(camAabb, camVec);

		//m_Camera.SetTransform(cameraTransform);
		static RBVector3 actualCamVec;
		actualCamVec = RBVector3::Lerp(actualCamVec, camVec, 5.0f * timer.DeltaTime());
		m_Camera.SetPosition(actualCamVec + lookTarget);
		m_Camera.LookAt(lookTarget);

		//playerAabb.pMin = RBVector3(-50.0f, 0.0f, -50.0f) + m_Player->GetPosition();
		//playerAabb.pMax = RBVector3(50.0f, 150.0f, 50.0f) + m_Player->GetPosition();
		//m_DebugRenderer.DrawAabb(playerAabb);

		//m_DebugRenderer.DrawFrustum(m_Camera.GetFrustum());

		//RSphere s = { RBVector3(0, 100, 0), 50.0f };
		//RCapsule cap = m_Player->GetCollisionShape();
		//RColor color = RColor(0, 1, 1);
		//if (RCollision::TestSphereWithCapsule(s, cap))
		//{
		//	color = RColor(1, 0, 0);
		//}
		//m_DebugRenderer.DrawSphere(s.center, s.radius, color);
		//m_DebugRenderer.DrawSphere(cap.start, cap.radius, color);
		//m_DebugRenderer.DrawSphere(cap.end, cap.radius, color);


		if (m_Player->GetBehavior() == BHV_SpinAttack &&
			m_Player->GetBehaviorTime() > 0.3f &&
			m_Player->GetBehaviorTime() < 0.6f)
		{
			RBSphere hit_sphere;
			hit_sphere.center = m_Player->GetPosition() + m_Player->GetNodeTransform().GetForward() * 50 + RBVector3(0, 50, 0);
			hit_sphere.radius = 50.0f;
			if (m_DrawHitBound)
				m_DebugRenderer.DrawSphere(hit_sphere.center, hit_sphere.radius);

			if (RBCollision::TestSphereWithCapsule(hit_sphere, m_AIPlayer->GetCollisionShape()))
			{
				if (m_AIPlayer->GetBehavior() != BHV_HitDown)
				{
					RBVector3 relVec = hit_sphere.center - m_AIPlayer->GetPosition();
					relVec.y = 0.0f;
					RBVector3 playerForward = m_Player->GetNodeTransform().GetForward();
					if (playerForward.Dot(relVec) >= 0)
						relVec = -playerForward;
					relVec.Normalize();

					m_AIPlayer->SetPlayerRotation(RAD_TO_DEG(atan2f(relVec.x, relVec.z)));
				}
				m_AIPlayer->SetBehavior(BHV_HitDown);
			}
		}

		if (m_Player->GetBehavior() == BHV_Punch &&
			m_Player->GetBehaviorTime() > 0.1f &&
			m_Player->GetBehaviorTime() < 0.3f)
		{
			RBSphere hit_sphere;
			hit_sphere.center = m_Player->GetPosition() + m_Player->GetNodeTransform().GetForward() * 50 + RBVector3(0, 100, 0);
			hit_sphere.radius = 20.0f;
			if (m_DrawHitBound)
				m_DebugRenderer.DrawSphere(hit_sphere.center, hit_sphere.radius);

			if (RBCollision::TestSphereWithCapsule(hit_sphere, m_AIPlayer->GetCollisionShape()))
			{
				if (m_AIPlayer->GetBehavior() != BHV_HitDown &&
					m_AIPlayer->GetBehavior() != BHV_Hit)
				{
					m_AIPlayer->SetBehavior(BHV_Hit);
				}
			}
		}

		if (m_Player->GetBehavior() == BHV_Kick &&
			m_Player->GetBehaviorTime() > 0.1f &&
			m_Player->GetBehaviorTime() < 0.3f)
		{
			RBSphere hit_sphere;
			hit_sphere.center = m_Player->GetPosition() + m_Player->GetNodeTransform().GetForward() * 50 + RBVector3(0, 100, 0);
			hit_sphere.radius = 50.0f;
			if (m_DrawHitBound)
				m_DebugRenderer.DrawSphere(hit_sphere.center, hit_sphere.radius);

			if (RBCollision::TestSphereWithCapsule(hit_sphere, m_AIPlayer->GetCollisionShape()))
			{
				if (m_AIPlayer->GetBehavior() != BHV_HitDown &&
					m_AIPlayer->GetBehavior() != BHV_Hit)
				{
					m_AIPlayer->SetBehavior(BHV_Hit);
				}
			}
		}

		if (m_Player->GetBehavior() == BHV_BackKick &&
			m_Player->GetBehaviorTime() > 0.1f &&
			m_Player->GetBehaviorTime() < 0.3f)
		{
			RBSphere hit_sphere;
			hit_sphere.center = m_Player->GetPosition() + m_Player->GetNodeTransform().GetForward() * 30 + RBVector3(0, 100, 0);
			hit_sphere.radius = 50.0f;
			if (m_DrawHitBound)
				m_DebugRenderer.DrawSphere(hit_sphere.center, hit_sphere.radius);

			if (RBCollision::TestSphereWithCapsule(hit_sphere, m_AIPlayer->GetCollisionShape()))
			{
				if (m_AIPlayer->GetBehavior() != BHV_HitDown)
				{
					RBVector3 relVec = hit_sphere.center - m_AIPlayer->GetPosition();
					relVec.y = 0.0f;
					RBVector3 playerForward = m_Player->GetNodeTransform().GetForward();
					if (playerForward.Dot(relVec) >= 0)
						relVec = -playerForward;
					relVec.Normalize();

					m_AIPlayer->SetPlayerRotation(RAD_TO_DEG(atan2f(relVec.x, relVec.z)));
					m_AIPlayer->SetBehavior(BHV_HitDown);
				}
			}
		}

		m_Player->PostUpdate(timer);

		char msg_buf[1024];
		RBVector3 playerPos = m_Player->GetPosition();
		float playerRot = m_Player->GetPlayerRotation();
		RBAnimationBlender& blender = m_Player->GetAnimationBlender();
		sprintf_s(msg_buf, 1024, "Player: (%f, %f, %f), rot: %f\n"
			"Blend From : %s\n"
			"Blend To   : %s\n"
			"Blend time : %f",
			playerPos.x, playerPos.y, playerPos.z, playerRot,
			blender.GetStartAnimation() ? blender.GetStartAnimation()->GetName().c_str() : "",
			blender.GetEndAnimation() ? blender.GetEndAnimation()->GetName().c_str() : "",
			blender.GetElapsedBlendTime());
		m_Text.SetText(msg_buf, RBColor(0, 1, 0));
	}

	// Update AI player
	m_AIPlayer->PreUpdate(timer);
	RBVector3 moveVec = RBVector3(0, 0, 0);
	moveVec += RBVector3(0, -1000.0f * timer.DeltaTime(), 0);
	m_AIPlayer->UpdateMovement(timer, moveVec);
	m_AIPlayer->PostUpdate(timer);

	RBSCRIPT.UpdateScriptableObjects();
}

void RebornFighterApp::RenderScene()
{
	RBRENDERER.SetSamplerState(0, RBSamplerState_Texture);
	RBRENDERER.SetSamplerState(2, RBSamplerState_ShadowDepthComparison);

	float width = static_cast<float>(RBRENDERER.GetClientWidth());
	float height = static_cast<float>(RBRENDERER.GetClientHeight());
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, width, height, 0.0f, 1.0f };


	SHADER_OBJECT_BUFFER cbObject;

	for (int pass = 0; pass < 2; pass++)
	{
		if (pass == 0)
		{
			ID3D11ShaderResourceView* nullSRV[] = { nullptr };
			RBRENDERER.D3DImmediateContext()->PSSetShaderResources(RBShadowMap::ShaderResourceSlot(), 1, nullSRV);

			m_ShadowMap.SetupRenderTarget();
		}
		else
		{
			RBRENDERER.SetRenderTargets();
			RBRENDERER.D3DImmediateContext()->RSSetViewports(1, &vp);

			ID3D11ShaderResourceView* shadowMapSRV[] = { m_ShadowMap.GetRenderTargetDepthSRV() };
			RBRENDERER.D3DImmediateContext()->PSSetShaderResources(RBShadowMap::ShaderResourceSlot(), 1, shadowMapSRV);
		}

		RBRENDERER.Clear();

		if (pass == 0)
		{
			RBFrustum frustum = m_ShadowMap.GetFrustum();
			m_Scene.RenderDepthPass(&frustum);
		}
		else
		{
			RBFrustum frustum = m_Camera.GetFrustum();
			m_Scene.Render(&frustum);
		}

		if (m_Player)
		{
			cbObject.worldMatrix = m_Player->GetNodeTransform();
			m_Scene.cbPerObject.UpdateContent(&cbObject);
			m_Scene.cbPerObject.ApplyToShaders();
			if (pass == 0)
			{
				m_Player->DrawDepthPass();
				m_AIPlayer->DrawDepthPass();
			}
			else
			{
				RBRENDERER.SetBlendState(RBBlend_AlphaBlending);
				m_Player->Draw();
				m_AIPlayer->Draw();
				RBRENDERER.SetBlendState(RBBlend_Opaque);
			}
		}
	}

	cbObject.worldMatrix = RBMatrix4x4::IDENTITY;
	m_Scene.cbPerObject.UpdateContent(&cbObject);
	m_Scene.cbPerObject.ApplyToShaders();
	m_DebugRenderer.Render();
	m_DebugRenderer.Reset();

	RBRENDERER.Clear(false);
	m_Text.Render();

	RBRENDERER.Present();
}

void RebornFighterApp::OnResize(int width, int height)
{
	m_Camera.SetAspectRatio((float)width / (float)height);
}
