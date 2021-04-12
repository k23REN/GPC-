#pragma once

namespace MadCanyon::Math
{
	/**
	* @brief �~����
	*/
	//template<class T = float>
	//constexpr T PI = static_cast<T>(std::numbers::pi);
	template<class T = float>
	constexpr T PI = static_cast<T>(3.141592653589793);
	/**
	* @brief �~���� * 2.0
	*/
	template<class T = float>
	constexpr T PI2 = static_cast<T>(PI<double> * 2.0);
	/**
	* @brief �~���� / 2.0
	*/
	template<class T = float>
	constexpr T PIHalf = static_cast<T>(PI<double> / 2.0);
	/**
	* @brief �~���� / 4.0
	*/
	template<class T = float>
	constexpr T PIQrt = static_cast<T>(PIHalf<double> / 2);

	/**
	* @brief �f�O���[�p�̈���̊p�x(360)
	*/
	template<class T = float>
	constexpr T DegRound = static_cast<T>(360.0);
	/**
	* @brief �f�O���[�p�̔����̊p�x(180)
	*/
	template<class T = float>
	constexpr T DegRoundHalf = static_cast<T>(DegRound<double> / 2.0);
	/**
	* @brief �f�O���[�p��4����1���̊p�x(90)
	*/
	template<class T = float>
	constexpr T DegRoundQtr = static_cast<T>(DegRoundHalf<double> / 2.0);

	/**
	* @brief ���W�A���p�̈���̊p�x(PI2)
	*/
	template<class T = float>
	constexpr T RadRound = PI2<T>;
	/**
	* @brief ���W�A���p�̔����̊p�x(PI)
	*/
	template<class T = float>
	constexpr T RadRoundHalf = PI<T>;
	/**
	* @brief ���W�A���p��4����1���̊p�x(PIHalf)
	*/
	template<class T = float>
	constexpr T RadRoundQtr = PIHalf<T>;

	/**
	* @brief �l�C�s�A��
	*/
	//template<class T = float>
	//constexpr T E = static_cast<T>(std::numbers::e);
	template<class T = float>
	constexpr T E = static_cast<T>(2.718281828459045);

	/**
	* @brief �f�O���[�p�֏�Z���鎖�Ń��W�A���p�֊��Z����܂�
	*/
	template<class T = float>
	constexpr T DegToRad = static_cast<T>(RadRound<> / DegRound<>);
	/**
	* @brief �f�O���[�p�֏�Z���鎖�Ń��W�A���p�֊��Z����܂�
	*/
	template<class T = float>
	constexpr T RadToDeg = static_cast<T>(DegRound<> / RadRound<>);

	/**
	* @brief ���������_�덷��Ԃ��܂�(1.0��1.0�̎��̒l�Ƃ̍�)
	*/
	template<class T = float>
	constexpr T Epsilon = std::numeric_limits<T>::epsilon();

	/**
	* @brief ���̖����\����Ԃ��܂�
	*/
	template<class T>
	constexpr T Infinity = std::numeric_limits<T>::infinity();
	/**
	* @brief �V�O�i���𔭐������Ȃ�NaN��Ԃ��܂�
	*/
	template<class T>
	constexpr T QuietNaN = std::numeric_limits<T>::quiet_NaN();
	/**
	* @brief �V�O�i���𔭐�������NaN��Ԃ��܂�
	*/
	template<class T>
	constexpr T SigQuietNaN = std::numeric_limits<T>::signaling_NaN();

	/**
	* @brief ���Z����Ɗ��Z���鎖���ł���
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
	* @brief ��Z����Ɗ��Z���鎖���ł���
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

	//TODO �ȉ��ʓr�t�@�C����(Math.h�Ȃǂ�).

	/**
	* @brief float�^���m�̔�r���s���A�u��v�����Ȃ�true��Ԃ��܂�
	* @note (value1 - epsilon) < value2 < (value1 + epsilon)�̎���true��Ԃ��܂�
	*/
	constexpr bool Approximately(float value1, float value2, float epsilon = Epsilon<>)
	{
		const float Value = value1 - value2;
		return (Value >= 0.0f ? Value : -Value) <= epsilon;
	}

	/**
	* @brief �l���w��͈̔͂Ɏ��߂�����Ԃ��܂�
	*/
	template<class T>
	constexpr T Clamp(T aValue, T aMin, T aMax) { return aValue <= aMin ? aMin : aValue >= aMax ? aMax : aValue; }

	//=====================================================

}