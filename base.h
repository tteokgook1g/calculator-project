//This header file defines basic things about this project.

#pragma once
#ifndef CALCULATOR_PROJECT_BASE_H_
#define CALCULATOR_PROJECT_BASE_H_

#include <algorithm>
#include <array>
#include <iostream>
#include <string>

#ifndef NAMESPACE_DEFINE
#define NAMESPACE_DEFINE
#define BASE_BEGIN namespace base {
#define BASE_END }
#define REAL_NUMBER_BEGIN namespace real_number {
#define REAL_NUMBER_END }
#define MATHEMATICAL_EXPRESSION_BEGIN namespace mathematical_expression {
#define MATHEMATICAL_EXPRESSION_END }
#define CALCULATOR_BEGIN namespace calculator {
#define CALCULATOR_END }
#endif

BASE_BEGIN

using MySize_t = unsigned short;


//change the values according to the situation
//The "accuracy" is how many digits are found after the decimal point.
constexpr int kAccuracy = 100;
//Heap error occurs when something is allocated more than the "base::max_size_val."
constexpr int kMaxSizeVal = USHRT_MAX;


//Check that whether value is blank or not.
inline constexpr bool IsBlank(const char _Val) noexcept {
	if (_Val == ' ' || _Val == '	' || _Val == '\n') return true;
	return false;
}
//Change {,[ to ( and },] to )
inline void BracketToParenthesis(std::string& val) noexcept {
	for (size_t i = 0;i != val.length();i++) {
		switch (val.at(i))
		{
		case '[':
			val.replace(i, 1, "(");
			break;
		case ']':
			val.replace(i, 1, ")");
			break;
		case '{':
			val.replace(i, 1, "(");
			break;
		case '}':
			val.replace(i, 1, ")");
			break;
		}
	}
}
//Delete ','
inline void DeleteComma(std::string& val) noexcept {
	for (size_t i = 0;i != val.length();i++) {
		if (val.at(i) == ',') val.erase(i, 1);
	}
}
//Delete blanks
inline void DeleteBlank(std::string& val) noexcept {
	for (size_t i = 0;i != val.length();i++) {
		if (IsBlank(val.at(i))) val.erase(i, 1);
	}
}
//Change A,B,C... to a,b,c...
inline void CapitalToSmall(std::string& val) noexcept {
	for (size_t i = 0;i != val.length();i++) {
		if ('A' <= val.at(i) && val.at(i) <= 'Z') {
			val.replace(i, 1, std::string(1, val.at(i) - 'A' + 'a'));
		}
	}
}

//Check that whether char type value is number or not.
inline constexpr bool IsNumber(const char _Val) noexcept {
	return ('0' <= _Val && _Val <= '9');
}
inline bool IsNumber(const std::string& _Val) noexcept {
	std::string val(_Val);
	DeleteComma(val);
	bool exist_point = false;
	switch (val.at(0))
	{
	case '+':case '-':
		break;
	case '.':
		exist_point = true;
	default:
		if (IsNumber(val.at(0))) break;
		return false;
	}
	for (size_t i = 1;i != val.length();i++) {
		if (val.at(i) == '.') {
			if (exist_point) return false;
			else
			{
				exist_point = true;
				continue;
			}
		}
		if (!IsNumber(val.at(i))) return false;
	}
	return true;
}

//Calculate power of ten.
template<typename Ty>
inline constexpr Ty PowerOfTen(const int index) noexcept {
	Ty answer = 1;
	if (index > 308 || index < -308) return NULL;
	if (index > 0) {
		for (int i = 0;i < index;i++) {
			answer *= 10;
		}
	}
	else
	{
		for (int i = 0;i < -1 * index;i++) {
			answer /= 10;
		}
	}
	return answer;
}

//Calculate square root of under 100.
inline constexpr unsigned int SqrtUnder100(const unsigned int _Val) noexcept {
	if (_Val > 100) return NULL;
	if (_Val == 100) return 10;
	if (_Val >= 81) return 9;
	if (_Val >= 64) return 8;
	if (_Val >= 49) return 7;
	if (_Val >= 36) return 6;
	if (_Val >= 25)	return 5;
	if (_Val >= 16) return 4;
	if (_Val >= 9) return 3;
	if (_Val >= 4) return 2;
	if (_Val >= 1) return 1;
	return 0;
}

template<typename Ty>
inline constexpr Ty& Max(const Ty& _Val1, const Ty& _Val2) noexcept {
	return std::max<Ty>(_Val1, _Val2);
}
template<typename Ty, typename ...Tys>
inline constexpr Ty& Max(const Ty& _Val1, const Ty& _Val2, const Tys&... _Vals) noexcept {
	return Max(Max(_Val1, _Val2), _Vals...);
}

template<typename Ty>
inline constexpr Ty& Min(const Ty& _Val1, const Ty& _Val2) noexcept {
	return std::min<Ty>(_Val1, _Val2);
}
template<typename Ty, typename ...Tys>
inline constexpr Ty& Min(const Ty& _Val1, const Ty& _Val2, const Tys&... _Vals) noexcept {
	return Min(Min(_Val1, _Val2), _Vals...);
}

BASE_END

#endif // !CALCULATOR_PROJECT_BASE_H_
