//This header file defines about calculator class.

#pragma once
#ifndef CALCULATOR_PROJECT_CALCULATOR_H_
#define CALCULATOR_PROJECT_CALCULATOR_H_

#include "mathematical_expression.h"
#include "base.h"

CALCULATOR_BEGIN

class Calculator
{
protected:
	mathematical_expression::Expression expression_;

public:
	//constructor & destructor
	Calculator() noexcept;
	Calculator(const std::string& val) noexcept;
	Calculator(const mathematical_expression::Expression& val) noexcept;
	Calculator(const Calculator&) = default;
	Calculator(Calculator&&) = delete;
	~Calculator() = default;

	void Clear() noexcept;
	void RoundOff(const int digit = 0) noexcept;
	void RoundUp(const int digit = 0) noexcept;
	void RoundDown(const int digit = 0) noexcept;

	Calculator& operator=(const Calculator&) = default;
	Calculator& operator=(const Calculator&&) = delete;

	friend std::istream& operator>>(std::istream& _Istr, Calculator& val) noexcept;
	friend std::ostream& operator<<(std::ostream& _Ostr, const Calculator& val) noexcept;
};

std::istream& operator>>(std::istream& _Istr, Calculator& val) noexcept;
std::ostream& operator<<(std::ostream& _Ostr, const Calculator& val) noexcept;

CALCULATOR_END

#endif // !CALCULATOR_PROJECT_CALCULATOR_H_