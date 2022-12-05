#ifndef ___PRIMITIVE_H___
#define ___PRIMITIVE_H___

namespace Pirmitive
{
	//--- 定数
	#define NORMALIZATION_EPSILON float(1e-20f)	
	#define EPSILON float(1e-6f) // 許容誤差

	class Float3;			// float3
	typedef Float3 Point;	// 点
	class Vector3;			// 三次元ベクトル
	class Line;				// 直線
	class Segment;			// 線分
	class Sphere;			// 球
	class Capsule;			// カプセル
	class AABB;				// AABB
}

#endif //!___PRIMITIVE_H___