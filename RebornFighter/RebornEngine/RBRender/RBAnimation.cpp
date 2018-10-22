#include "RBAnimation.h"
using namespace RebornEngine;
RBAnimationPlayer::RBAnimationPlayer()
	: IsAnimDone(false)
{
}

void RBAnimationPlayer::Proceed(float time)
{
	if (Animation && !IsAnimDone)
	{
		// Changing time may cause start time greater than end time
		if (CurrentTime >= Animation->GetEndTime() - 1)
		{
			CurrentTime = Animation->GetStartTime();
		}
		RBVector3 start_offset = Animation->GetRootPosition(CurrentTime);

		CurrentTime += time * Animation->GetFrameRate() * TimeScale;
		bool startOver = false;

		if (CurrentTime >= Animation->GetEndTime() - 1)
		{
			if (Animation->GetBitFlags() & RB_AnimBitFlag_Loop)
			{
				do
				{
					CurrentTime -= Animation->GetEndTime() - Animation->GetStartTime() - 1;
					startOver = true;
				} while (CurrentTime >= Animation->GetEndTime() - 1);
			}
			else
			{
				CurrentTime = Animation->GetEndTime() - 1;
				IsAnimDone = true;
			}
		}

		if (Animation->GetBitFlags() & RB_AnimBitFlag_HasRootMotion)
		{
			RootOffset = Animation->GetRootPosition(CurrentTime) - start_offset;
			if (startOver)
			{
				RootOffset = Animation->GetRootPosition(Animation->GetEndTime() - 1) - start_offset +
					Animation->GetRootPosition(CurrentTime) - Animation->GetInitRootPosition();
			}
		}
		else
		{
			RootOffset = RBVector3(0, 0, 0);
		}

	}

}

void RBAnimationPlayer::Reset()
{
	IsAnimDone = false;
	CurrentTime = Animation->GetStartTime();
}

RBAnimationBlender::RBAnimationBlender()
{
}

void RBAnimationBlender::Play(RBAnimation * anim, float time, float timeScale)
{
	m_BlendStartAnim.Animation = anim;
	m_BlendStartAnim.CurrentTime = time;
	m_BlendStartAnim.TimeScale = timeScale;
	m_BlendStartAnim.IsAnimDone = false;
	m_BlendEndAnim.Animation = nullptr;
}

void RBAnimationBlender::Play(RBAnimation * anim, float timeScale)
{
	Play(anim, anim->GetStartTime(), timeScale);
}

void RBAnimationBlender::Blend(RBAnimation * start, float startTime, float startTimeScale, RBAnimation * end, float endTime, float endTimeScale, float blendTime)
{
	m_BlendStartAnim.Animation = start;
	m_BlendStartAnim.CurrentTime = startTime;
	m_BlendStartAnim.TimeScale = startTimeScale;
	m_BlendStartAnim.IsAnimDone = false;

	m_BlendEndAnim.Animation = end;
	m_BlendEndAnim.CurrentTime = endTime;
	m_BlendEndAnim.TimeScale = endTimeScale;
	m_BlendEndAnim.IsAnimDone = false;

	m_BlendTime = blendTime;
	m_ElapsedBlendTime = 0.0f;
}

void RBAnimationBlender::BlendTo(RBAnimation * target, float targetTime, float targetTimeScale, float blendTime)
{
	if (m_BlendEndAnim.Animation)
	{
		m_BlendStartAnim.Animation = m_BlendEndAnim.Animation;
		m_BlendStartAnim.CurrentTime = m_BlendEndAnim.CurrentTime;
		m_BlendStartAnim.TimeScale = m_BlendEndAnim.TimeScale;
		m_BlendStartAnim.IsAnimDone = m_BlendEndAnim.IsAnimDone;
	}

	m_BlendEndAnim.Animation = target;
	m_BlendEndAnim.CurrentTime = targetTime;
	m_BlendEndAnim.TimeScale = targetTimeScale;
	m_BlendEndAnim.IsAnimDone = false;

	m_BlendTime = blendTime;
	m_ElapsedBlendTime = 0.0f;
}

void RBAnimationBlender::ProceedAnimation(float time)
{
	m_BlendStartAnim.Proceed(time);

	if (m_BlendEndAnim.Animation)
	{
		m_BlendEndAnim.Proceed(time);
		m_ElapsedBlendTime += time;
		if (m_ElapsedBlendTime >= m_BlendTime)
		{
			m_BlendStartAnim = m_BlendEndAnim;
			m_BlendEndAnim.Animation = nullptr;
		}
	}

}

void RBAnimationBlender::GetCurrentBlendedNodePose(int startNodeId, int endNodeId, RBMatrix4x4 * matrix)
{
	if (m_BlendStartAnim.Animation && m_BlendEndAnim.Animation)
	{
		RBMatrix4x4 mat1, mat2;
		m_BlendStartAnim.Animation->GetNodePose(startNodeId, m_BlendStartAnim.CurrentTime, &mat1);
		m_BlendEndAnim.Animation->GetNodePose(endNodeId, m_BlendEndAnim.CurrentTime, &mat2);

		// Apply inversed root translation
		if (m_BlendStartAnim.Animation->GetBitFlags() & RB_AnimBitFlag_HasRootMotion)
			mat1 *= RBMatrix4x4::CreateTranslation(-m_BlendStartAnim.Animation->GetRootPosition(m_BlendStartAnim.CurrentTime));
		if (m_BlendEndAnim.Animation->GetBitFlags() & RB_AnimBitFlag_HasRootMotion)
			mat2 *= RBMatrix4x4::CreateTranslation(-m_BlendEndAnim.Animation->GetRootPosition(m_BlendEndAnim.CurrentTime));

		float t = min(1.0f, m_ElapsedBlendTime / m_BlendTime);
		*matrix = RBMatrix4x4::Lerp(mat1, mat2, t);
	}
	else if (m_BlendStartAnim.Animation)
	{
		m_BlendStartAnim.Animation->GetNodePose(startNodeId, m_BlendStartAnim.CurrentTime, matrix);

		if (m_BlendStartAnim.Animation->GetBitFlags() & RB_AnimBitFlag_HasRootMotion)
			*matrix *= RBMatrix4x4::CreateTranslation(-m_BlendStartAnim.Animation->GetRootPosition(m_BlendStartAnim.CurrentTime));
	}

}

RBVector3 RBAnimationBlender::GetCurrentRootOffset()
{
	if (m_BlendStartAnim.Animation && m_BlendEndAnim.Animation)
	{
		float t = min(1.0f, m_ElapsedBlendTime / m_BlendTime);

		return RBVector3::Lerp(m_BlendStartAnim.RootOffset, m_BlendEndAnim.RootOffset, t);
	}
	else if (m_BlendStartAnim.Animation)
	{
		return m_BlendStartAnim.RootOffset;
	}

	return RBVector3::Zero();
}

bool RBAnimationBlender::IsAnimationDone()
{
	if (m_BlendEndAnim.Animation)
		return m_BlendEndAnim.IsAnimDone;
	return (m_BlendStartAnim.Animation && m_BlendStartAnim.IsAnimDone);
}

RBAnimation * RBAnimationBlender::GetStartAnimation()
{
	return m_BlendStartAnim.Animation;
}

float RBAnimationBlender::GetStartAnimationTime() const
{
	return m_BlendStartAnim.CurrentTime;
}

RBAnimation * RBAnimationBlender::GetEndAnimation()
{
	return m_BlendEndAnim.Animation;
}

float RBAnimationBlender::GetEndAnimationTime() const
{
	return m_BlendEndAnim.CurrentTime;
}

float RBAnimationBlender::GetElapsedBlendTime() const
{
	return m_ElapsedBlendTime;
}

RBAnimation::RBAnimation() 
	: m_Flags(0)
{
}

RBAnimation::RBAnimation(int nodeCount, int frameCount, float startTime, float endTime, float frameRate)
	: m_Flags(0), m_FrameCount(frameCount), m_StartTime(startTime), m_EndTime(endTime), m_FrameRate(frameRate), m_RootNode(-1)
{
}

RBAnimation::~RBAnimation()
{
	for (unsigned int i = 0; i < m_NodeKeyFrames.size(); i++)
	{
		if (m_NodeKeyFrames[i])
		{
			delete[] m_NodeKeyFrames[i];
		}
	}
}

void RBAnimation::Serialize(RBSerializer & serializer)
{
	if (!serializer.EnsureHeader("ANIM", 4))
		return;

	serializer.SerializeData(m_Name);
	serializer.SerializeData(m_Flags);
	serializer.SerializeData(m_FrameCount);
	serializer.SerializeData(m_StartTime);
	serializer.SerializeData(m_EndTime);
	serializer.SerializeData(m_FrameRate);
	serializer.SerializeData(m_RootNode);
	serializer.SerializeVector(m_NodeNames, &RBSerializer::SerializeData);
	serializer.SerializeVector(m_NodeParents);

	if (serializer.IsReading())
		m_NodeKeyFrames.resize(m_NodeNames.size());

	for (size_t i = 0; i < m_NodeKeyFrames.size(); i++)
	{
		serializer.SerializeArray(&m_NodeKeyFrames[i], m_FrameCount);
	}

}

void RBAnimation::AddNodePose(int nodeId, int frameId, const RBMatrix4x4 * matrix)
{
	assert(nodeId >= 0 && nodeId < (int)m_NodeKeyFrames.size());
	assert(frameId >= 0 && frameId < m_FrameCount);

	if (!m_NodeKeyFrames[nodeId])
	{
		m_NodeKeyFrames[nodeId] = new RBMatrix4x4[m_FrameCount];
	}

	memcpy(&m_NodeKeyFrames[nodeId][frameId], matrix, sizeof(RBMatrix4x4));
}

void RBAnimation::GetNodePose(int nodeId, float time, RBMatrix4x4 * matrix) const
{
	// Make zero based time
	time -= m_StartTime;

	assert(nodeId >= 0 && nodeId < (int)m_NodeKeyFrames.size());
	assert(time >= 0 && time < m_FrameCount);

	int frame1 = (int)time;
	int frame2 = ((int)time + 1) % m_FrameCount;
	//if (frame2 < frame1)
	//	frame2 = frame1;
	float t = time - frame1;

	// Linear interpolate two matrices
	// TODO: Use quaternion slerp and position lerp instead!
	for (int i = 0; i < 16; i++)
	{
		float va = ((float*)&m_NodeKeyFrames[nodeId][frame1])[i];
		float vb = ((float*)&m_NodeKeyFrames[nodeId][frame2])[i];
		((float*)matrix)[i] = va + (vb - va) * t;
	}
}

RBVector3 RBAnimation::GetInitRootPosition() const
{
	if (m_RootDisplacement.size() == 0)
		return RBVector3::Zero();

	return m_RootDisplacement[0];
}

RBVector3 RBAnimation::GetRootPosition(float time) const
{
	if (m_RootDisplacement.size() == 0)
		return RBVector3::Zero();

	// Make zero based time
	time -= m_StartTime;

	int frame1 = (int)time;
	int frame2 = ((int)time + 1) % m_FrameCount;
	//if (frame2 < frame1)
	//	frame2 = frame1;
	float t = time - frame1;

	RBVector3 va = m_RootDisplacement[frame1];
	RBVector3 vb = m_RootDisplacement[frame2];
	return RBVector3::Lerp(va, vb, t);
}

void RBAnimation::SetParentId(int nodeId, int parentId)
{
	m_NodeParents[nodeId] = parentId;
}

int RBAnimation::GetParentId(int nodeId) const
{
	return m_NodeParents[nodeId];
}

void RBAnimation::AddNodeNameToId(const char * nodeName, int nodeId)
{
	m_NodeNames[nodeId] = nodeName;
}

int RBAnimation::GetNodeIdByName(const char * nodeName) const
{
	for (vector<string>::const_iterator iter = m_NodeNames.begin(); iter != m_NodeNames.end(); iter++)
	{
		if (strcmp(iter->c_str(), nodeName) == 0)
			return (int)(iter - m_NodeNames.begin());
	}

	return -1;
}

void RBAnimation::BuildRootDisplacementArray()
{
	if (m_RootNode == -1)
		return;

	m_RootDisplacement.resize(m_FrameCount);

	for (int i = 0; i < m_FrameCount; i++)
	{
		m_RootDisplacement[i] = m_NodeKeyFrames[m_RootNode][i].GetTranslation();
		m_RootDisplacement[i].y = 0.0f;
	}
}
