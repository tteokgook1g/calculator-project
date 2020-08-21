//This header file defines about class real_number(math).

#pragma once
#ifndef CALCULATOR_PROJECT_REAL_NUMBER_H_
#define CALCULATOR_PROJECT_REAL_NUMBER_H_

#include <iostream>
#include <string>
#include "base.h"

REAL_NUMBER_BEGIN

//This class is about real number.
class Real {
protected:
	//data
	//true is -,false is +,0 is +
	bool sign_;
	short* data_int_;
	short* data_dec_;
	base::MySize_t reserved_size_;
	bool infinity_bit_;

	explicit Real(const unsigned long long& _Val, bool _Sign, bool _Check2) noexcept;
	//Error occurs when someone tries to reference out of range.
	short& operator[](const int index) const;
public:
	//flag values
	static const int return_INFINITY = base::kMaxSizeVal + 1;
	static const int return_NaN = base::kMaxSizeVal + 2;


	//constructor
	Real() noexcept;
	explicit Real(bool _Sign, bool _INFINITYbit) noexcept;
	Real(const std::string& _Val) noexcept;
	Real(const bool _Val) noexcept;
	Real(const short _Val) noexcept;
	Real(const int _Val) noexcept;
	Real(const long _Val) noexcept;
	Real(const long long& _Val) noexcept;
	Real(const unsigned short _Val) noexcept;
	Real(const unsigned int _Val) noexcept;
	Real(const unsigned long _Val) noexcept;
	Real(const unsigned long long& _Val) noexcept;
	Real(const float _Val) noexcept;
	Real(const double& _Val) noexcept;
	Real(const long double& _Val) noexcept;
	Real(const char* const _Val) noexcept;
	Real(const std::nullptr_t& _nullptr) noexcept;
	Real(const Real& _Val) noexcept;
	Real(Real&& _Val) noexcept;

	~Real();

	//Reserves places that datas will be saved.
	void Reserve(const base::MySize_t _Size) noexcept;
	//Reserves double of current.
	void Sizeup() noexcept;

	//Check how many digit are saved in integer place.
	int DigitInt() const noexcept;
	//Check how many digit are saved in integer place.
	int DigitDec() const noexcept;
	//Check current reserved size.
	int ReservedSizeCurrent() const noexcept;
	//Check the highest digit.
	int DigitHighest() const noexcept;
	//Check whether it is infinity or not.
	bool IsInfinity() const noexcept;
	//Check whether it is not a number or a number.
	//Includes infinity.
	bool IsNan() const noexcept;

	const Real& operator=(const Real& _Val) noexcept;
	const Real& operator=(Real&& _Val) noexcept;
	const Real& operator+=(const Real& _Val) noexcept;
	const Real& operator-=(const Real& _Val) noexcept;
	const Real& operator*=(const Real& _Val) noexcept;
	const Real& operator/=(const Real& _Val) noexcept;
	const Real& operator<<=(const int _Val) noexcept;
	const Real& operator>>=(const int _Val) noexcept;

	const Real operator<<(const int index) const noexcept;
	const Real operator>>(const int index) const noexcept;
	const Real& operator++() noexcept;
	const Real operator++(int) noexcept;
	const Real& operator--() noexcept;
	const Real operator--(int) noexcept;
	const Real operator-() const noexcept;


	friend std::istream& operator>>(std::istream& _Istr, Real& _Val) noexcept;
	friend std::ostream& operator<<(std::ostream& _Ostr, const Real& _Val) noexcept;
	friend Real operator+(const Real& _Val1, const Real& _Val2);
	friend Real operator-(const Real& _Val1, const Real& _Val2);
	friend Real operator*(const Real& _Val1, const Real& _Val2);
	friend Real operator/(const Real& _Val1, const Real& _Val2);
	friend bool operator<(const Real& _Val1, const Real& _Val2) noexcept;
	friend bool operator>(const Real& _Val1, const Real& _Val2) noexcept;
	friend bool operator<=(const Real& _Val1, const Real& _Val2) noexcept;
	friend bool operator>=(const Real& _Val1, const Real& _Val2) noexcept;
	friend bool operator==(const Real& _Val1, const Real& _Val2) noexcept;
	friend bool operator!=(const Real& _Val1, const Real& _Val2) noexcept;


	explicit operator std::string() const noexcept;
	explicit operator bool() const noexcept;
	explicit operator short() const noexcept;
	explicit operator unsigned short() const noexcept;
	explicit operator int() const noexcept;
	explicit operator unsigned int() const noexcept;
	explicit operator long() const noexcept;
	explicit operator unsigned long() const noexcept;
	explicit operator long long() const noexcept;
	explicit operator unsigned long long() const noexcept;
	explicit operator float() const noexcept;
	explicit operator double() const noexcept;
	explicit operator long double() const noexcept;


	//If someone tries to reference out of range, it returns 0.
	short At(const int index) const;
	Real RoundOff(const int _digit = 0) noexcept;
	Real RoundUp(const int _digit = 0) noexcept;
	Real RoundDown(const int _digit = 0) noexcept;


	friend Real Power(const Real& base, const int index) noexcept;
	friend Real PowerOfTen(const int index) noexcept;
	friend Real SquareRoot(const Real& _Val) noexcept;
	friend Real PrintSquareRoot(const Real& _Val, std::ostream& _Ostr) noexcept;
	friend Real RoundOff(const Real& _Val, const int _digit) noexcept;
	friend Real RoundUp(const Real& _Val, const int _digit) noexcept;
	friend Real RoundDown(const Real& _Val, const int _digit) noexcept;
};

Real Power(const Real& base, const int index) noexcept;
Real PowerOfTen(const int index) noexcept;
Real SquareRoot(const Real& _Val) noexcept;
Real PrintSquareRoot(const Real& _Val, std::ostream& _Ostr = std::cout) noexcept;
Real RoundOff(const Real& _Val, const int _digit = 0) noexcept;
Real RoundUp(const Real& _Val, const int _digit = 0) noexcept;
Real RoundDown(const Real& _Val, const int _digit = 0) noexcept;

namespace real_number_value {
	const real_number::Real NaN{ nullptr };
	const real_number::Real infinity{ 0, 1 };
	const real_number::Real infinity_negative{ 1, 1 };
	const real_number::Real zero{};
}

REAL_NUMBER_END

#endif // !CALCULATOR_PROJECT_REAL_NUMBER_H_
