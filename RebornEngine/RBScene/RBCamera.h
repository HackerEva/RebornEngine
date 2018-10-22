#ifndef RBCAMERA_H
#define RBCAMERA_H

#include "RBSceneObject.h"
namespace RebornEngine
{
	class RBCamera : public RBSceneObject
	{
	public:
		RBCamera();
		~RBCamera();

		RBSceneObjectType GetType() const { return RB_SO_Camera; }

		const RBMatrix4x4& GetViewMatrix();
		const RBMatrix4x4& GetProjectionMatrix();
		RBFrustum GetFrustum() const;

		void SetupView(float fov, float aspect, float _near, float _far);
		void SetAspectRatio(float aspect) { m_Aspect = aspect; m_DirtyProjMatrix = true; }

		float GetFOV() const { return m_FOV; }
		float GetAspectRatio() const { return m_Aspect; }
		float GetNearPlane() const { return m_Near; }
		float GetFarPlane() const { return m_Far; }

	private:
		RBMatrix4x4			m_ViewMatrix;
		RBMatrix4x4			m_ProjectionMatrix;

		float				m_FOV;
		float				m_Aspect;
		float				m_Near, m_Far;

		bool				m_DirtyProjMatrix;
	};

}

#endif