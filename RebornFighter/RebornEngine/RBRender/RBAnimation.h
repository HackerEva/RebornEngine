#ifndef RBANIMATION_H
#define RBANIMATION_H
#include "../RBMath/RBMath.h"
#include "../RBSystem/RBSerializer.h"
#include<string>
#include<vector>
namespace RebornEngine
{
	using std::string;
	using std::vector;
	//using std::s;
	using namespace RBMATH;
	enum RBAnimationBitFlag
	{
		RB_AnimBitFlag_Loop = 1 << 0,
		RB_AnimBitFlag_HasRootMotion = 1 << 1,
	};

	class RBAnimation;

	class RBAnimationPlayer
	{
	public:
		RBAnimationPlayer();

		RBAnimation*	Animation;
		float			CurrentTime;
		float			TimeScale;

		RBVector3		RootOffset;
		bool			IsAnimDone;

		void Proceed(float time);
		void Reset();
	};

	class RBAnimationBlender
	{
	public:
		RBAnimationBlender();

		void Play(RBAnimation* anim, float time, float timeScale);
		void Play(RBAnimation* anim, float timeScale = 1.0f);
		void Blend(RBAnimation* start, float startTime, float startTimeScale = 1.0f,
			RBAnimation* end = nullptr, float endTime = 0.0f, float endTimeScale = 0.0f,
			float blendTime = 0.0f);
		void BlendTo(RBAnimation* target, float targetTime, float targetTimeScale = 1.0f, float blendTime = 0.0f);

		void ProceedAnimation(float time);
		void GetCurrentBlendedNodePose(int startNodeId, int endNodeId, RBMatrix4x4* matrix);
		RBVector3 GetCurrentRootOffset();
		bool IsAnimationDone();
		RBAnimation* GetStartAnimation();
		float GetStartAnimationTime() const;
		RBAnimation* GetEndAnimation();
		float GetEndAnimationTime() const;
		float GetElapsedBlendTime() const;
	private:
		RBAnimationPlayer	m_BlendStartAnim;
		RBAnimationPlayer	m_BlendEndAnim;
		float				m_BlendTime;
		float				m_ElapsedBlendTime;
	};


	class RBAnimation
	{
	public:
		RBAnimation();
		RBAnimation(int nodeCount, int frameCount, float startTime, float endTime, float frameRate);
		~RBAnimation();

		void SetName(const string& name) { m_Name = name; }
		const string& GetName() const { return m_Name; }

		void SetBitFlags(int flags) { m_Flags = flags; }
		int GetBitFlags() const { return m_Flags; }

		void Serialize(RBSerializer& serializer);

		void AddNodePose(int nodeId, int frameId, const RBMatrix4x4* matrix);
		void GetNodePose(int nodeId, float time, RBMatrix4x4* matrix) const;
		int GetNodeCount() const { return (int)m_NodeKeyFrames.size(); }

		RBVector3 GetInitRootPosition() const;
		RBVector3 GetRootPosition(float time) const;

		void SetParentId(int nodeId, int parentId);
		int GetParentId(int nodeId) const;

		void AddNodeNameToId(const char* nodeName, int nodeId);
		int GetNodeIdByName(const char* nodeName) const;

		void SetRootNode(int nodeId) { m_RootNode = nodeId; }
		int GetRootNode() const { return m_RootNode; }
		void BuildRootDisplacementArray();

		float GetStartTime() const { return m_StartTime; }
		float GetEndTime() const { return m_EndTime; }
		float GetFrameRate() { return m_FrameRate; }
	private:
		string					m_Name;
		int						m_Flags;
		int						m_FrameCount;
		float					m_StartTime, m_EndTime, m_FrameRate;

		vector<string>			m_NodeNames;
		vector<int>				m_NodeParents;
		vector<RBMatrix4x4*>		m_NodeKeyFrames;

		vector<RBVector3>			m_RootDisplacement;
		int						m_RootNode;
	};

}

#endif
