#ifndef ___VECTOR3_H___
#define ___VECTOR3_H___

#include "Primitive.h"
#include "Float3.h"

namespace Primitive
{
	class Vector3 : public Float3
	{
	public:
		// コンストラクタ
		Vector3() : Float3(){}
		Vector3(float a) : Float3(a){}
		Vector3(float nx, float ny, float nz) : Float3(nx, ny, nz){}
		Vector3(const Float3& f) : Float3(f){}

		// 垂直関係か？
		bool isVertical(const Vector3& v) const
		{
			float d = dot(v);
			return (-EPSILON < d && d < EPSILON);	// 許容誤差範囲内なら垂直と判定
		}

		// 平行関係か？
		bool isParallel(const Vector3& v) const
		{
			float d = cross(v).magnitudeSquared();
			return (-EPSILON < d && d < EPSILON);	// 許容誤差範囲内なら平行と判定
		}

		// 鋭角関係か？
		bool isSharpAngle(const Vector3& v) const
		{
			return (dot(v) >= 0.0f);
		}

	};

	// ワールド座標における上下左右や前後方向を表すベクトル
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