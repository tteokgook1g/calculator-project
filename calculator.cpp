#pragma once
#include "calculator.h"

CALCULATOR_BEGIN

Calculator::Calculator() noexcept :expression_() {}
Calculator::Calculator(const std::string& val) noexcept : expression_(val) {}
Calculator::Calculator(const mathematical_expression::Expression& val) noexcept
	: expression_(val) {}

void Calculator::Clear() noexcept {
	expression_ = mathematical_expression::Expression();
}
void Calculator::RoundOff(const int digit) noexcept {
	expression_ =
		mathematical_expression::Expression
		(static_cast<std::string>(expression_.Value().RoundOff(digit)));
}
void Calculator::RoundUp(const int digit) noexcept {
	expression_ =
		mathematical_expression::Expression
		(static_cast<std::string>(expression_.Value().RoundUp(digit)));

}
void Calculator::RoundDown(const int digit) noexcept {
	expression_ =
		mathematical_expression::Expression
		(static_cast<std::string>(expression_.Value().RoundDown(digit)));

}

std::istream& operator>>(std::istream& _Istr, Calculator& val) noexcept {
	std::string temp;
	std::getline(_Istr, temp);
	base::DeleteBlank(temp);
	if (temp.find("clear") != std::string::npos) {
		val.Clear();
		return _Istr;
	}
	if (temp.find("roundoff") != std::string::npos) {
		temp.erase(temp.find("roundoff"), 8);
		val.RoundOff(static_cast<int>(real_number::FixedReal(temp)));
		return _Istr;
	}
	if (temp.find("roundup") != std::string::npos) {
		temp.erase(temp.find("roundup"), 7);
		val.RoundUp(static_cast<int>(real_number::FixedReal(temp)));
		return _Istr;
	}
	if (temp.find("rounddown") != std::string::npos) {
		temp.erase(temp.find("rounddown"), 9);
		val.RoundDown(static_cast<int>(real_number::FixedReal(temp)));
		return _Istr;
	}
	if (temp.find("round") != std::string::npos) {
		temp.erase(temp.find("round"), 5);
		if (temp.length() == 0) {
			val.RoundOff(-3);
			return _Istr;
		}
		val.RoundOff(static_cast<int>(real_number::FixedReal(temp)));
		return _Istr;
	}
	if (val.expression_.Value() == 0) {
		if (base::IsNumber(temp.at(0))) {
			val.expression_ = mathematical_expression::Expression(temp);
			return _Istr;
		}
	}
	val.expression_ =
		mathematical_expression::Expression
		(static_cast<std::string>(val.expression_.Value()) + temp);
	return _Istr;
}
std::ostream& operator<<(std::ostream& _Ostr, const Calculator& val) noexcept {
	_Ostr << val.expression_;
	return _Ostr;
}


CALCULATOR_END
