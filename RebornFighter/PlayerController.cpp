#include "PlayerController.h"

BehaviorInfo PlayerBehaviorInfo[] =
{
{ PlayerAnim_Idle,			0.2f },
{ PlayerAnim_Run,			0.2f },
{ PlayerAnim_Punch1,		0.0f },
{ PlayerAnim_Kick,			0.0f },
{ PlayerAnim_BackKick,		0.0f },
{ PlayerAnim_SpinAttack,	0.2f },
{ PlayerAnim_Hit,			0.0f },
{ PlayerAnim_Down,			0.0f },
{ PlayerAnim_GetUp,			0.0f },
};

PlayerController::PlayerController()
	: m_Rotation(0.0f), m_Behavior(BHV_Idle)
{

}

void PlayerController::Cache()
{
	RBMesh* playerMesh = RBResourceManager::Instance().FindMesh("Assets/unitychan/unitychan.fbx");
	if (!playerMesh)
		playerMesh = RBResourceManager::Instance().LoadFbxMesh("Assets/unitychan/unitychan.fbx", RB_RLM_Immediate);
	SetMesh(playerMesh);

	m_Animations[PlayerAnim_Idle] = LoadAnimation("Assets/unitychan/FUCM05_0000_Idle.fbx", RB_AnimBitFlag_Loop);
	m_Animations[PlayerAnim_Run] = LoadAnimation("Assets/unitychan/FUCM_0012b_EH_RUN_LP_NoZ.fbx", RB_AnimBitFlag_Loop);
	m_Animations[PlayerAnim_Punch1] = LoadAnimation("Assets/unitychan/FUCM05_0001_M_CMN_LJAB.fbx", RB_AnimBitFlag_HasRootMotion);
	m_Animations[PlayerAnim_Kick] = LoadAnimation("Assets/unitychan/FUCM_04_0001_RHiKick.fbx", RB_AnimBitFlag_HasRootMotion);
	m_Animations[PlayerAnim_BackKick] = LoadAnimation("Assets/unitychan/FUCM02_0004_CH01_AS_MAWAK.fbx", RB_AnimBitFlag_HasRootMotion);
	m_Animations[PlayerAnim_SpinAttack] = LoadAnimation("Assets/unitychan/FUCM02_0029_Cha01_STL01_ScrewK01.fbx", RB_AnimBitFlag_HasRootMotion);
	m_Animations[PlayerAnim_Hit] = LoadAnimation("Assets/unitychan/unitychan_DAMAGED00.fbx", RB_AnimBitFlag_HasRootMotion);
	m_Animations[PlayerAnim_Down] = LoadAnimation("Assets/unitychan/FUCM02_0025_MYA_TF_DOWN.fbx", RB_AnimBitFlag_HasRootMotion);
	m_Animations[PlayerAnim_GetUp] = LoadAnimation("Assets/unitychan/FUCM03_0019_HeadSpring.fbx", RB_AnimBitFlag_HasRootMotion);

	for (int i = 0; i < PlayerAnimCount; i++)
	{
		m_Mesh->CacheAnimation(m_Animations[i]);
	}

	m_AnimBlender.Play(m_Animations[PlayerAnim_Idle]);
}

void PlayerController::PreUpdate(const RBTimer& timer)
{
	m_AnimBlender.ProceedAnimation(timer.DeltaTime());

	m_RootOffset = m_AnimBlender.GetCurrentRootOffset();

	if (m_Behavior == BHV_Idle || m_Behavior == BHV_Running)
		m_RootOffset = RBVector3(0, 0, 0);

	if (m_AnimBlender.IsAnimationDone())
	{
		if (m_Behavior == BHV_HitDown)
			SetBehavior(BHV_GetUp);
		else
			SetBehavior(BHV_Idle);
	}
}

float LerpDegreeAngle(float from, float to, float t)
{
	while (from - to > 180.0f)
	{
		from -= 360.0f;
	}

	while (from - to < -180.0f)
	{
		from += 360.0f;
	}

	if (t < 0) t = 0;
	if (t > 1) t = 1;

	return from + (to - from) * t;
}

void PlayerController::UpdateMovement(const RBTimer& timer, const RBVector3 moveVec)
{
	RBVector3 hVec = moveVec;
	hVec.y = 0.0f;

	if (hVec.SquaredMagitude() > 0.0f)
	{
		hVec = hVec.GetNormalizedVec3();
		m_Rotation = LerpDegreeAngle(m_Rotation, RAD_TO_DEG(atan2f(hVec.x, hVec.z)), 10.0f * timer.DeltaTime());
	}

	static const RBVector3 StairOffset = RBVector3(0, 10, 0);

	RBAABB playerAabb = GetMovementCollisionShape();
	playerAabb.pMin += StairOffset;
	playerAabb.pMax += StairOffset;

	RBVector3 worldMoveVec = moveVec;
	worldMoveVec += (RBVector4(GetRootOffset(), 0) * GetNodeTransform()).ToVector3();
	worldMoveVec -= StairOffset;
	worldMoveVec = m_Scene->TestMovingAabbWithScene(playerAabb, worldMoveVec);

	Translate(worldMoveVec + StairOffset);

	SetRotation(RBMatrix4x4::CreateYAxisRotation(m_Rotation));
}

void PlayerController::PostUpdate(const RBTimer& timer)
{
	for (int i = 0; i < m_Mesh->GetBoneCount(); i++)
	{
		RBMatrix4x4 matrix;

		int boneId = m_Mesh->GetCachedAnimationNodeId(m_AnimBlender.GetStartAnimation(), i);
		int targetBondId = m_Mesh->GetCachedAnimationNodeId(m_AnimBlender.GetEndAnimation(), i);
		m_AnimBlender.GetCurrentBlendedNodePose(boneId, targetBondId, &matrix);

		m_BoneMatrices[i] = m_Mesh->GetBoneInitInvMatrices(i) * matrix * GetNodeTransform();
	}
}

void PlayerController::Draw()
{
	m_Scene->cbBoneMatrices.UpdateContent((SHADER_SKINNED_BUFFER*)&m_BoneMatrices);
	m_Scene->cbBoneMatrices.ApplyToShaders();

	RBSceneMeshObject::Draw();
}

void PlayerController::DrawDepthPass()
{
	m_Scene->cbBoneMatrices.UpdateContent((SHADER_SKINNED_BUFFER*)&m_BoneMatrices);
	m_Scene->cbBoneMatrices.ApplyToShaders();

	RBSceneMeshObject::DrawDepthPass();
}

void PlayerController::SetBehavior(PlayerBehavior behavior)
{
	switch (behavior)
	{
	case BHV_Kick:
		if (m_Behavior != BHV_Kick && m_Behavior != BHV_BackKick)
		{
			m_AnimBlender.Play(m_Animations[PlayerAnim_Kick]);
			m_Behavior = BHV_Kick;
		}
		else if (m_Behavior == BHV_Kick && GetBehaviorTime() >= 0.5f)
		{
			m_AnimBlender.Play(m_Animations[PlayerAnim_BackKick]);
			m_Behavior = BHV_BackKick;
		}
		break;

	case BHV_Hit:
		m_AnimBlender.Play(m_Animations[PlayerAnim_Hit]);
		m_Behavior = BHV_Hit;
		break;

	default:
		if (m_Behavior != behavior)
		{
			if (behavior == BHV_Idle)
			{
				int b = 0; b++;
			}

			if (PlayerBehaviorInfo[behavior].blendTime > 0.0f)
			{
				m_AnimBlender.BlendTo(m_Animations[PlayerBehaviorInfo[behavior].anim],
					m_Animations[PlayerBehaviorInfo[behavior].anim]->GetStartTime(), 1.0f,
					PlayerBehaviorInfo[behavior].blendTime);
			}
			else
			{
				m_AnimBlender.Play(m_Animations[PlayerBehaviorInfo[behavior].anim]);
			}
			m_Behavior = behavior;
		}
	}
}

PlayerBehavior PlayerController::GetBehavior() const
{
	return m_Behavior;
}

float PlayerController::GetBehaviorTime()
{
	if (m_AnimBlender.GetStartAnimation() && m_AnimBlender.GetEndAnimation())
	{
		return m_AnimBlender.GetEndAnimationTime() / m_AnimBlender.GetEndAnimation()->GetFrameRate();
	}
	else if (m_AnimBlender.GetStartAnimation())
	{
		return m_AnimBlender.GetStartAnimationTime() / m_AnimBlender.GetStartAnimation()->GetFrameRate();
	}

	return 0.0f;
}

RBAABB PlayerController::GetMovementCollisionShape() const
{
	RBAABB playerAabb;
	playerAabb.pMin = RBVector3(-50.0f, 0.0f, -50.0f) + GetPosition();
	playerAabb.pMax = RBVector3(50.0f, 150.0f, 50.0f) + GetPosition();

	return playerAabb;
}

RBCapsule PlayerController::GetCollisionShape() const
{
	return RBCapsule{ GetPosition() + RBVector3(0, 40, 0), GetPosition() + RBVector3(0, 110, 0), 40 };
}

RBAnimation* PlayerController::LoadAnimation(const char* resPath, int flags)
{
	RBMesh* mesh = RBResourceManager::Instance().FindMesh(resPath);
	if (!mesh)
		mesh = RBResourceManager::Instance().LoadFbxMesh(resPath, RB_RLM_Immediate);

	if (mesh)
	{
		RBAnimation* anim = mesh->GetAnimation();
		anim->SetBitFlags(flags);

		string strResPath = string(resPath);
		strResPath = strResPath.substr(strResPath.find_last_of('/') + 1);
		anim->SetName(strResPath);
		return anim;
	}

	return nullptr;
}