#ifndef RBMATRIX_H
#define RBMATRIX_H

#include "RBVector.h"
namespace RebornEngine
{
	namespace RBMATH
	{
		class RBMatrix4x4
		{
		public:
			union
			{
				float m[4][4];
				struct {
					float _m11, _m12, _m13, _m14,
						_m21, _m22, _m23, _m24,
						_m31, _m32, _m33, _m34,
						_m41, _m42, _m43, _m44;
				};
			};

			RBMatrix4x4();

			RBMatrix4x4(float m11, float m12, float m13, float m14,
				float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34,
				float m41, float m42, float m43, float m44);

			RBMatrix4x4& operator=(const RBMatrix4x4& rhs);

			RBMatrix4x4 operator*(const RBMatrix4x4& rhs) const;
			RBMatrix4x4& operator*=(const RBMatrix4x4& rhs);

			// Fast inverse a homogenous transformation matrix
			RBMatrix4x4 FastInverse() const;

			RBVector3 GetForward() const;
			RBVector3 GetUp() const;
			RBVector3 GetRight() const;

			RBVector4 GetRow(int index) const;
			void SetRow(int index, const RBVector4& row);

			void Translate(const RBVector3& vec);
			void Translate(float x, float y, float z);
			void TranslateLocal(const RBVector3& vec);
			void TranslateLocal(float x, float y, float z);

			void SetTranslation(const RBVector3& vec);
			void SetTranslation(float x, float y, float z);
			RBVector3 GetTranslation() const;
			void GetTranslation(float& x, float& y, float& z) const;

			RBVector3 RotateVector(const RBVector3& vec) const;

			RBMatrix4x4 Inverse() const;

			// Identity matrix variable
			static RBMatrix4x4 IDENTITY;

			// Rotation matrix
			static RBMatrix4x4 CreateXAxisRotation(float degree);
			static RBMatrix4x4 CreateYAxisRotation(float degree);
			static RBMatrix4x4 CreateZAxisRotation(float degree);

			static RBMatrix4x4 CreateTranslation(const RBVector3& vec);
			static RBMatrix4x4 CreateTranslation(float x, float y, float z);

			static RBMatrix4x4 CreateScale(const RBVector3& scale);
			static RBMatrix4x4 CreateScale(float sx, float sy, float sz);

			static RBMatrix4x4 CreatePerspectiveProjectionLH(float fov, float aspect, float zNear, float zFar);
			static RBMatrix4x4 CreateOrthographicProjectionLH(float viewWidth, float viewHeight, float zNear, float zFar);
			static RBMatrix4x4 CreateLookAtViewLH(const RBVector3& eye, const RBVector3& lookAt, const RBVector3& up);

			static RBMatrix4x4 Lerp(const RBMatrix4x4& lhs, const RBMatrix4x4&, float t);
		};

		RBVector4 operator*(const RBVector4& v, const RBMatrix4x4& m);

	}

}

#endif