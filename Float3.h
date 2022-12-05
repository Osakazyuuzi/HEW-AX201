//#pragma once
#ifndef ___FLOAT3_H___
#define ___FLOAT3_H___

#include <math.h>
#include <float.h>
#include "Primitive.h"
#include <DirectXMath.h>

namespace Primitive
{
	//--- クラス定義
	class Float3
	{
	public:
		//--- 要素
		float x, y, z;

		// デフォルトコンストラクタ
		Float3(){}
		// 引数付きコンストラクタ
		explicit Float3(float a)
			: x(a), y(a), z(a)
		{}
		// 引数付きコンストラクタ
		Float3(float nx, float ny, float nz)
			: x(nx), y(ny), z(nz)
		{}
		// 引数付きコンストラクタ
		Float3(const Float3& f)
			: x(f.x), y(f.y), z(f.z)
		{}

		//--- 演算子
		// =
		Float3 operator=(const Float3& p)
		{
			x = p.x;
			y = p.y;
			z = p.z;
			return *this;
		}
		// []
		float& operator[](unsigned int index)
		{
			return reinterpret_cast<float*>(this)[index];
		}
		// []
		const float& operator[](unsigned int index) const
		{
			return reinterpret_cast<const float*>(this)[index];
		}
		// ==
		bool operator==(const Float3& f) const
		{
			return x == f.x && y == f.y && z == f.z;
		}
		// !=
		bool operator!=(const Float3& f) const
		{
			return x != f.x || y != f.y || z != f.z;
		}
		// -符号
		Float3 operator-() const
		{
			return Float3(-x, -y, -z);
		}
		// +
		Float3 operator+(const Float3& f) const
		{
			return Float3(x + f.x, y + f.y, z + f.z);
		}
		// -
		Float3 operator-(const Float3& f) const
		{
			return Float3(x - f.x, y - f.y, z - f.z);
		}
		// *
		Float3 operator*(float f) const
		{
			return Float3(x * f, y * f, z * f);
		}
		// /
		Float3 operator/(float f) const
		{
			f = 1.0f / f;
			return Float3(x * f, y * f, z * f);
		}
		// += 
		Float3& operator+=(const Float3& f)
		{
			x += f.x;
			y += f.y;
			z += f.z;
			return *this;
		}
		// -= 
		Float3& operator-=(const Float3& f)
		{
			x -= f.x;
			y -= f.y;
			z -= f.z;
			return *this;
		}
		// *= 
		Float3& operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		// /= 
		Float3& operator/=(float f)
		{
			f = 1.0f / f;
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		// 要素が全て 0 かどうか
		bool isZero() const
		{
			return x == 0.0f && y == 0.0f && z == 0.0f;
		}
		// 有限かどうか
		bool isFinite() const
		{
			return (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(x)))
				&& (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(y)))
				&& (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(z)));
		}
		// 正規化が出来るかどうか
		bool isNormalized() const
		{
			const float unitTolerance = 1e-4f;
			return isFinite() && fabs(magnitude() - 1) < unitTolerance;
		}
		// ベクトルの長さの二乗
		float magnitudeSquared()const
		{
			return x * x + y * y + z * z;
		}
		// ベクトルの長さ
		float magnitude() const
		{
			return sqrtf(magnitudeSquared());
		}
		// 内積
		float dot(const Float3& f) const
		{
			return x * f.x + y * f.y + z * f.z;
		}
		// 外積
		Float3 cross(const Float3& f) const
		{
			return Float3(y * f.z - z * f.y, z * f.x - x * f.z, x * f.y - y * f.x);
		}
		// 正規化した値を取得
		Float3 GetNormalized() const
		{
			const float m = magnitudeSquared();
			return m > 0.0f ? *this * (1.0f / sqrt(m)) : Float3(0, 0, 0);
		}
		// 正規化
		float normalize()
		{
			const float mag = magnitude();
			if (mag < NORMALIZATION_EPSILON)
				return 0.0f;
			*this *= 1.0f / mag;
			return mag;
		}
	};

	//--- 演算子
	// * 
	static Float3 operator*(float f, const Float3& f3)
	{
		return Float3(f * f3.x, f * f3.y, f * f3.z);
	}

	// DirectX::XMFLOAT3からFloat3への変換
	static Float3 ConvertToFloat3(DirectX::XMFLOAT3 f)
	{
		return Float3(f.x, f.y, f.z);
	}
	// Float3からDirectX::XMFLOAT3への変換
	static DirectX::XMFLOAT3 ConvertToDirectXFloat3(Float3 f)
	{
		return DirectX::XMFLOAT3(f.x, f.y, f.z);
	}
}

#endif //!___FLOAT3_H___