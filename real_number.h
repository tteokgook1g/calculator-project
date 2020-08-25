//This header file defines about class real_number(math).

#pragma once
#ifndef CALCULATOR_PROJECT_REAL_NUMBER_H_
#define CALCULATOR_PROJECT_REAL_NUMBER_H_

#include <iostream>
#include <string>
#include "base.h"

REAL_NUMBER_BEGIN

//This class is about real number.
//This class is fixed point.
class FixedReal {
protected:
	//data
	//true is -,false is +,0 is +
	bool sign_;
	short* data_int_;
	short* data_dec_;
	base::MySize_t reserved_size_;
	bool infinity_bit_;

	explicit FixedReal(const unsigned long long& _Val, bool _Sign, bool _Check2) noexcept;
	//Error occurs when someone tries to reference out of range.
	short& operator[](const int index) const;
public:
	//flag values
	static const int return_INFINITY = base::kMaxSizeVal + 1;
	static const int return_NaN = base::kMaxSizeVal + 2;


	//constructor
	FixedReal() noexcept;
	explicit FixedReal(bool _Sign, bool _INFINITYbit) noexcept;
	FixedReal(const std::string& _Val) noexcept;
	FixedReal(const bool _Val) noexcept;
	FixedReal(const short _Val) noexcept;
	FixedReal(const int _Val) noexcept;
	FixedReal(const long _Val) noexcept;
	FixedReal(const long long& _Val) noexcept;
	FixedReal(const unsigned short _Val) noexcept;
	FixedReal(const unsigned int _Val) noexcept;
	FixedReal(const unsigned long _Val) noexcept;
	FixedReal(const unsigned long long& _Val) noexcept;
	FixedReal(const float _Val) noexcept;
	FixedReal(const double& _Val) noexcept;
	FixedReal(const long double& _Val) noexcept;
	FixedReal(const char* const _Val) noexcept;
	FixedReal(const std::nullptr_t& _nullptr) noexcept;
	FixedReal(const FixedReal& _Val) noexcept;
	FixedReal(FixedReal&& _Val) noexcept;

	~FixedReal();

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
	inline bool IsInteger() const noexcept {
		return (data_dec_[0] == 0 && DigitDec() == 1);
	}
	inline bool IsEven() const noexcept {
		return (data_int_[0] == 1 || data_int_[0] == 3 ||
			data_int_[0] == 5 || data_int_[0] == 7 || data_int_[0] == 9);
	}
	inline bool IsOdd() const noexcept {
		return (data_int_[0] == 0 || data_int_[0] == 2 ||
			data_int_[0] == 4 || data_int_[0] == 6 || data_int_[0] == 8);
	}

	const FixedReal& operator=(const FixedReal& _Val) noexcept;
	const FixedReal& operator=(FixedReal&& _Val) noexcept;
	const FixedReal& operator+=(const FixedReal& _Val) noexcept;
	const FixedReal& operator-=(const FixedReal& _Val) noexcept;
	const FixedReal& operator*=(const FixedReal& _Val) noexcept;
	const FixedReal& operator/=(const FixedReal& _Val) noexcept;
	const FixedReal& operator<<=(const int _Val) noexcept;
	const FixedReal& operator>>=(const int _Val) noexcept;

	const FixedReal operator<<(const int index) const noexcept;
	const FixedReal operator>>(const int index) const noexcept;
	const FixedReal& operator++() noexcept;
	const FixedReal operator++(int) noexcept;
	const FixedReal& operator--() noexcept;
	const FixedReal operator--(int) noexcept;
	const FixedReal operator-() const noexcept;


	friend std::istream& operator>>(std::istream& _Istr, FixedReal& _Val) noexcept;
	friend std::ostream& operator<<(std::ostream& _Ostr, const FixedReal& _Val) noexcept;
	friend FixedReal operator+(const FixedReal& _Val1, const FixedReal& _Val2);
	friend FixedReal operator-(const FixedReal& _Val1, const FixedReal& _Val2);
	friend FixedReal operator*(const FixedReal& _Val1, const FixedReal& _Val2);
	friend FixedReal operator/(const FixedReal& _Val1, const FixedReal& _Val2);
	friend bool operator<(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator>(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator<=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator>=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator==(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator!=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;


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
	FixedReal RoundOff(const int _digit = 0) noexcept;
	FixedReal RoundUp(const int _digit = 0) noexcept;
	FixedReal RoundDown(const int _digit = 0) noexcept;


	friend FixedReal Power(const FixedReal& base, const FixedReal& index) noexcept;
	friend FixedReal PowerOfTen(const int index) noexcept;
	friend FixedReal SquareRoot(const FixedReal& _Val) noexcept;
	friend FixedReal PrintSquareRoot(const FixedReal& _Val, std::ostream& _Ostr) noexcept;
	friend FixedReal RoundOff(const FixedReal& _Val, const int _digit) noexcept;
	friend FixedReal RoundUp(const FixedReal& _Val, const int _digit) noexcept;
	friend FixedReal RoundDown(const FixedReal& _Val, const int _digit) noexcept;
};

FixedReal Power(const FixedReal& base, const FixedReal& index) noexcept;
FixedReal PowerOfTen(const int index) noexcept;
FixedReal SquareRoot(const FixedReal& _Val) noexcept;
FixedReal PrintSquareRoot(const FixedReal& _Val, std::ostream& _Ostr = std::cout) noexcept;
FixedReal RoundOff(const FixedReal& _Val, const int _digit = 0) noexcept;
FixedReal RoundUp(const FixedReal& _Val, const int _digit = 0) noexcept;
FixedReal RoundDown(const FixedReal& _Val, const int _digit = 0) noexcept;

FixedReal Factorial(const unsigned int _Val);

REAL_NUMBER_END

#endif // !CALCULATOR_PROJECT_REAL_NUMBER_H_
