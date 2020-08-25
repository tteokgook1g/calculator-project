#pragma once
#include <string>
#include <stack>
#include <vector>
#include "real_number.h"
#include "mathematical_expression.h"

MATHEMATICAL_EXPRESSION_BEGIN
Expression::ExpressionComponent::ExpressionComponent(const std::string& val) noexcept {
	std::string _val(val);
	if (val.at(0) == '.') {
		_val.insert(val.begin(), '0');
	}
	type_ = ValueType(_val);
	if (type_ == Expression::ComponentType::kInvalid) {
		value_ = "Invalid";
	}
	else
	{
		value_ = _val;
	}
}


void Expression::InfixToPostfix() noexcept {
	std::vector<ExpressionComponent> infix(infix_expression_);
	infix.insert(infix.begin(), 1, ExpressionComponent("("));
	infix.push_back(ExpressionComponent(")"));
	std::stack<ExpressionComponent> stack_operator;
	for (size_t i = 0;i != infix.size();i++) {
		switch (infix.at(i).type_)
		{
		case ComponentType::kInvalid:
			postfix_expression_.clear();
			postfix_expression_.push_back(ExpressionComponent("Invalid"));
			return;
		case ComponentType::kNumber:
			postfix_expression_.push_back(infix.at(i));
			continue;
		case ComponentType::kUnaryOperator:
		case ComponentType::kBinaryOperator:
			if (stack_operator.empty()) {
				postfix_expression_.clear();
				postfix_expression_.push_back(ExpressionComponent("Invalid"));
				return;
			}
			{
				int a = static_cast<int>(Priority(infix.at(i).value_));
				volatile int b = static_cast<int>(Priority(stack_operator.top().value_));
				if (a > b) {
					stack_operator.push(infix.at(i));
				}
				else
				{
					while (a <= b)
					{
						postfix_expression_.push_back(stack_operator.top());
						stack_operator.pop();
						b = static_cast<int>(Priority(stack_operator.top().value_));
					}
					stack_operator.push(infix.at(i));
				}
			}
			continue;
		case ComponentType::kOpenParenthesis:
			stack_operator.push(infix.at(i));
			continue;
		case ComponentType::kClosingParenthesis:
			while (stack_operator.top().value_ != "(")
			{
				if (stack_operator.empty()) {
					postfix_expression_.clear();
					postfix_expression_.push_back(ExpressionComponent("Invalid"));
					return;
				}
				postfix_expression_.push_back(stack_operator.top());
				stack_operator.pop();
			}
			stack_operator.pop();
			continue;
		}
	}
	if (!stack_operator.empty()) {
		postfix_expression_.clear();
		postfix_expression_.push_back(ExpressionComponent("Invalid"));
	}
}
void Expression::ExpressionCalculate() noexcept {
	if (postfix_expression_.at(0).type_ == ComponentType::kInvalid) {
		value_of_expression_ = real_number::FixedReal(nullptr);
		return;
	}
	std::stack<real_number::FixedReal> numbers;
	real_number::FixedReal temp1, temp2;
	for (size_t i = 0;i != postfix_expression_.size();i++) {
		switch (postfix_expression_.at(i).type_)
		{
		case ComponentType::kNumber:
			numbers.push(real_number::FixedReal(postfix_expression_.at(i).value_));
			continue;
		case ComponentType::kUnaryOperator:
			if (numbers.empty()) {
				value_of_expression_ = real_number::FixedReal(nullptr);
				return;
			}
			temp1 = numbers.top();
			numbers.pop();
			switch (postfix_expression_.at(i).value_.c_str()[0])
			{
			case '!':
				numbers.push(real_number::Factorial(static_cast<unsigned int>(temp1)));
				continue;
			default:
				if (postfix_expression_.at(i).value_ == "sqrt") {
					numbers.push(real_number::SquareRoot(temp1));
				}
				continue;
			}
		case ComponentType::kBinaryOperator:
			if (numbers.size() < 2) {
				value_of_expression_ = real_number::FixedReal(nullptr);
				return;
			}
			temp2 = numbers.top();
			numbers.pop();
			temp1 = numbers.top();
			numbers.pop();
			switch (postfix_expression_.at(i).value_.c_str()[0])
			{
			case '*':
				numbers.push(temp1 * temp2);
				continue;
			case '+':
				numbers.push(temp1 + temp2);
				continue;
			case '-':
				numbers.push(temp1 - temp2);
				continue;
			case '/':
				numbers.push(temp1 / temp2);
				continue;
			case '^':
				numbers.push(real_number::Power(temp1,temp2));
				continue;
			}
		}
	}
	if (numbers.size() == 1) {
		value_of_expression_ = numbers.top();
	}
	else
	{
		value_of_expression_ = real_number::FixedReal(nullptr);
	}
}

Expression::Expression() noexcept {
	infix_expression_.push_back(ExpressionComponent("0"));
	postfix_expression_.push_back(ExpressionComponent("0"));
	value_of_expression_ = 0;
}
Expression::Expression(const std::string& expression) noexcept {
	std::string val(expression);
	base::BracketToParenthesis(val);
	base::DeleteComma(val);
	base::DeleteBlank(val);
	base::CapitalToSmall(val);
	__int8 current_typeid = 0; //0:nothing, 1:number, 2:string
	size_t start;
	for (size_t i = 0;i != val.length();i++) {
		switch (current_typeid)
		{
		case 0:
			switch (val.at(i))
			{
			case '(':
				infix_expression_.push_back(ExpressionComponent("("));
				continue;
			case ')':
				infix_expression_.push_back(ExpressionComponent(")"));
				continue;
			case '*':
				infix_expression_.push_back(ExpressionComponent("*"));
				continue;
			case '+':
				infix_expression_.push_back(ExpressionComponent("+"));
				continue;
			case '-':
				infix_expression_.push_back(ExpressionComponent("-"));
				continue;
			case '/':
				infix_expression_.push_back(ExpressionComponent("/"));
				continue;
			default:
				if (base::IsNumber(val.at(i)) || val.at(i) == '.') {
					current_typeid = 1;
					start = i;
				}
				else
				{
					current_typeid = 2;
					start = i;
				}
				continue;
			}
		case 1:
			switch (val.at(i))
			{
			case '(':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("("));
				current_typeid = 0;
				continue;
			case ')':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent(")"));
				current_typeid = 0;
				continue;
			case '*':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("*"));
				current_typeid = 0;
				continue;
			case '+':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("+"));
				current_typeid = 0;
				continue;
			case '-':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("-"));
				current_typeid = 0;
				continue;
			case '/':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("/"));
				current_typeid = 0;
				continue;
			default:
				if (!base::IsNumber(val.at(i)) && val.at(i) != '.') {
					infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
					current_typeid = 2;
					start = i;
				}
				continue;
			}
		case 2:
			switch (val.at(i))
			{
			case '(':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("("));
				current_typeid = 0;
				continue;
			case ')':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent(")"));
				current_typeid = 0;
				continue;
			case '*':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("*"));
				current_typeid = 0;
				continue;
			case '+':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("+"));
				current_typeid = 0;
				continue;
			case '-':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("-"));
				current_typeid = 0;
				continue;
			case '/':
				infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
				infix_expression_.push_back(ExpressionComponent("/"));
				current_typeid = 0;
				continue;
			default:
				if (base::IsNumber(val.at(i)) || val.at(i) == '.') {
					infix_expression_.push_back(ExpressionComponent(val.substr(start, i - start)));
					current_typeid = 1;
					start = i;
				}
				continue;
			}
		}
	}
	if (current_typeid) {
		infix_expression_.push_back(ExpressionComponent(val.substr(start, val.length() - start)));
	}
	if (infix_expression_.at(0).value_ == "+") {
		infix_expression_.erase(infix_expression_.begin());
	}
	else if (infix_expression_.at(0).value_ == "-") {
		infix_expression_.erase(infix_expression_.begin());
		infix_expression_.insert(infix_expression_.begin(), 1, ExpressionComponent('-' + infix_expression_.at(0).value_));
		infix_expression_.erase(infix_expression_.begin() + 1);
	}
	for (size_t i = 0;i != infix_expression_.size();i++) {
		if (infix_expression_.at(i).type_ == ComponentType::kOpenParenthesis) {
			++i;
			if (infix_expression_.at(i).value_ == "+") {
				infix_expression_.erase(infix_expression_.begin() + i);
			}
			else if (infix_expression_.at(i).value_ == "-") {
				infix_expression_.erase(infix_expression_.begin() + i);
				infix_expression_.insert(infix_expression_.begin() + i, 1, ExpressionComponent('-' + infix_expression_.at(i).value_));
				infix_expression_.erase(infix_expression_.begin() + i + 1);
			}
		}
	}

	InfixToPostfix();
	ExpressionCalculate();
}

std::string Expression::InfixExpression() const noexcept {
	std::string answer(infix_expression_.at(0).value_);
	for (int i = 1;i != infix_expression_.size();i++) {
		answer.append(" ");
		answer.append(infix_expression_.at(i).value_);
	}
	return answer;
}
std::string Expression::PostfixExpression() const noexcept {
	std::string answer(postfix_expression_.at(0).value_);
	for (int i = 1;i != postfix_expression_.size();i++) {
		answer.append(" ");
		answer.append(postfix_expression_.at(i).value_);
	}
	return answer;
}
real_number::FixedReal Expression::Value() const noexcept {
	return value_of_expression_;
}
void Expression::SquareRoot() noexcept {
	infix_expression_.push_back(ExpressionComponent(")"));
	infix_expression_.insert(infix_expression_.begin(), 1, ExpressionComponent("("));
	infix_expression_.insert(infix_expression_.begin(), 1, ExpressionComponent("sqrt"));
	postfix_expression_.push_back(ExpressionComponent("sqrt"));
	value_of_expression_ = real_number::SquareRoot(value_of_expression_);
}

const Expression& Expression::operator+=(const Expression& _Val) noexcept {
	*this = *this + _Val;
	return *this;
}
const Expression& Expression::operator-=(const Expression& _Val) noexcept {
	*this = *this - _Val;
	return *this;
}
const Expression& Expression::operator*=(const Expression& _Val) noexcept {
	*this = *this * _Val;
	return *this;
}
const Expression& Expression::operator/=(const Expression& _Val) noexcept {
	*this = *this / _Val;
	return *this;
}

std::istream& operator>>(std::istream& _Istr, Expression& _Val) noexcept {
	std::string str;
	std::getline(_Istr, str);
	_Val = Expression(str);
	return _Istr;
}
std::ostream& operator<<(std::ostream& _Ostr, const Expression& _Val) noexcept {
	_Ostr << _Val.InfixExpression();
	_Ostr << " = " << _Val.Value();
	return _Ostr;
}
bool operator>(const Expression& _Val1, const Expression& _Val2) noexcept {
	return _Val1.Value() > _Val2.Value();
}
bool operator<(const Expression& _Val1, const Expression& _Val2) noexcept {
	return _Val1.Value() < _Val2.Value();
}
bool operator>=(const Expression& _Val1, const Expression& _Val2) noexcept {
	return _Val1.Value() >= _Val2.Value();
}
bool operator<=(const Expression& _Val1, const Expression& _Val2) noexcept {
	return _Val1.Value() <= _Val2.Value();
}
bool operator==(const Expression& _Val1, const Expression& _Val2) noexcept {
	return _Val1.Value() == _Val2.Value();
}
bool operator!=(const Expression& _Val1, const Expression& _Val2) noexcept {
	return _Val1.Value() != _Val2.Value();
}


Expression operator+(const Expression& _Val1, const Expression& _Val2) noexcept {
	Expression answer(_Val1);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent("+")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		_Val2.infix_expression_.begin(),
		_Val2.infix_expression_.end()
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		_Val2.postfix_expression_.begin(),
		_Val2.postfix_expression_.end()
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		Expression::ExpressionComponent("+")
	);
	answer.value_of_expression_ = _Val1.value_of_expression_ + _Val2.value_of_expression_;
	return answer;
}
Expression operator-(const Expression& _Val1, const Expression& _Val2) noexcept {
	Expression answer(_Val1);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent("-")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent("(")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		_Val2.infix_expression_.begin(),
		_Val2.infix_expression_.end()
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent(")")
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		_Val2.postfix_expression_.begin(),
		_Val2.postfix_expression_.end()
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		Expression::ExpressionComponent("-")
	);
	answer.value_of_expression_ = _Val1.value_of_expression_ - _Val2.value_of_expression_;
	return answer;
}
Expression operator*(const Expression& _Val1, const Expression& _Val2) noexcept {
	Expression answer(_Val1);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.begin(),
		Expression::ExpressionComponent("(")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent(")")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent("*")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent("(")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		_Val2.infix_expression_.begin(),
		_Val2.infix_expression_.end()
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent(")")
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		_Val2.postfix_expression_.begin(),
		_Val2.postfix_expression_.end()
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		Expression::ExpressionComponent("*")
	);
	answer.value_of_expression_ = _Val1.value_of_expression_ * _Val2.value_of_expression_;
	return answer;
}
Expression operator/(const Expression& _Val1, const Expression& _Val2) noexcept {
	Expression answer(_Val1);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.begin(),
		Expression::ExpressionComponent("(")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent(")")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent("/")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent("(")
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		_Val2.infix_expression_.begin(),
		_Val2.infix_expression_.end()
	);
	answer.infix_expression_.insert
	(
		answer.infix_expression_.end(),
		Expression::ExpressionComponent(")")
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		_Val2.postfix_expression_.begin(),
		_Val2.postfix_expression_.end()
	);
	answer.postfix_expression_.insert
	(
		answer.postfix_expression_.end(),
		Expression::ExpressionComponent("/")
	);
	answer.value_of_expression_ = _Val1.value_of_expression_ / _Val2.value_of_expression_;
	return answer;
}

Expression SquareRoot(const Expression& _Val) noexcept {
	Expression answer(_Val);
	answer.SquareRoot();
	return answer;
}

Expression::ComponentType ValueType(std::string& val) noexcept {
	if (val.length() == 1) {
		switch (val.at(0))
		{
		case '!':
			return Expression::ComponentType::kUnaryOperator;
		case '(':
			return Expression::ComponentType::kOpenParenthesis;
		case ')':
			return Expression::ComponentType::kClosingParenthesis;
		case '*':
		case '+':
		case '-':
		case '/':
		case '^':
			return Expression::ComponentType::kBinaryOperator;
		}
	}
	if (base::IsNumber(val)) {
		return Expression::ComponentType::kNumber;
	}
	if (val == "sqrt") {
		return Expression::ComponentType::kUnaryOperator;
	}
	if (val == "root") {
		val = "sqrt";
		return Expression::ComponentType::kUnaryOperator;
	}
	return Expression::ComponentType::kInvalid;
}

OperatorPriority Priority(const std::string& val) noexcept {
	if (val.length() == 1) {
		switch (val.at(0))
		{
		case '!':
			return OperatorPriority::kFour;
		case '(':
			return OperatorPriority::kZero;
		case ')':
			return OperatorPriority::kZero;
		case '*':
			return OperatorPriority::kTwo;
		case '+':
			return OperatorPriority::kOne;
		case '-':
			return OperatorPriority::kOne;
		case '/':
			return OperatorPriority::kTwo;
		case '^':
			return OperatorPriority::kFour;
		}
	}
	if (val == "sqrt") {
		return OperatorPriority::kThree;
	}
	return OperatorPriority::kInvalid;
}

MATHEMATICAL_EXPRESSION_END
