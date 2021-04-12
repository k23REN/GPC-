#pragma once

namespace MadCanyon::Math
{
	/**
	* @brief 円周率
	*/
	//template<class T = float>
	//constexpr T PI = static_cast<T>(std::numbers::pi);
	template<class T = float>
	constexpr T PI = static_cast<T>(3.141592653589793);
	/**
	* @brief 円周率 * 2.0
	*/
	template<class T = float>
	constexpr T PI2 = static_cast<T>(PI<double> * 2.0);
	/**
	* @brief 円周率 / 2.0
	*/
	template<class T = float>
	constexpr T PIHalf = static_cast<T>(PI<double> / 2.0);
	/**
	* @brief 円周率 / 4.0
	*/
	template<class T = float>
	constexpr T PIQrt = static_cast<T>(PIHalf<double> / 2);

	/**
	* @brief デグリー角の一周の角度(360)
	*/
	template<class T = float>
	constexpr T DegRound = static_cast<T>(360.0);
	/**
	* @brief デグリー角の半周の角度(180)
	*/
	template<class T = float>
	constexpr T DegRoundHalf = static_cast<T>(DegRound<double> / 2.0);
	/**
	* @brief デグリー角の4分の1周の角度(90)
	*/
	template<class T = float>
	constexpr T DegRoundQtr = static_cast<T>(DegRoundHalf<double> / 2.0);

	/**
	* @brief ラジアン角の一周の角度(PI2)
	*/
	template<class T = float>
	constexpr T RadRound = PI2<T>;
	/**
	* @brief ラジアン角の半周の角度(PI)
	*/
	template<class T = float>
	constexpr T RadRoundHalf = PI<T>;
	/**
	* @brief ラジアン角の4分の1周の角度(PIHalf)
	*/
	template<class T = float>
	constexpr T RadRoundQtr = PIHalf<T>;

	/**
	* @brief ネイピア数
	*/
	//template<class T = float>
	//constexpr T E = static_cast<T>(std::numbers::e);
	template<class T = float>
	constexpr T E = static_cast<T>(2.718281828459045);

	/**
	* @brief デグリー角へ乗算する事でラジアン角へ換算されます
	*/
	template<class T = float>
	constexpr T DegToRad = static_cast<T>(RadRound<> / DegRound<>);
	/**
	* @brief デグリー角へ乗算する事でラジアン角へ換算されます
	*/
	template<class T = float>
	constexpr T RadToDeg = static_cast<T>(DegRound<> / RadRound<>);

	/**
	* @brief 浮動小数点誤差を返します(1.0と1.0の次の値との差)
	*/
	template<class T = float>
	constexpr T Epsilon = std::numeric_limits<T>::epsilon();

	/**
	* @brief 正の無限表現を返します
	*/
	template<class T>
	constexpr T Infinity = std::numeric_limits<T>::infinity();
	/**
	* @brief シグナルを発生させないNaNを返します
	*/
	template<class T>
	constexpr T QuietNaN = std::numeric_limits<T>::quiet_NaN();
	/**
	* @brief シグナルを発生させるNaNを返します
	*/
	template<class T>
	constexpr T SigQuietNaN = std::numeric_limits<T>::signaling_NaN();

	/**
	* @brief 除算すると換算する事ができる
	* @note	100 / Kilo = 0.1(Killo)
	*/
	namespace Prefix {
		template<class T = float>
		constexpr T Milli = static_cast<T>(1.0 / 1000.0);
		template<class T = float>
		constexpr T Micro = static_cast<T>(Milli<T> / 1000.0);
		template<class T = float>
		constexpr T Nano = static_cast<T>(Micro<T> / 1000.0);
		template<class T = float>
		constexpr T Pico = static_cast<T>(Nano<T> / 1000.0);
		template<class T = uint32_t>
		constexpr T Kilo = static_cast<T>(1000);
		template<class T = uint32_t>
		constexpr T Mega = static_cast<T>(Kilo<T> * 1000);
		template<class T = uint32_t>
		constexpr T Giga = static_cast<T>(Mega<T> * 1000);
		template<class T = uint32_t>
		constexpr T Tera = static_cast<T>(Tera<T> * 1000);
	}
	/**
	* @brief 乗算すると換算する事ができる
	* @note 100 * Kilo = 0.1(Killo)
	*/
	namespace PrefixOfReciprocal {
		template<class T = uint32_t>
		constexpr T Milli = static_cast<T>(1.0 / (1.0 / 1000.0));
		template<class T = uint32_t>
		constexpr T Micro = static_cast<T>(1.0 / (Milli<T> / 1000.0));
		template<class T = uint32_t>
		constexpr T Nano = static_cast<T>(1.0 / (Micro<T> / 1000.0));
		template<class T = uint32_t>
		constexpr T Pico = static_cast<T>(1.0 / (Nano<T> / 1000.0));
		template<class T = float>
		constexpr T Kilo = static_cast<T>(1.0 / 1000);
		template<class T = float>
		constexpr T Mega = static_cast<T>(1.0 / (Kilo<T> * 1000));
		template<class T = float>
		constexpr T Giga = static_cast<T>(1.0 / (Mega<T> * 1000));
		template<class T = float>
		constexpr T Tera = static_cast<T>(1.0 / (Tera<T> * 1000));
	}


	//=====================================================

	//TODO 以下別途ファイルに(Math.hなどに).

	/**
	* @brief float型同士の比較を行い、「約」同じならtrueを返します
	* @note (value1 - epsilon) < value2 < (value1 + epsilon)の時にtrueを返します
	*/
	constexpr bool Approximately(float value1, float value2, float epsilon = Epsilon<>)
	{
		const float Value = value1 - value2;
		return (Value >= 0.0f ? Value : -Value) <= epsilon;
	}

	/**
	* @brief 値を指定の範囲に収めた数を返します
	*/
	template<class T>
	constexpr T Clamp(T aValue, T aMin, T aMax) { return aValue <= aMin ? aMin : aValue >= aMax ? aMax : aValue; }

	//=====================================================

}