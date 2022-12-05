#ifndef ___AABB_H___
#define ___AABB_H___

#include "Primitive.h"
#include "Float3.h"
#include "Point.h"

namespace Primitive
{
	class AABB
	{
	public:
		//--- 要素
		Point p;	// 中心点
		Float3 hl;	// 各軸の辺の長さの半分

		// コンストラクタ
		AABB(){}
		AABB(const Point& p, const Float3& hl) : p(p), hl(hl){}

		// 辺の長さを取得
		float lenX() { return hl.x * 2.0f; }
		float lenY() { return hl.y * 2.0f; }
		float lenZ() { return hl.z * 2.0f; }
		float len(int i) {
			return *((&hl.x) + i) * 2.0f;
		}
	};
}

#endif //!___AABB_H___