#ifndef ___SEGMENT_H___
#define ___SEGMENT_H___

#include "Primitive.h"
#include "Line.h"

namespace Primitive
{
	class Segment : public Line
	{
	public:
		// �R���X�g���N�^
		Segment(){}
		Segment(const Point& p, const Vector3& v) : Line(p, v){}
		Segment(const Point& p1, const Point& p2) : Line(p1, p2-p1){}

		// �I�_���擾
		Float3 getEndPoint() const
		{
			return p + v;
		}
	};
}

#endif //!___SEGMENT_H___