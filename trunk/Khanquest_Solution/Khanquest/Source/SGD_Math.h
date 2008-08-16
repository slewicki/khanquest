////////////////////////////////////////////////
//	File	:		"SGD_Math.h"
//
//	Author	:		David Brown (DB)
//
//	Purpose	:		A math utility library for SGD.
////////////////////////////////////////////////
#pragma once

const float SGD_PI	=	3.141592653589732f;

//	Define vector	-	use for positions, movement, or calculations
struct tVector2D
{
	float fX;
	float fY;
	
	tVector2D operator+(const tVector2D& v)
	{
		tVector2D r = { fX + v.fX, fY + v.fY };
		return r;
	}

	tVector2D operator-(const tVector2D& v)
	{
		tVector2D r = { fX - v.fX, fY - v.fY };
		return r;
	}

	//	Scalar (multiply)
	//	v * 5, 5 * v
	tVector2D operator*(const float f)
	{
		tVector2D r = { fX * f, fY * f };
		return r;
	}

	tVector2D operator/(const float f)
	{
		tVector2D r = { fX / f, fY / f };
		return r;
	}
};

//	Find the length of a vector (magnitude)
float Vector2DLength(tVector2D vec);

//	Find the dot product
float DotProduct(tVector2D v1, tVector2D v2);

//	Normalize a vector
tVector2D Vector2DNormalize(tVector2D vec);

//	Rotate a vector
tVector2D Vector2DRotate(tVector2D vec, float fRadians);

//	Find the angle between 2 vectors
float AngleBetweenVectors(tVector2D v1, tVector2D v2);

//	Linear Interpolation
//	v = a + ((b - a) * t)
float Lerp(float fStart, float fEnd, float fPercent);

//	Tests if two float are approximately equal given a margin of error
bool FloatEquals(float fResult, float fExpected, float fEpsilon = 0.001f);

//	Determines which way to turn (clockwise or counter-clockwise)
float Steering(tVector2D vOrientation, tVector2D vToTarget);
