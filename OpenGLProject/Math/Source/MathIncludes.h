#pragma once

/**
* @file MathIncludes.h
* @brief Math関連のすべてのヘッダをインクルード
*/

using MVector = __m128;
using MVectorI = __m128i;
#define MC_VECCALL __vectorcall


#include "MathConstants.h"

#include "Simd/SimdMath.h"
#include "Simd/SimdMathVector.h"

#include "MathVector2.h"
#include "MathVector3.h"
#include "MathVector4.h"
#include "MathQuaternion.h"
#include "MathMatrix44.h"