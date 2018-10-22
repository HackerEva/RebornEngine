#include "RBCamera.h"

namespace RebornEngine
{
	RBCamera::RBCamera()
		: RBSceneObject(), m_DirtyProjMatrix(true)
	{
	}

	RBCamera::~RBCamera()
	{
	}

	const RBMatrix4x4 & RBCamera::GetViewMatrix()
	{
		m_ViewMatrix = m_NodeTransform.FastInverse();
		return m_ViewMatrix;
	}

	const RBMatrix4x4& RBCamera::GetProjectionMatrix()
	{
		if (m_DirtyProjMatrix)
		{
			m_ProjectionMatrix = RBMatrix4x4::CreatePerspectiveProjectionLH(m_FOV, m_Aspect, m_Near, m_Far);
			m_DirtyProjMatrix = false;
		}

		return m_ProjectionMatrix;
	}

	RBFrustum RBCamera::GetFrustum() const
	{
		RBFrustum frustum;

		RBVector3 nc = m_NodeTransform.GetTranslation() + m_NodeTransform.GetForward() * m_Near;
		RBVector3 fc = m_NodeTransform.GetTranslation() + m_NodeTransform.GetForward() * m_Far;

		float hNear = 2.0f * tanf(DEG_TO_RAD(m_FOV) / 2.0f) * m_Near;
		float hFar = 2.0f * tanf(DEG_TO_RAD(m_FOV) / 2.0f) * m_Far;
		float wNear = hNear * m_Aspect;
		float wFar = hFar * m_Aspect;
		frustum.corners[RB_FC_FTL] = fc + m_NodeTransform.GetUp() * (hFar * 0.5f) - m_NodeTransform.GetRight() * (wFar * 0.5f);
		frustum.corners[RB_FC_FTR] = fc + m_NodeTransform.GetUp() * (hFar * 0.5f) + m_NodeTransform.GetRight() * (wFar * 0.5f);
		frustum.corners[RB_FC_FBL] = fc - m_NodeTransform.GetUp() * (hFar * 0.5f) - m_NodeTransform.GetRight() * (wFar * 0.5f);
		frustum.corners[RB_FC_FBR] = fc - m_NodeTransform.GetUp() * (hFar * 0.5f) + m_NodeTransform.GetRight() * (wFar * 0.5f);
		frustum.corners[RB_FC_NTL] = nc + m_NodeTransform.GetUp() * (hNear * 0.5f) - m_NodeTransform.GetRight() * (wNear * 0.5f);
		frustum.corners[RB_FC_NTR] = nc + m_NodeTransform.GetUp() * (hNear * 0.5f) + m_NodeTransform.GetRight() * (wNear * 0.5f);
		frustum.corners[RB_FC_NBL] = nc - m_NodeTransform.GetUp() * (hNear * 0.5f) - m_NodeTransform.GetRight() * (wNear * 0.5f);
		frustum.corners[RB_FC_NBR] = nc - m_NodeTransform.GetUp() * (hNear * 0.5f) + m_NodeTransform.GetRight() * (wNear * 0.5f);

		frustum.BuildPlanesFromCorners();

		return frustum;
	}

	void RBCamera::SetupView(float fov, float aspect, float _near, float _far)
	{
		m_FOV = fov;
		m_Aspect = aspect;
		m_Near = _near;
		m_Far = _far;
		m_DirtyProjMatrix = true;
	}

}
