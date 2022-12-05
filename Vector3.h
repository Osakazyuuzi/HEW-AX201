#ifndef ___VECTOR3_H___
#define ___VECTOR3_H___

#include "Primitive.h"
#include "Float3.h"

namespace Primitive
{
	class Vector3 : public Float3
	{
	public:
		// �R���X�g���N�^
		Vector3() : Float3(){}
		Vector3(float a) : Float3(a){}
		Vector3(float nx, float ny, float nz) : Float3(nx, ny, nz){}
		Vector3(const Float3& f) : Float3(f){}

		// �����֌W���H
		bool isVertical(const Vector3& v) const
		{
			float d = dot(v);
			return (-EPSILON < d && d < EPSILON);	// ���e�덷�͈͓��Ȃ琂���Ɣ���
		}

		// ���s�֌W���H
		bool isParallel(const Vector3& v) const
		{
			float d = cross(v).magnitudeSquared();
			return (-EPSILON < d && d < EPSILON);	// ���e�덷�͈͓��Ȃ畽�s�Ɣ���
		}

		// �s�p�֌W���H
		bool isSharpAngle(const Vector3& v) const
		{
			return (dot(v) >= 0.0f);
		}

	};

	// ���[���h���W�ɂ�����㉺���E��O�������\���x�N�g��
	const static Vector3 Vector3_one		= { 1.0f,  1.0f,  1.0f };
	const static Vector3 Vector3_zero		= { 0.0f,  0.0f,  0.0f };
	const static Vector3 Vector3_up			= { 0.0f,  1.0f,  0.0f };
	const static Vector3 Vector3_down		= { 0.0f, -1.0f,  0.0f };
	const static Vector3 Vector3_right		= { 1.0f,  0.0f,  0.0f };
	const static Vector3 Vector3_left		= { -1.0f,  0.0f,  0.0f };
	const static Vector3 Vector3_forward	= { 0.0f,  0.0f,  1.0f };
	const static Vector3 Vector3_back		= { 0.0f,  0.0f, -1.0f };
}
#endif //!___VECTOR3_H___