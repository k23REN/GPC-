#pragma once

namespace MadCanyon::Simd
{
	//#ifdef MC_SSE_INTRINSICS_
	inline MVector MC_VECCALL VectorMultiplyAdd(MVector v1, MVector v2, MVector v3)
	{
		MVector vResult = _mm_mul_ps(v1, v2);
		return _mm_add_ps(vResult, v3);
	}
	inline MVector MC_VECCALL VectorNegativeMultiplySubtract(MVector V1,MVector V2,MVector V3)
	{
		MVector R = _mm_mul_ps(V1, V2);
		return _mm_sub_ps(V3, R);
	}
	inline MVector MC_VECCALL VectorLess(MVector v1, MVector v2)
	{
		return _mm_cmplt_ps(v1, v2);
	}
	inline MVector MC_VECCALL VectorSelect(MVector v1, MVector v2, MVector control)
	{
		MVector vTemp1 = _mm_andnot_ps(control, v1);
		MVector vTemp2 = _mm_and_ps(v2, control);
		return _mm_or_ps(vTemp1, vTemp2);
	}
	inline MVector MC_VECCALL VectorTrueInt()
	{
		MVectorI V = _mm_set1_epi32(-1);
		return _mm_castsi128_ps(V);
	}
	inline MVector MC_VECCALL VectorEqual(MVector v1, MVector v2)
	{
		return _mm_cmpeq_ps(v1, v2);
	}
	inline MVector MC_VECCALL VectorLessOrEqual(MVector V1,MVector V2)
	{
		return _mm_cmple_ps(V1, V2);
	}
	inline MVector MC_VECCALL VectorEqualInt(MVector v1, MVector v2)
	{
		MVectorI V = _mm_cmpeq_epi32(_mm_castps_si128(v1), _mm_castps_si128(v2));
		return _mm_castsi128_ps(V);
	}
	inline bool MC_VECCALL Vector3Equal(MVector v1, MVector v2)
	{
		MVector vTemp = _mm_cmpeq_ps(v1, v2);
		return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
	}
	inline bool MC_VECCALL Vector3IsInfinite(MVector v)
	{
		// Mask off the sign bit
		__m128 vTemp = _mm_and_ps(v, Simd::MV_AbsMask);
		// Compare to infinity
		vTemp = _mm_cmpeq_ps(vTemp, Simd::MV_Infinity);
		// If x,y or z are infinity, the signs are true.
		return ((_mm_movemask_ps(vTemp) & 7) != 0);
	}
	inline MVector MC_VECCALL VectorIsInfinite(MVector v)
	{
		// 符号ビットをマスク
		MVector vTemp = _mm_and_ps(v, DirectX::g_XMAbsMask);
		// Compare to infinity
		vTemp = _mm_cmpeq_ps(vTemp, DirectX::g_XMInfinity);
		// If any are infinity, the signs are true.
		return vTemp;
	}
	inline MVector MC_VECCALL VectorOrInt(MVector v1, MVector v2)
	{
		MVectorI V = _mm_or_si128(_mm_castps_si128(v1), _mm_castps_si128(v2));
		return _mm_castsi128_ps(V);
	}
	inline MVector MC_VECCALL VectorAbs(MVector v)
	{
		MVector vResult = _mm_setzero_ps();
		vResult = _mm_sub_ps(vResult, v);
		vResult = _mm_max_ps(vResult, v);
		return vResult;
	}
	inline MVector MC_VECCALL VectorRound(MVector v)
	{
		MVector sign = _mm_and_ps(v, DirectX::g_XMNegativeZero);
		MVector sMagic = _mm_or_ps(DirectX::g_XMNoFraction, sign);
		MVector R1 = _mm_add_ps(v, sMagic);
		R1 = _mm_sub_ps(R1, sMagic);
		MVector R2 = _mm_and_ps(v, DirectX::g_XMAbsMask);
		MVector mask = _mm_cmple_ps(R2, DirectX::g_XMNoFraction);
		R2 = _mm_andnot_ps(mask, v);
		R1 = _mm_and_ps(R1, mask);
		MVector vResult = _mm_xor_ps(R1, R2);
		return vResult;

	}
	inline MVector MC_VECCALL VectorModAngles(MVector angles)
	{
		// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
		MVector vResult = _mm_mul_ps(angles, DirectX::g_XMReciprocalTwoPi);
		// Use the inline function due to complexity for rounding
		vResult = VectorRound(vResult);
		vResult = _mm_mul_ps(vResult, DirectX::g_XMTwoPi);
		vResult = _mm_sub_ps(angles, vResult);
		return vResult;
	}
	inline MVector MC_VECCALL VectorSin(MVector v)
	{
		// Force the value within the bounds of pi
		MVector x = VectorModAngles(v);

		// Map in [-pi/2,pi/2] with sin(y) = sin(x).
		MVector sign = _mm_and_ps(x, DirectX::g_XMNegativeZero);
		MVector c = _mm_or_ps(DirectX::g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
		MVector absx = _mm_andnot_ps(sign, x);  // |x|
		MVector rflx = _mm_sub_ps(c, x);
		MVector comp = _mm_cmple_ps(absx, DirectX::g_XMHalfPi);
		MVector select0 = _mm_and_ps(comp, x);
		MVector select1 = _mm_andnot_ps(comp, rflx);
		x = _mm_or_ps(select0, select1);

		MVector x2 = _mm_mul_ps(x, x);

		// Compute polynomial approximation
		const MVector SC1 = DirectX::g_XMSinCoefficients1;
		MVector vConstants = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
		MVector Result = _mm_mul_ps(vConstants, x2);

		const MVector SC0 = DirectX::g_XMSinCoefficients0;
		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);
		Result = _mm_add_ps(Result, DirectX::g_XMOne);
		Result = _mm_mul_ps(Result, x);
		return Result;

	}
	inline MVector MC_VECCALL VectorATan(MVector v)
	{
		MVector absV = VectorAbs(v);
		MVector invV = _mm_div_ps(DirectX::g_XMOne, v);
		MVector comp = _mm_cmpgt_ps(v, DirectX::g_XMOne);
		MVector select0 = _mm_and_ps(comp, DirectX::g_XMOne);
		MVector select1 = _mm_andnot_ps(comp, DirectX::g_XMNegativeOne);
		MVector sign = _mm_or_ps(select0, select1);
		comp = _mm_cmple_ps(absV, DirectX::g_XMOne);
		select0 = _mm_and_ps(comp, DirectX::g_XMZero);
		select1 = _mm_andnot_ps(comp, sign);
		sign = _mm_or_ps(select0, select1);
		select0 = _mm_and_ps(comp, v);
		select1 = _mm_andnot_ps(comp, invV);
		MVector x = _mm_or_ps(select0, select1);

		MVector x2 = _mm_mul_ps(x, x);

		// Compute polynomial approximation
		const MVector TC1 = DirectX::g_XMATanCoefficients1;
		MVector vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(3, 3, 3, 3));
		MVector Result = _mm_mul_ps(vConstants, x2);

		vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		const MVector TC0 = DirectX::g_XMATanCoefficients0;
		vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);
		Result = _mm_add_ps(Result, DirectX::g_XMOne);
		Result = _mm_mul_ps(Result, x);
		MVector result1 = _mm_mul_ps(sign, DirectX::g_XMHalfPi);
		result1 = _mm_sub_ps(result1, Result);

		comp = _mm_cmpeq_ps(sign, DirectX::g_XMZero);
		select0 = _mm_and_ps(comp, Result);
		select1 = _mm_andnot_ps(comp, result1);
		Result = _mm_or_ps(select0, select1);
		return Result;
	}
	inline MVector MC_VECCALL VectorATan2(MVector y, MVector x)
	{
		static const MVector ATan2Constants = { MC_PI, MC_PIDIV2, MC_PIDIV4,MC_PI * 3.0f / 4.0f };

		MVector Zero = _mm_setzero_ps();
		MVector ATanResultValid = VectorTrueInt();

		MVector Pi = _mm_shuffle_ps(ATan2Constants, ATan2Constants, _MM_SHUFFLE(0, 0, 0, 0));
		MVector PiOverTwo = _mm_shuffle_ps(ATan2Constants, ATan2Constants, _MM_SHUFFLE(1, 1, 1, 1));
		MVector PiOverFour = _mm_shuffle_ps(ATan2Constants, ATan2Constants, _MM_SHUFFLE(2, 2, 2, 2));
		MVector ThreePiOverFour = _mm_shuffle_ps(ATan2Constants, ATan2Constants, _MM_SHUFFLE(3, 3, 3, 3));

		MVector YEqualsZero = VectorEqual(y, Zero);
		MVector XEqualsZero = VectorEqual(x, Zero);
		MVector XIsPositive = _mm_and_ps(x, DirectX::g_XMNegativeZero.v);
		XIsPositive = VectorEqualInt(XIsPositive, Zero);
		MVector YEqualsInfinity = VectorIsInfinite(y);
		MVector XEqualsInfinity = VectorIsInfinite(x);

		MVector YSign = _mm_and_ps(y, DirectX::g_XMNegativeZero.v);
		Pi = VectorOrInt(Pi, YSign);
		PiOverTwo = VectorOrInt(PiOverTwo, YSign);
		PiOverFour = VectorOrInt(PiOverFour, YSign);
		ThreePiOverFour = VectorOrInt(ThreePiOverFour, YSign);

		MVector R1 = VectorSelect(Pi, YSign, XIsPositive);
		MVector R2 = VectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
		MVector R3 = VectorSelect(R2, R1, YEqualsZero);
		MVector R4 = VectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
		MVector R5 = VectorSelect(PiOverTwo, R4, XEqualsInfinity);
		MVector Result = VectorSelect(R3, R5, YEqualsInfinity);
		ATanResultValid = VectorEqualInt(Result, ATanResultValid);

		MVector V = _mm_div_ps(y, x);

		MVector R0 = VectorATan(V);

		R1 = VectorSelect(Pi, DirectX::g_XMNegativeZero, XIsPositive);
		R2 = _mm_add_ps(R0, R1);

		return VectorSelect(Result, R2, ATanResultValid);

	}
	inline void MC_VECCALL VectorSinCos(MVector* sin, MVector* cos, MVector v)
	{
		assert(sin != nullptr);
		assert(cos != nullptr);

		// Force the value within the bounds of pi
		MVector x = VectorModAngles(v);

		// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
		MVector sign = _mm_and_ps(x, DirectX::g_XMNegativeZero);
		__m128 c = _mm_or_ps(DirectX::g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
		__m128 absx = _mm_andnot_ps(sign, x);  // |x|
		__m128 rflx = _mm_sub_ps(c, x);
		__m128 comp = _mm_cmple_ps(absx, DirectX::g_XMHalfPi);
		__m128 select0 = _mm_and_ps(comp, x);
		__m128 select1 = _mm_andnot_ps(comp, rflx);
		x = _mm_or_ps(select0, select1);
		select0 = _mm_and_ps(comp, DirectX::g_XMOne);
		select1 = _mm_andnot_ps(comp, DirectX::g_XMNegativeOne);
		sign = _mm_or_ps(select0, select1);

		__m128 x2 = _mm_mul_ps(x, x);

		// Compute polynomial approximation of sine
		const MVector SC1 = DirectX::g_XMSinCoefficients1;
		MVector vConstants = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
		MVector Result = _mm_mul_ps(vConstants, x2);

		const MVector SC0 = DirectX::g_XMSinCoefficients0;
		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);
		Result = _mm_add_ps(Result, DirectX::g_XMOne);
		Result = _mm_mul_ps(Result, x);
		*sin = Result;

		// Compute polynomial approximation of cosine
		const MVector CC1 = DirectX::g_XMCosCoefficients1;
		vConstants = XM_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_mul_ps(vConstants, x2);

		const MVector CC0 = DirectX::g_XMCosCoefficients0;
		vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);
		Result = _mm_add_ps(Result, DirectX::g_XMOne);
		Result = _mm_mul_ps(Result, sign);
		*cos = Result;
	}
	inline void MC_VECCALL ScalarSinCos(float* sin, float* cos, float value)
	{
		assert(sin);
		assert(cos);

		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = DirectX::XM_1DIV2PI * value;
		if (value >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}
		float y = value - DirectX::XM_2PI * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > DirectX::XM_PIDIV2)
		{
			y = DirectX::XM_PI - y;
			sign = -1.0f;
		}
		else if (y < -DirectX::XM_PIDIV2)
		{
			y = -DirectX::XM_PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*cos = sign * p;

	}

	//inline void MC_VECCALL VectorGetXPtr(float* x, MVector& v)
	inline void MC_VECCALL VectorGetXPtr(float* x, MVector v)
	{
		assert(x != nullptr);
		_mm_store_ss(x, v);
	}

	//inline float MC_VECCALL VectorGetX(MVector& V)
	inline float MC_VECCALL VectorGetX(MVector V)
	{
		return _mm_cvtss_f32(V);
	}
	//inline float MC_VECCALL VectorGetY(MVector& V)
	inline float MC_VECCALL VectorGetY(MVector V)
	{
		MVector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		return _mm_cvtss_f32(vTemp);
	}
	//inline float MC_VECCALL VectorGetZ(MVector& V)
	inline float MC_VECCALL VectorGetZ(MVector V)
	{
		MVector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		return _mm_cvtss_f32(vTemp);
	}
	//inline float MC_VECCALL VectorGetW(MVector& V)
	inline float MC_VECCALL VectorGetW(MVector V)
	{
		MVector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
		return _mm_cvtss_f32(vTemp);
	}
	inline MVector MC_VECCALL VectorNegate(MVector V)
	{
		MVector Z;
		Z = _mm_setzero_ps();
		return _mm_sub_ps(Z, V);
	}
	inline MVector MC_VECCALL VectorScale(MVector V, float scaleFactor)
	{
		MVector vResult = _mm_set_ps1(scaleFactor);
		return _mm_mul_ps(vResult, V);
	}
	//#endif
}