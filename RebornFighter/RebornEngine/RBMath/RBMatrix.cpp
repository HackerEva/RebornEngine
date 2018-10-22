//=============================================================================
// RMatrix.cpp by Shiyang Ao, 2016 All Rights Reserved.
//
// 
//=============================================================================

#include "RBMatrix.h"
#include <memory>

using namespace RebornEngine::RBMATH;
RBMatrix4x4 RBMatrix4x4::IDENTITY = RBMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

RBMatrix4x4::RBMatrix4x4()
{
}

RBMatrix4x4::RBMatrix4x4(float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
	: _m11(m11), _m12(m12), _m13(m13), _m14(m14),
	_m21(m21), _m22(m22), _m23(m23), _m24(m24),
	_m31(m31), _m32(m32), _m33(m33), _m34(m34),
	_m41(m41), _m42(m42), _m43(m43), _m44(m44)
{

}

RBMatrix4x4 RBMatrix4x4::operator*(const RBMatrix4x4& rhs) const
{
	RBMatrix4x4 mat;

	mat.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
	mat.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
	mat.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
	mat.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];

	mat.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
	mat.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
	mat.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
	mat.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];

	mat.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
	mat.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
	mat.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
	mat.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];

	mat.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
	mat.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
	mat.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
	mat.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];

	return mat;
}

RBMatrix4x4& RBMatrix4x4::operator=(const RBMatrix4x4& rhs)
{
	if (this != &rhs)
	{
		_m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13; _m14 = rhs._m14;
		_m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23; _m24 = rhs._m24;
		_m31 = rhs._m31; _m32 = rhs._m32; _m33 = rhs._m33; _m34 = rhs._m34;
		_m41 = rhs._m41; _m42 = rhs._m42; _m43 = rhs._m43; _m44 = rhs._m44;
	}

	return *this;
}

RBMatrix4x4& RBMatrix4x4::operator*=(const RBMatrix4x4& rhs)
{
	*this = *this * rhs;
	return *this;
}

RBMatrix4x4 RBMatrix4x4::FastInverse() const
{
	RBMatrix4x4 view = *this;

	// Swap row and column in 3x3 matrix
	std::swap(view.m[0][1], view.m[1][0]);
	std::swap(view.m[0][2], view.m[2][0]);
	std::swap(view.m[1][2], view.m[2][1]);

	float vx = view.m[3][0] * view.m[0][0] + view.m[3][1] * view.m[1][0] + view.m[3][2] * view.m[2][0];
	float vy = view.m[3][0] * view.m[0][1] + view.m[3][1] * view.m[1][1] + view.m[3][2] * view.m[2][1];
	float vz = view.m[3][0] * view.m[0][2] + view.m[3][1] * view.m[1][2] + view.m[3][2] * view.m[2][2];

	view.m[3][0] = -vx;
	view.m[3][1] = -vy;
	view.m[3][2] = -vz;

	return view;
}

RBVector3 RBMatrix4x4::GetForward() const
{
	return RBVector3(m[2][0], m[2][1], m[2][2]);
}

RBVector3 RBMatrix4x4::GetUp() const
{
	return RBVector3(m[1][0], m[1][1], m[1][2]);
}

RBVector3 RBMatrix4x4::GetRight() const
{
	return RBVector3(m[0][0], m[0][1], m[0][2]);
}

RBVector4 RBMatrix4x4::GetRow(int index) const
{
	return RBVector4(m[index]);
}

void RBMatrix4x4::SetRow(int index, const RBVector4& row)
{
	m[index][0] = row.x;
	m[index][1] = row.y;
	m[index][2] = row.z;
	m[index][3] = row.w;
}

void RBMatrix4x4::Translate(const RBVector3& vec)
{
	Translate(vec.x, vec.y, vec.z);
}

void RBMatrix4x4::Translate(float x, float y, float z)
{
	m[3][0] += x;
	m[3][1] += y;
	m[3][2] += z;
}

void RBMatrix4x4::TranslateLocal(const RBVector3& vec)
{
	TranslateLocal(vec.x, vec.y, vec.z);
}

void RBMatrix4x4::TranslateLocal(float x, float y, float z)
{
	float _x = m[3][0] + x * m[0][0] + y * m[1][0] + z * m[2][0];
	float _y = m[3][1] + x * m[0][1] + y * m[1][1] + z * m[2][1];
	float _z = m[3][2] + x * m[0][2] + y * m[1][2] + z * m[2][2];

	m[3][0] = _x;
	m[3][1] = _y;
	m[3][2] = _z;
}

void RBMatrix4x4::SetTranslation(const RBVector3& vec)
{
	SetTranslation(vec.x, vec.y, vec.z);
}

void RBMatrix4x4::SetTranslation(float x, float y, float z)
{
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
}

RBVector3 RBMatrix4x4::GetTranslation() const
{
	return RBVector3(m[3][0], m[3][1], m[3][2]);
}

void RBMatrix4x4::GetTranslation(float& x, float& y, float& z) const
{
	x = m[3][0];
	y = m[3][1];
	z = m[3][2];
}

RBVector3 RBMatrix4x4::RotateVector(const RBVector3& vec) const
{
	float _x = vec.x * m[0][0] + vec.y * m[1][0] + vec.z * m[2][0];
	float _y = vec.x * m[0][1] + vec.y * m[1][1] + vec.z * m[2][1];
	float _z = vec.x * m[0][2] + vec.y * m[1][2] + vec.z * m[2][2];

	return RBVector3(_x, _y, _z);
}

// Calculate the determinant of a 3x3 matrix
float Matrix_Determinant(float e_11, float e_12, float e_13,
	float e_21, float e_22, float e_23,
	float e_31, float e_32, float e_33)
{
	return e_11 * e_22 * e_33 + e_12 * e_23 * e_31 + e_13 * e_21 * e_32 -
		e_11 * e_23 * e_32 - e_12 * e_21 * e_33 - e_13 * e_22 * e_31;
}

// Calculate the determinal of a RBMatrix4x4
float Matrix_Determinant(const RBMatrix4x4& m)
{
	return m._m11 * Matrix_Determinant(m._m22, m._m23, m._m24, m._m32, m._m33, m._m34, m._m42, m._m43, m._m44) -
		m._m12 * Matrix_Determinant(m._m21, m._m23, m._m24, m._m31, m._m33, m._m34, m._m41, m._m43, m._m44) +
		m._m13 * Matrix_Determinant(m._m21, m._m22, m._m24, m._m31, m._m32, m._m34, m._m41, m._m42, m._m44) -
		m._m14 * Matrix_Determinant(m._m21, m._m22, m._m23, m._m31, m._m32, m._m33, m._m41, m._m42, m._m43);
}


RBMatrix4x4 RBMatrix4x4::Inverse() const
{
	float det = Matrix_Determinant(*this);
	if (FLT_EQUAL_ZERO(det))
		return *this;

	RBMatrix4x4 r;
	float inv_det = 1.0f / det;

	r._m11 = Matrix_Determinant(_m22, _m23, _m24, _m32, _m33, _m34, _m42, _m43, _m44) * inv_det;
	r._m12 = -Matrix_Determinant(_m12, _m13, _m14, _m32, _m33, _m34, _m42, _m43, _m44) * inv_det;
	r._m13 = Matrix_Determinant(_m12, _m13, _m14, _m22, _m23, _m24, _m42, _m43, _m44) * inv_det;
	r._m14 = -Matrix_Determinant(_m12, _m13, _m14, _m22, _m23, _m24, _m32, _m33, _m34) * inv_det;
	r._m21 = -Matrix_Determinant(_m21, _m23, _m24, _m31, _m33, _m34, _m41, _m43, _m44) * inv_det;
	r._m22 = Matrix_Determinant(_m11, _m13, _m14, _m31, _m33, _m34, _m41, _m43, _m44) * inv_det;
	r._m23 = -Matrix_Determinant(_m11, _m13, _m14, _m21, _m23, _m24, _m41, _m43, _m44) * inv_det;
	r._m24 = Matrix_Determinant(_m11, _m13, _m14, _m21, _m23, _m24, _m31, _m33, _m34) * inv_det;
	r._m31 = Matrix_Determinant(_m21, _m22, _m24, _m31, _m32, _m34, _m41, _m42, _m44) * inv_det;
	r._m32 = -Matrix_Determinant(_m11, _m12, _m14, _m31, _m32, _m34, _m41, _m42, _m44) * inv_det;
	r._m33 = Matrix_Determinant(_m11, _m12, _m14, _m21, _m22, _m24, _m41, _m42, _m44) * inv_det;
	r._m34 = -Matrix_Determinant(_m11, _m12, _m14, _m21, _m22, _m24, _m31, _m32, _m34) * inv_det;
	r._m41 = -Matrix_Determinant(_m21, _m22, _m23, _m31, _m32, _m33, _m41, _m42, _m43) * inv_det;
	r._m42 = Matrix_Determinant(_m11, _m12, _m13, _m31, _m32, _m33, _m41, _m42, _m43) * inv_det;
	r._m43 = -Matrix_Determinant(_m11, _m12, _m13, _m21, _m22, _m23, _m41, _m42, _m43) * inv_det;
	r._m44 = Matrix_Determinant(_m11, _m12, _m13, _m21, _m22, _m23, _m31, _m32, _m33) * inv_det;

	return r;
}


RBMatrix4x4 RBMatrix4x4::CreateXAxisRotation(float degree)
{
	RBMatrix4x4 mat = RBMatrix4x4::IDENTITY;
	mat.m[1][1] = cosf(DEG_TO_RAD(degree));
	mat.m[1][2] = sinf(DEG_TO_RAD(degree));
	mat.m[2][1] = -sinf(DEG_TO_RAD(degree));
	mat.m[2][2] = cosf(DEG_TO_RAD(degree));

	return mat;
}

RBMatrix4x4 RBMatrix4x4::CreateYAxisRotation(float degree)
{
	RBMatrix4x4 mat = RBMatrix4x4::IDENTITY;
	mat.m[0][0] = cosf(DEG_TO_RAD(degree));
	mat.m[0][2] = -sinf(DEG_TO_RAD(degree));
	mat.m[2][0] = sinf(DEG_TO_RAD(degree));
	mat.m[2][2] = cosf(DEG_TO_RAD(degree));

	return mat;
}

RBMatrix4x4 RBMatrix4x4::CreateZAxisRotation(float degree)
{
	RBMatrix4x4 mat = RBMatrix4x4::IDENTITY;
	mat.m[0][0] = cosf(DEG_TO_RAD(degree));
	mat.m[0][1] = sinf(DEG_TO_RAD(degree));
	mat.m[1][0] = -sinf(DEG_TO_RAD(degree));
	mat.m[1][1] = cosf(DEG_TO_RAD(degree));

	return mat;
}

RBMatrix4x4 RBMatrix4x4::CreateTranslation(const RBVector3& vec)
{
	return CreateTranslation(vec.x, vec.y, vec.z);
}

RBMatrix4x4 RBMatrix4x4::CreateTranslation(float x, float y, float z)
{
	RBMatrix4x4 mat = RBMatrix4x4::IDENTITY;
	mat.m[3][0] = x;
	mat.m[3][1] = y;
	mat.m[3][2] = z;

	return mat;
}

RBMatrix4x4 RBMatrix4x4::CreateScale(const RBVector3& scale)
{
	return CreateScale(scale.x, scale.y, scale.z);
}

RBMatrix4x4 RBMatrix4x4::CreateScale(float sx, float sy, float sz)
{
	RBMatrix4x4 mat = RBMatrix4x4::IDENTITY;
	mat.m[0][0] = sx;
	mat.m[1][1] = sy;
	mat.m[2][2] = sz;

	return mat;
}

RBMatrix4x4 RBMatrix4x4::CreatePerspectiveProjectionLH(float fov, float aspect, float zNear, float zFar)
{
	float y_scale = 1.0f / tanf(0.5f * DEG_TO_RAD(fov));

	return RBMatrix4x4(y_scale / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, y_scale, 0.0f, 0.0f,
		0.0f, 0.0f, zFar / (zFar - zNear), 1.0f,
		0.0f, 0.0f, -(zFar * zNear) / (zFar - zNear), 0.0f);
}

RBMatrix4x4 RBMatrix4x4::CreateOrthographicProjectionLH(float viewWidth, float viewHeight, float zNear, float zFar)
{
	float fRange = 1.0f / (zFar - zNear);

	return RBMatrix4x4(2.0f / viewWidth, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / viewHeight, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		0.0f, 0.0f, -fRange * zNear, 1.0f);
}

RBMatrix4x4 RBMatrix4x4::CreateLookAtViewLH(const RBVector3& eye, const RBVector3& lookAt, const RBVector3& up)
{
	RBVector3 zaxis = (lookAt - eye).GetNormalizedVec3();
	RBVector3 xaxis = up.Cross(zaxis).GetNormalizedVec3();
	RBVector3 yaxis = zaxis.Cross(xaxis);

	return RBMatrix4x4(xaxis.x, yaxis.x, zaxis.x, 0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		-xaxis.Dot(eye), -yaxis.Dot(eye), -zaxis.Dot(eye), 1);
}

RBMatrix4x4 RBMatrix4x4::Lerp(const RBMatrix4x4& lhs, const RBMatrix4x4& rhs, float t)
{
	RBMatrix4x4 result;

	for (int j = 0; j < 16; j++)
	{
		float va = ((float*)&lhs)[j];
		float vb = ((float*)&rhs)[j];
		((float*)&result)[j] = va + (vb - va) * t;
	}

	return result;
}

RBVector4 operator*(const RBVector4& v, const RBMatrix4x4& m)
{
	RBVector4 v_result = v;
	v_result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
	v_result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
	v_result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
	v_result.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
	return v_result;
}