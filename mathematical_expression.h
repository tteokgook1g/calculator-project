//This header file defines about mathematical expression.

#pragma once
#ifndef CALCULATOR_PROJECT_MATHEMATICAL_EXPRESSION_H_
#define CALCULATOR_PROJECT_MATHEMATICAL_EXPRESSION_H_

#include <string>
#include <vector>
#include "base.h"
#include "real_number.h"

MATHEMATICAL_EXPRESSION_BEGIN
class Expression {
protected:
	enum class ComponentType
	{
		kInvalid = -1,
		kNumber,
		kUnaryOperator,
		kBinaryOperator,
		kOpenParenthesis,
		kClosingParenthesis
	};

	class ExpressionComponent
	{
	public:
		ComponentType type_ = ComponentType::kInvalid;
		std::string value_ = "Invalid";

		ExpressionComponent() = delete;
		ExpressionComponent(const std::string& val) noexcept;
		ExpressionComponent(const ExpressionComponent&) = default;
		ExpressionComponent(ExpressionComponent&&) = default;
		~ExpressionComponent() = default;
		ExpressionComponent& operator=(const ExpressionComponent&) = default;
		ExpressionComponent& operator=(ExpressionComponent&&) = default;
	};

	std::vector<ExpressionComponent> infix_expression_;
	std::vector<ExpressionComponent> postfix_expression_;
	real_number::Real value_of_expression_;

	//Changes infix expression to postfix expression.
	void InfixToPostfix() noexcept;
	//Calculate postfix expression.
	//If an operator is added, you need to update.
	void ExpressionCalculate() noexcept;

public:
	//constructor & destructor
	Expression() noexcept;
	Expression(const std::string& expression) noexcept;
	Expression(const Expression&) = default;
	Expression(Expression&&) = default;
	~Expression() = default;

	//Returns infix_expression_.
	std::string InfixExpression() const noexcept;
	//Returns postfix_expression_.
	std::string PostfixExpression() const noexcept;
	//Returns value_of_expression_.
	real_number::Real Value() const noexcept;
	//Root the entire expression.
	void SquareRoot() noexcept;

	Expression& operator=(const Expression& _Val) = default;
	const Expression& operator+=(const Expression& _Val) noexcept;
	const Expression& operator-=(const Expression& _Val) noexcept;
	const Expression& operator*=(const Expression& _Val) noexcept;
	const Expression& operator/=(const Expression& _Val) noexcept;
	Expression& operator=(Expression&&) = default;

	friend Expression operator+(const Expression& _Val1, const Expression& _Val2) noexcept;
	friend Expression operator-(const Expression& _Val1, const Expression& _Val2) noexcept;
	friend Expression operator*(const Expression& _Val1, const Expression& _Val2) noexcept;
	friend Expression operator/(const Expression& _Val1, const Expression& _Val2) noexcept;

	friend Expression::ComponentType ValueType(std::string& val) noexcept;
};

std::istream& operator>>(std::istream& _Istr, Expression& _Val) noexcept;
std::ostream& operator<<(std::ostream& _Ostr, const Expression& _Val) noexcept;
bool operator>(const Expression& _Val1, const Expression& _Val2) noexcept;
bool operator<(const Expression& _Val1, const Expression& _Val2) noexcept;
bool operator>=(const Expression& _Val1, const Expression& _Val2) noexcept;
bool operator<=(const Expression& _Val1, const Expression& _Val2) noexcept;
bool operator==(const Expression& _Val1, const Expression& _Val2) noexcept;
bool operator!=(const Expression& _Val1, const Expression& _Val2) noexcept;

//Calculate square root of value of expression.
Expression SquareRoot(const Expression& _Val) noexcept;

//Check about ComponentType of val.
//Parameter could be changed if it has minor error. ex) root->sqrt
//If an operator is added, you need to update.
Expression::ComponentType ValueType(std::string& val) noexcept;

//If an operator is added, you need to update.
enum class OperatorPriority
{
	kInvalid = -1,
	kZero, //parentheses
	kOne, //addition, subtraction
	kTwo, //multiplication, division
	kThree //sqrt (square root)
};

//Returns operator's priority.
//If an operator is added, you need to update.
OperatorPriority Priority(const std::string& val) noexcept;

MATHEMATICAL_EXPRESSION_END

#endif // !CALCULATOR_PROJECT_MATHEMATICAL_EXPRESSION_H_