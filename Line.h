#ifndef ___LINE_H___
#define ___LINE_H___

#include "Primitive.h"
#include "Vector3.h"
#include "Point.h"

namespace Primitive
{
	class Line
	{
	public:
		//--- 要素
		Point p;
		Vector3 v;	// 方向ベクトル
		
		// コンストラクタ
		Line() : p({0.0f, 0.0f, 0.0f}), v({1.0f, 0.0f, 0.0f}){}
		Line(const Point& p, const Vector3& v) : p(p), v(v){}

		// 点上の座標を取得
		Point getPoint(float t) const
		{
			return p + t * v;
		}
	};
}

#endif //!___LINE_H___