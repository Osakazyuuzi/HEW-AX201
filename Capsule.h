#ifndef ___CAPSULE_H___
#define ___CAPSULE_H___

#include "Primitive.h"
#include "Segment.h"
#include "Point.h"

namespace Primitive
{
	class Capsule
	{
	public:
		//--- 要素
		Segment s;
		float r;	// 半径

		// コンストラクタ
		Capsule() : r(0.5f){}
		Capsule(const Segment& s, float r) : s(s), r(r) {}
		Capsule(const Point& p1, const Point& p2, float r) : s(p1, p2), r(r){}
	};
}

#endif //!___CAPSULE_H___