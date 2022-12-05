#ifndef ___SPHERE_H___
#define ___SPHERE_H___

#include "Primitive.h"
#include "Point.h"

namespace Primitive
{
	class Sphere
	{
	public:
		//--- �v�f
		Point p;	// ���S
		float r;	// ���a

		// �R���X�g���N�^
		Sphere() : p({ 0.0f, 0.0f, 0.0f }), r(0.5f) {}
		Sphere(const Point& p, float r) : p(p), r(r) {}
	};
}

#endif //!___SPHERE_H___