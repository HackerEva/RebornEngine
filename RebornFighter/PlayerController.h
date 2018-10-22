#ifndef _PLAYERCONTROLLER_H
#define _PLAYERCONTROLLER_H
#include "RBEngine.h"

enum PlayerAnimation
{
	PlayerAnim_Idle,
	PlayerAnim_Run,
	PlayerAnim_Punch1,
	PlayerAnim_Kick,
	PlayerAnim_BackKick,
	PlayerAnim_SpinAttack,

	PlayerAnim_Hit,
	PlayerAnim_Down,
	PlayerAnim_GetUp,

	PlayerAnimCount,
};

enum PlayerBehavior
{
	BHV_Idle,
	BHV_Running,
	BHV_Punch,
	BHV_Kick,
	BHV_BackKick,
	BHV_SpinAttack,
	BHV_Hit,
	BHV_HitDown,
	BHV_GetUp,
};

struct BehaviorInfo
{
	PlayerAnimation anim;
	float			blendTime;
};

class PlayerController : public RBSceneMeshObject
{
public:
	PlayerController();

	void Cache();

	void PreUpdate(const RBTimer& timer);
	const RBVector3& GetRootOffset() const { return m_RootOffset; }
	void UpdateMovement(const RBTimer& timer, const RBVector3 moveVec);
	void PostUpdate(const RBTimer& timer);

	void Draw();
	void DrawDepthPass();

	void SetPlayerRotation(float rot) { m_Rotation = rot; }
	float GetPlayerRotation() const { return m_Rotation; }

	void SetBehavior(PlayerBehavior behavior);
	PlayerBehavior GetBehavior() const;
	float GetBehaviorTime();

	RBAABB GetMovementCollisionShape() const;
	RBCapsule GetCollisionShape() const;

	RBAnimationBlender& GetAnimationBlender() { return m_AnimBlender; }
private:
	RBAnimation * LoadAnimation(const char* resPath, int flags = 0);

	float					m_Rotation;
	RBAnimation*			m_Animations[PlayerAnimCount];
	RBAnimationBlender		m_AnimBlender;
	RBVector3				m_RootOffset;
	RBMatrix4x4				m_BoneMatrices[MAX_BONE_COUNT];

	PlayerBehavior			m_Behavior;
};


#endif
