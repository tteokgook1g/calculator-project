#pragma once
#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <queue>
#include <utility>
#include "Base.h"
#include "real_number.h"


REAL_NUMBER_BEGIN
FixedReal::FixedReal(const unsigned long long& _Val, bool _Sign, bool _Check2) noexcept :sign_(_Sign), infinity_bit_(0) {
	std::string val{ std::to_string(_Val) };
	reserved_size_ = static_cast<base::MySize_t>(val.length());
	data_int_ = new short[reserved_size_];
	data_dec_ = new short[reserved_size_];
	for (int i = reserved_size_ - 1;i >= 0;i--) {
		data_int_[reserved_size_ - i - 1] = val.at(i) - '0';
		data_dec_[i] = 0;
	}
}

FixedReal::FixedReal() noexcept :infinity_bit_(0), sign_(0), reserved_size_(10) {
	data_int_ = new short[10];
	data_dec_ = new short[10];
	for (int i = 9;i >= 0;i--) {
		data_int_[i] = 0;
		data_dec_[i] = 0;
	}
}
FixedReal::FixedReal(bool _Sign, bool _INFINITYbit) noexcept : infinity_bit_(_INFINITYbit) {
	if (infinity_bit_) {
		sign_ = _Sign;
		reserved_size_ = 0;
		data_int_ = nullptr;
		data_dec_ = nullptr;
		return;
	}
	sign_ = false;
	reserved_size_ = 10;
	data_int_ = new short[10];
	data_dec_ = new short[10];
	for (int i = 9;i >= 0;i--) {
		data_int_[i] = 0;
		data_dec_[i] = 0;
	}
}
FixedReal::FixedReal(const std::string& _Val) noexcept :infinity_bit_(0), sign_(0), reserved_size_(0) {
	data_int_ = nullptr;
	data_dec_ = nullptr;
	std::string val = _Val;
	base::DeleteComma(val);
	base::DeleteBlank(val);
	base::CapitalToSmall(val);
	if (_Val.find("-inf") != std::string::npos || _Val.find("-infinity") != std::string::npos) {
		*this = FixedReal(true, true);
		return;
	}
	if (_Val.find("inf") != std::string::npos || _Val.find("infinity") != std::string::npos) {
		*this = FixedReal(false, true);
		return;
	}
	if (_Val.find("nan") != std::string::npos) {
		*this = FixedReal(nullptr);
		return;
	}
	if (val.length() == 0) {
		*this = 0;
		return;
	}
	switch (val.at(0))
	{
	case '+':
		val.erase(0, 1);
		break;
	case '-':
		sign_ = 1;
		val.erase(0, 1);
	}
	if (val.length() > base::kMaxSizeVal) {
		std::cout << "It's too big" << std::endl;
		std::cout << "It'll be INFINITY instead" << std::endl;
		reserved_size_ = 0;
		data_int_ = nullptr;
		data_dec_ = nullptr;
		infinity_bit_ = true;
		return;
	}
	if (!base::IsNumber(val)) {
		std::cout << "It isn't a number." << std::endl;
		std::cout << "It'll be NaN instead." << std::endl;
		*this = FixedReal(nullptr);
		return;
	}
	std::stack<short> num_int;
	std::queue<short> num_dec;
	const size_t decimal_point = val.find('.');
	if (decimal_point == std::string::npos) {
		for (size_t i = 0;i < val.length();i++) {
			num_int.push(val.at(i) - '0');
		}
	}
	else
	{
		for (size_t i = 0;i < decimal_point;i++) {
			num_int.push(val.at(i) - '0');
		}
		for (size_t i = decimal_point + 1;i < val.length();i++) {
			num_dec.push(val.at(i) - '0');
		}
	}
	reserved_size_ = static_cast<base::MySize_t>(std::max(num_int.size(), num_dec.size()));
	data_int_ = new short[reserved_size_];
	data_dec_ = new short[reserved_size_];
	for (size_t i = 0;i < reserved_size_;i++) {
		if (!num_int.empty()) {
			data_int_[i] = num_int.top();
			num_int.pop();
		}
		else
		{
			data_int_[i] = 0;
		}
		if (!num_dec.empty()) {
			data_dec_[i] = num_dec.front();
			num_dec.pop();
		}
		else
		{
			data_dec_[i] = 0;
		}
	}
	for (int i = reserved_size_ - 1;i >= 0;i--) {
		if (data_int_[i] != 0 || data_dec_[i] != 0) {
			return;
		}
	}
	sign_ = false;
}
FixedReal::FixedReal(const short _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(static_cast<long long>(_Val) * -1, true, true));
}
FixedReal::FixedReal(const int _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(static_cast<long long>(_Val) * -1, true, true));
}
FixedReal::FixedReal(const long _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(static_cast<long long>(_Val) * -1, true, true));
}
FixedReal::FixedReal(const long long& _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(_Val * -1, true, true));
}
FixedReal::FixedReal(const unsigned short _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const unsigned int _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const unsigned long _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const unsigned long long& _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const float _Val) noexcept {
	*this = std::move(FixedReal{ std::to_string(_Val) });
}
FixedReal::FixedReal(const double& _Val) noexcept {
	*this = std::move(FixedReal{ std::to_string(_Val) });
}
FixedReal::FixedReal(const long double& _Val) noexcept {
	*this = std::move(FixedReal{ std::to_string(_Val) });
}
FixedReal::FixedReal(const bool _Val) noexcept {
	*this = std::move(FixedReal{});
	if (_Val) {
		data_int_[0] = 1;
	}
}
FixedReal::FixedReal(const char* const _Val) noexcept {
	*this = std::move(FixedReal{ std::string(_Val) });
}
FixedReal::FixedReal(const std::nullptr_t& _nullptr) noexcept
	:infinity_bit_(0), sign_(0), reserved_size_(0), data_int_(nullptr), data_dec_(nullptr) {}
FixedReal::FixedReal(const FixedReal& _Val) noexcept :infinity_bit_(0), sign_(0), reserved_size_(0), data_int_(nullptr), data_dec_(nullptr) {
	*this = _Val;
}
FixedReal::FixedReal(FixedReal&& _Val) noexcept {
	sign_ = _Val.sign_;
	infinity_bit_ = _Val.infinity_bit_;
	reserved_size_ = _Val.reserved_size_;
	data_int_ = _Val.data_int_;
	data_dec_ = _Val.data_dec_;
	_Val.data_int_ = nullptr;
	_Val.data_dec_ = nullptr;
}

FixedReal::~FixedReal() {
	if (data_int_ != nullptr) {
		delete[] data_int_;
	}
	if (data_dec_ != nullptr) {
		delete[] data_dec_;
	}
}


void FixedReal::Reserve(const base::MySize_t _Size) noexcept {
	if (infinity_bit_ || reserved_size_ >= _Size) {
		return;
	}
	if (data_dec_ == nullptr) {
		reserved_size_ = _Size;
		data_int_ = new short[reserved_size_];
		data_dec_ = new short[reserved_size_];
		for (int i = reserved_size_ - 1;i >= 0;i--) {
			data_int_[i] = 0;
			data_dec_[i] = 0;
		}
	}
	FixedReal temp(std::move(*this));
	reserved_size_ = _Size;
	data_int_ = new short[reserved_size_];
	data_dec_ = new short[reserved_size_];
	int i = 0;
	for (;i < temp.reserved_size_;i++) {
		data_int_[i] = temp.data_int_[i];
		data_dec_[i] = temp.data_dec_[i];
	}
	for (;i < reserved_size_;i++) {
		data_int_[i] = 0;
		data_dec_[i] = 0;
	}
}
void FixedReal::Sizeup() noexcept {
	if (reserved_size_ > base::kMaxSizeVal / 2) {
		Reserve(base::kMaxSizeVal);
		return;
	}
	this->Reserve(reserved_size_ * 2);
}


int FixedReal::DigitInt() const noexcept {
	if (infinity_bit_) {
		return return_INFINITY;
	}
	if (data_dec_ == nullptr) {
		return return_NaN;
	}
	int i = reserved_size_ - 1;
	for (;i >= 0;i--) {
		if (data_int_[i] != 0) {
			break;
		}
	}
	++i;
	if (i == 0) {
		return 1;
	}
	return static_cast<base::MySize_t>(i);
}
int FixedReal::DigitDec() const noexcept {
	if (infinity_bit_) {
		return return_INFINITY;
	}
	if (data_dec_ == nullptr) {
		return return_NaN;
	}
	int i = reserved_size_ - 1;
	for (;i >= 0;i--) {
		if (data_dec_[i] != 0) {
			break;
		}
	}
	++i;
	if (i == 0) {
		return 1;
	}
	return static_cast<base::MySize_t>(i);
}
int FixedReal::ReservedSizeCurrent() const noexcept {
	return reserved_size_;
}
int FixedReal::DigitHighest() const noexcept {
	if (infinity_bit_) {
		return return_INFINITY;
	}
	if (data_dec_ == nullptr) {
		return return_NaN;
	}
	if (DigitInt() > 1) {
		return DigitInt() - 1;
	}
	if (data_int_[0] != 0) {
		return 0;
	}
	for (int i = 0;i <= reserved_size_;i++) {
		if (data_dec_[i] != 0) {
			return -1 * i - 1;
		}
	}
	return 0;
}
bool FixedReal::IsInfinity() const noexcept {
	return infinity_bit_;
}
bool FixedReal::IsNan() const noexcept {
	if (data_dec_ == nullptr) {
		return true;
	}
	return false;
}

short& FixedReal::operator[](const int index) const {
	if (infinity_bit_) {
		std::cout << "REFERENCE INFINITY" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		throw std::exception{};
	}
	if (data_dec_ == nullptr) {
		std::cout << "REFERENCE NaN" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		throw std::exception{};
	}
	if (index >= reserved_size_ || index < -1 * reserved_size_) {
		std::cout << "Referenced value out of range." << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		std::cout << "size: " << reserved_size_ << ", index: " << index << std::endl;
		throw std::exception{};
	}
	if (index >= 0) {
		return data_int_[index];
	}
	return data_dec_[-1 * index - 1];
}

const FixedReal& FixedReal::operator=(const FixedReal& _Val) noexcept {
	if (_Val.infinity_bit_) {
		*this = std::move(FixedReal{ 0, 1 });
		sign_ = _Val.sign_;
		return *this;
	}
	if (_Val.data_int_ == nullptr || _Val.data_dec_ == nullptr) {
		*this = std::move(FixedReal{ nullptr });
		return *this;
	}
	sign_ = _Val.sign_;
	if (reserved_size_ < std::max(_Val.DigitInt(), _Val.DigitDec())) {
		Reserve(std::max(_Val.DigitInt(), _Val.DigitDec()));
	}
	for (int i = reserved_size_ - 1;i >= 0;i--) {
		data_int_[i] = _Val.At(i);
		data_dec_[i] = _Val.At(-1 * i - 1);
	}
	return *this;
}
const FixedReal& FixedReal::operator=(FixedReal&& _Val) noexcept {
	sign_ = _Val.sign_;
	infinity_bit_ = _Val.infinity_bit_;
	reserved_size_ = _Val.reserved_size_;
	data_int_ = _Val.data_int_;
	data_dec_ = _Val.data_dec_;
	_Val.data_int_ = nullptr;
	_Val.data_dec_ = nullptr;
	return *this;
}
const FixedReal& FixedReal::operator+=(const FixedReal& _Val) noexcept {
	(*this) = (*this) + _Val;
	return *this;
}
const FixedReal& FixedReal::operator-=(const FixedReal& _Val) noexcept {
	(*this) = (*this) - _Val;
	return *this;
}
const FixedReal& FixedReal::operator*=(const FixedReal& _Val) noexcept {
	(*this) = (*this) * _Val;
	return *this;
}
const FixedReal& FixedReal::operator/=(const FixedReal& _Val) noexcept {
	(*this) = (*this) / _Val;
	return *this;
}
const FixedReal& FixedReal::operator<<=(const int _Val) noexcept {
	(*this) = (*this) << _Val;
	return *this;
}
const FixedReal& FixedReal::operator>>=(const int _Val) noexcept {
	(*this) = (*this) >> _Val;
	return *this;
}

const FixedReal FixedReal::operator<<(const int index) const noexcept {
	if (data_dec_ == nullptr || index == 0) {
		return *this;
	}
	if (index < 0) {
		return (*this) >> (-1 * index);
	}
	FixedReal answer{ *this };
	answer.Reserve(this->reserved_size_ + index);
	for (int i = this->DigitInt() - 1;i >= -1 * this->DigitDec();i--) {
		answer[i + index] = answer[i];
		answer[i] = 0;
	}
	return answer;
}
const FixedReal FixedReal::operator>>(const int index) const noexcept {
	if (data_dec_ == nullptr || index == 0) {
		return *this;
	}
	if (index < 0) {
		return (*this) << (-1 * index);
	}
	FixedReal answer{ *this };
	answer.Reserve(this->reserved_size_ + index);
	for (int i = -1 * this->DigitDec();i < this->DigitInt();i++) {
		answer[i - index] = answer[i];
		answer[i] = 0;
	}
	return answer;
}
const FixedReal& FixedReal::operator++() noexcept {
	if (data_int_[0] != 9) {
		data_int_[0] += 1;
	}
	else
	{
		*this += 1;
	}
	return *this;
}
const FixedReal FixedReal::operator++(int) noexcept {
	FixedReal temp{ *this };
	if (data_int_[0] != 9) {
		data_int_[0] += 1;
	}
	else
	{
		*this += 1;
	}
	return temp;
}
const FixedReal& FixedReal::operator--() noexcept {
	if (data_int_[0] != 0) {
		data_int_[0] -= 1;
	}
	else
	{
		*this -= 1;
	}
	return *this;
}
const FixedReal FixedReal::operator--(int) noexcept {
	FixedReal temp(*this);
	if (data_int_[0] != 0) {
		data_int_[0] -= 1;
	}
	else
	{
		*this -= 1;
	}
	return temp;
}
const FixedReal FixedReal::operator-() const noexcept {
	FixedReal answer{ *this };
	answer.sign_ = !answer.sign_;
	return answer;
}


std::istream& operator>>(std::istream& _Istr, FixedReal& _Reference) noexcept {
	std::string _val;
	_Istr >> _val;
	_Reference = std::move(FixedReal{ _val });
	return _Istr;
}
std::ostream& operator<<(std::ostream& _Ostr, const FixedReal& _Val) noexcept {
	if (_Val.data_dec_ == nullptr) {
		if (_Val.infinity_bit_) {
			if (_Val.sign_) {
				_Ostr << "-INFINITY";
				return _Ostr;
			}
			_Ostr << "INFINITY";
			return _Ostr;
		}
		_Ostr << "NaN";
		return _Ostr;
	}
	if (_Val.sign_ == 1) {
		_Ostr << '-';
	}
	for (int i = _Val.DigitInt() - 1;i >= 0;i--) {
		_Ostr << _Val.data_int_[i];
	}
	if (_Val.DigitDec() == 1 && _Val.data_dec_[0] == 0) {
		return _Ostr;
	}
	_Ostr << '.';
	for (int i = 0;i < _Val.DigitDec();i++) {
		_Ostr << _Val.data_dec_[i];
	}
	return _Ostr;
}
FixedReal operator+(const FixedReal& _Val1, const FixedReal& _Val2) {
	if (_Val1.data_dec_ == nullptr || _Val2.data_dec_ == nullptr) {
		return FixedReal(nullptr);
	}
	if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			if (_Val1.sign_ ^ _Val2.sign_) {
				return FixedReal(nullptr);
			}
		}
		return _Val1;
	}
	if (_Val2.infinity_bit_) {
		return _Val2;
	}
	int to_alloc = std::max(std::max(_Val1.DigitDec(), _Val1.DigitInt()), std::max(_Val2.DigitDec(), _Val2.DigitInt())) + 1;
	if (_Val1.DigitInt() == base::kMaxSizeVal
		&& _Val2.DigitInt() == base::kMaxSizeVal
		&& (_Val1.At(base::kMaxSizeVal - 1) + _Val2.At(base::kMaxSizeVal - 1) > 9)
		&& _Val1.sign_ == _Val2.sign_) {
		if (_Val1.sign_) {
			return FixedReal(true, true);
		}
		else
		{
			return FixedReal(false, true);
		}
	}
	if (to_alloc > base::kMaxSizeVal) {
		to_alloc = base::kMaxSizeVal;
	}
	FixedReal answer{};
	FixedReal val1{ _Val1 };
	FixedReal val2{ _Val2 };
	if (val1.sign_) {
		for (int i = -1 * val1.DigitDec();i < val1.DigitInt();i++) {
			val1[i] *= -1;
		}
	}
	if (val2.sign_) {
		for (int i = -1 * val2.DigitDec();i < val2.DigitInt();i++) {
			val2[i] *= -1;
		}
	}
	answer.Reserve(to_alloc);
	for (int i = -1 * answer.reserved_size_;i < answer.reserved_size_;i++) {
		answer[i] = val1.At(i) + val2.At(i);
	}
	if (answer.At(answer.DigitInt() - 1) < 0) {
		answer.sign_ = 1;
		for (int i = -1 * answer.reserved_size_;i < answer.reserved_size_;i++) {
			answer[i] *= -1;
		}
	}
	else if (answer.At(answer.DigitInt() - 1) == 0) {
		for (int i = 0;i < answer.reserved_size_;i++) {
			if (answer.data_dec_[i] > 0) {
				break;
			}
			if (answer.data_dec_[i] < 0) {
				answer.sign_ = 1;
				for (int i = -1 * answer.reserved_size_;i < answer.reserved_size_;i++) {
					answer[i] *= -1;
				}
				break;
			}
		}
	}
	volatile int limit = answer.DigitInt();
	for (int i = -1 * answer.DigitDec();i < limit;i++) {
		if (answer.At(i) > 9) {
			answer[i] -= 10;
			answer[i + 1] += 1;
			if (i == limit - 1) {
				++limit;
			}
		}
		if (answer.At(i) < 0) {
			answer[i] += 10;
			--answer[i + 1];
		}
	}
	return answer;
}
FixedReal operator-(const FixedReal& _Val1, const FixedReal& _Val2) {
	FixedReal val2{ _Val2 };
	val2.sign_ = 1 - val2.sign_;
	return _Val1 + val2;
}
FixedReal operator*(const FixedReal& _Val1, const FixedReal& _Val2) {
	if (_Val1.data_dec_ == nullptr || _Val2.data_dec_ == nullptr) {
		return FixedReal(nullptr);
	}
	if (_Val1.infinity_bit_ && _Val2.infinity_bit_) {
		if (_Val1.sign_ ^ _Val2.sign_) {
			return FixedReal(true, true);
		}
		return FixedReal(false, true);
	}
	if (_Val1.infinity_bit_ || _Val2.infinity_bit_) {
		if (_Val1 == 0 || _Val2 == 0) {
			return FixedReal(nullptr);
		}
		if (_Val1.sign_ ^ _Val2.sign_) {
			return FixedReal(true, true);
		}
		return FixedReal(false, true);
	}
	if (_Val1 == 0 || _Val2 == 0) {
		return 0;
	}
	if (_Val1.DigitInt() + _Val2.DigitInt() > base::kMaxSizeVal) {
		std::cout << "$ERROR$ number is too big" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		throw std::exception{};
	}
	const int to_alloc = std::max(_Val1.DigitInt() + _Val2.DigitInt(), _Val1.DigitDec() + _Val2.DigitDec());
	FixedReal answer{};
	answer.Reserve(to_alloc);
	if (_Val1.sign_ ^ _Val2.sign_) {
		answer.sign_ = 1;
	}
	int k;
	if (_Val1.DigitDec() + _Val2.DigitDec() > base::kMaxSizeVal) {
		for (int i = -1 * _Val1.DigitDec();i < _Val1.DigitInt();i++) {
			for (int j = -1 * _Val2.DigitDec();j < _Val2.DigitInt();j++) {
				if (i + j < -1 * base::kMaxSizeVal) {
					continue;
				}
				k = _Val1.At(i) * _Val2.At(j);
				answer[i + j] += k % 10;
				answer[i + j + 1] += k / 10;
			}
		}
	}
	else
	{
		for (int i = -1 * _Val1.DigitDec();i < _Val1.DigitInt();i++) {
			for (int j = -1 * _Val2.DigitDec();j < _Val2.DigitInt();j++) {
				k = _Val1.At(i) * _Val2.At(j);
				answer[i + j] += k % 10;
				answer[i + j + 1] += k / 10;
			}
		}
	}
	volatile int limit = answer.DigitInt();
	for (int i = -1 * answer.DigitDec();i < limit;i++) {
		if (answer.At(i) > 9) {
			k = answer.At(i) / 10;
			answer[i] -= k * 10;
			answer[i + 1] += k;
			if (i == limit - 1) {
				++limit;
			}
		}
	}
	return answer;
}
FixedReal operator/(const FixedReal& _Val1, const FixedReal& _Val2) {
	if (_Val1.data_dec_ == nullptr || _Val2.data_dec_ == nullptr) {
		return FixedReal(nullptr);
	}
	if (_Val1.infinity_bit_ && _Val2.infinity_bit_) {
		return FixedReal(nullptr);
	}
	if (_Val1.infinity_bit_) {
		if (_Val1.sign_ ^ _Val2.sign_) {
			return FixedReal(true, true);
		}
		return FixedReal(false, true);
	}
	if (_Val2.infinity_bit_) {
		return 0;
	}
	if (_Val2 == 0) {
		if (_Val1 == 0) {
			return FixedReal(nullptr);
		}
		if (_Val1.sign_) {
			return FixedReal(true, true);
		}
		return FixedReal(false, true);
	}
	if (_Val1 == 0) {
		return 0;
	}
	const int to_alloc = std::max(base::kAccuracy, static_cast<int>(_Val1.DigitInt() + _Val2.DigitDec()));
	if (to_alloc > base::kMaxSizeVal) {
		if (_Val1.sign_ ^ _Val2.sign_) {
			return FixedReal(true, true);
		}
		else
		{
			return FixedReal(false, true);
		}
	}
	FixedReal answer{};
	answer.Reserve(to_alloc);
	if (_Val1.sign_ ^ _Val2.sign_) {
		answer.sign_ = 1;
	}
	FixedReal remainder{ _Val1 };
	FixedReal divisor{ _Val2 };
	remainder.sign_ = false;
	divisor.sign_ = false;
	FixedReal k{};
	for (int i = _Val1.DigitInt() + _Val2.DigitDec() - 1;i >= -1 * base::kAccuracy;i--) {
		for (int j = 9;j > 0;j--) {
			k = j * divisor * real_number::PowerOfTen(i);
			if (remainder >= k) {
				answer[i] = j;
				remainder -= k;
				if (remainder == 0) {
					return answer;
				}
				break;
			}
		}
	}
	return answer;
}
bool operator<(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	if (_Val1.data_dec_ == nullptr || _Val2.data_dec_ == nullptr) {
		return false;
	}
	else if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			if (_Val1.sign_ ^ _Val2.sign_) {
				return !_Val2.sign_;
			}
			return false;
		}
		return _Val1.sign_;
	}
	else if (_Val2.infinity_bit_) {
		return !_Val2.sign_;
	}
	if (_Val1.sign_ ^ _Val2.sign_) {
		return !_Val2.sign_;
	}
	if (_Val1.sign_) {
		FixedReal val1{ _Val1 };
		FixedReal val2{ _Val2 };
		val1.sign_ = false;
		val2.sign_ = false;
		if (val1 == val2) {
			return false;
		}
		return !(val1 < val2);
	}
	if (_Val1.DigitInt() < _Val2.DigitInt()) {
		return true;
	}
	if (_Val1.DigitInt() > _Val2.DigitInt()) {
		return false;
	}
	for (int i = _Val1.DigitInt() - 1;i >= 0;i--) {
		if (_Val1.data_int_[i] < _Val2.data_int_[i]) {
			return true;
		}
		if (_Val1.data_int_[i] > _Val2.data_int_[i]) {
			return false;
		}
	}
	for (int i = -1;i >= -1 * std::max(_Val1.DigitDec(), _Val2.DigitDec());i--) {
		if (_Val1.At(i) < _Val2.At(i)) {
			return true;
		}
		if (_Val1.At(i) > _Val2.At(i)) {
			return false;
		}
	}
	return false;
}
bool operator>(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	if (_Val1.data_dec_ == nullptr || _Val2.data_dec_ == nullptr) {
		return false;
	}
	else if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			if (_Val1.sign_ ^ _Val2.sign_) {
				return !_Val1.sign_;
			}
			return false;
		}
		return !_Val1.sign_;
	}
	else if (_Val2.infinity_bit_) {
		return _Val2.sign_;
	}
	if (_Val1.sign_ ^ _Val2.sign_) {
		return !_Val1.sign_;
	}
	if (_Val1.sign_) {
		FixedReal val1{ _Val1 };
		FixedReal val2{ _Val2 };
		val1.sign_ = false;
		val2.sign_ = false;
		if (val1 == val2) {
			return false;
		}
		return !(val1 > val2);
	}
	if (_Val1.DigitInt() > _Val2.DigitInt()) {
		return true;
	}
	if (_Val1.DigitInt() < _Val2.DigitInt()) {
		return false;
	}
	for (int i = _Val1.DigitInt() - 1;i >= 0;i--) {
		if (_Val1.data_int_[i] > _Val2.data_int_[i]) {
			return true;
		}
		if (_Val1.data_int_[i] < _Val2.data_int_[i]) {
			return false;
		}
	}
	for (int i = -1;i >= -1 * std::max(_Val1.DigitDec(), _Val2.DigitDec());i--) {
		if (_Val1.At(i) > _Val2.At(i)) {
			return true;
		}
		if (_Val1.At(i) < _Val2.At(i)) {
			return false;
		}
	}
	return false;
}
bool operator<=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	return (_Val1 < _Val2 || _Val1 == _Val2);
}
bool operator>=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	return (_Val1 > _Val2 || _Val1 == _Val2);
}
bool operator==(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			return !(_Val1.sign_ ^ _Val2.sign_);
		}
		return false;
	}
	else if (_Val2.infinity_bit_) {
		return false;
	}
	else if (_Val1.data_dec_ == nullptr && _Val2.data_dec_ == nullptr) {
		return true;
	}
	else if (_Val1.data_dec_ == nullptr || _Val2.data_dec_ == nullptr) {
		return false;
	}
	if (_Val1.sign_ ^ _Val2.sign_) {
		return false;
	}
	if (_Val1.DigitInt() != _Val2.DigitInt()) {
		return false;
	}
	if (_Val1.DigitDec() != _Val2.DigitDec()) {
		return false;
	}
	for (int i = _Val1.DigitInt() - 1;i >= 0;i--) {
		if (_Val1.data_int_[i] != _Val2.data_int_[i]) {
			return false;
		}
	}
	for (int i = _Val1.DigitDec() - 1;i >= 0;i--) {
		if (_Val1.data_dec_[i] != _Val2.data_dec_[i]) {
			return false;
		}
	}
	return true;
}
bool operator!=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	return !(_Val1 == _Val2);
}


FixedReal::operator std::string() const noexcept {
	if (infinity_bit_) {
		if (sign_) {
			return std::string{ "-INFINITY" };
		}
		return std::string{ "INFINITY" };
	}
	if (data_dec_ == nullptr) {
		return std::string{ "NaN" };
	}
	std::string answer{};
	if (sign_) {
		std::string minus{ "-" };
		answer = minus;
	}
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer = answer + (std::to_string(data_int_[i]));
	}
	if (DigitDec() == 1 && data_dec_[0] == 0) {
		return answer;
	}
	answer.push_back('.');
	for (int i = 0;i < DigitDec();i++) {
		answer = answer + std::to_string(data_dec_[i]);
	}
	return answer;
}
FixedReal::operator bool() const noexcept {
	if (*this == 0) {
		return false;
	}
	return true;
}
FixedReal::operator short() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > SHRT_MAX - 1) {
		return SHRT_MAX;
	}
	if (*this < SHRT_MIN + 1) {
		return SHRT_MIN;
	}
	short answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<short>(i);
	}
	if (sign_) {
		answer *= -1;
	}
	return answer;
}
FixedReal::operator unsigned short() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > USHRT_MAX - 1) {
		return USHRT_MAX;
	}
	if (*this < 1) {
		return 0;
	}
	unsigned short answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<unsigned short>(i);
	}
	return answer;
}
FixedReal::operator int() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > INT_MAX - 1) {
		return INT_MAX;
	}
	if (*this < INT_MIN + 1) {
		return INT_MIN;
	}
	int answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<int>(i);
	}
	if (sign_) {
		answer *= -1;
	}
	return answer;
}
FixedReal::operator unsigned int() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > UINT_MAX - 1) {
		return UINT_MAX;
	}
	if (*this < 1) {
		return 0;
	}
	unsigned int answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<unsigned int>(i);
	}
	return answer;
}
FixedReal::operator long() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > LONG_MAX - 1) {
		return LONG_MAX;
	}
	if (*this < LONG_MIN + 1) {
		return LONG_MIN;
	}
	long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<long>(i);
	}
	if (sign_) {
		answer *= -1;
	}
	return answer;
}
FixedReal::operator unsigned long() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > ULONG_MAX - 1) {
		return ULONG_MAX;
	}
	if (*this < 1) {
		return 0;
	}
	unsigned long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<unsigned long>(i);
	}
	return answer;
}
FixedReal::operator long long() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > LLONG_MAX - 1) {
		return LLONG_MAX;
	}
	if (*this < LLONG_MIN + 1) {
		return LLONG_MIN;
	}
	long long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<long long>(i);
	}
	if (sign_) {
		answer *= -1;
	}
	return answer;
}
FixedReal::operator unsigned long long() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this > ULLONG_MAX - 1) {
		return ULLONG_MAX;
	}
	if (*this < 1) {
		return 0;
	}
	unsigned long long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) {
		answer += data_int_[i] * base::PowerOfTen<unsigned long long>(i);
	}
	return answer;
}
FixedReal::operator float() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this >= FLT_MAX) {
		return FLT_MAX;
	}
	if (*this <= -1 * FLT_MAX) {
		return -1 * FLT_MAX;
	}
	if (*this == 0) {
		return 0.0f;
	}
	float answer = 0;
	for (int i = DigitHighest();i > DigitHighest() - 7;i--) {
		answer += At(i) * base::PowerOfTen<float>(i);
	}
	if (sign_) {
		answer *= -1;
	}
	return answer;
}
FixedReal::operator double() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this >= DBL_MAX) {
		return DBL_MAX;
	}
	if (*this <= -1 * DBL_MAX) {
		return -1 * DBL_MAX;
	}
	if (*this == 0) {
		return 0.0;
	}
	double answer = 0;
	for (int i = DigitHighest();i > DigitHighest() - 15;i--) {
		answer += At(i) * base::PowerOfTen<double>(i);
	}
	if (sign_) {
		answer *= -1;
	}
	return answer;
}
FixedReal::operator long double() const noexcept {
	if (data_dec_ == nullptr) {
		return NULL;
	}
	if (*this >= LDBL_MAX) {
		return LDBL_MAX;
	}
	if (*this <= -1 * LDBL_MAX) {
		return -1 * LDBL_MAX;
	}
	if (*this == 0) {
		return 0.0L;
	}
	long double answer = 0;
	for (int i = DigitHighest();i > DigitHighest() - 15;i--) {
		answer += At(i) * base::PowerOfTen<long double>(i);
	}
	if (sign_) {
		answer *= -1;
	}
	return answer;
}

short FixedReal::At(const int index) const {
	if (data_dec_ == nullptr || infinity_bit_) {
		std::cout << "$ERROR$ referenced value out of range" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		return NULL;
	}
	if (index >= reserved_size_ || index < -1 * reserved_size_) {
		return 0;
	}
	if (index >= 0) {
		return data_int_[index];
	}
	return data_dec_[-1 * index - 1];
}
FixedReal FixedReal::RoundOff(const int _digit) noexcept {
	(*this) = real_number::RoundOff(*this, _digit);
	return *this;
}
FixedReal FixedReal::RoundUp(const int _digit) noexcept {
	(*this) = real_number::RoundUp(*this, _digit);
	return *this;
}
FixedReal FixedReal::RoundDown(const int _digit) noexcept {
	(*this) = real_number::RoundDown(*this, _digit);
	return *this;
}


FixedReal Power(const real_number::FixedReal& base, const FixedReal& exponent) noexcept {
	//Check special case
	{
		if (base.data_dec_ == FixedReal(nullptr) || exponent.data_dec_ == FixedReal(nullptr))
			return FixedReal(nullptr);
		bool base0 = (base == 0), base_inf = (base == FixedReal(false, true));
		bool base_n_inf = (base == FixedReal(true, true)), expo0 = (exponent == 0);
		bool expo_inf = (exponent == FixedReal(false, true));
		bool expo_n_inf = (exponent == FixedReal(true, true));
		if (exponent > INT_MAX) expo_inf = true;
		else if (exponent < INT_MIN) expo_n_inf = true;
		if (((base0 || base_inf || base_n_inf) && expo0) ||
			(base_n_inf && expo_inf) ||
			(base0 && expo_n_inf)) return FixedReal(nullptr);
		if ((base0 && expo_inf) ||
			((base_inf || base_n_inf) && expo_n_inf)) return 0;
		if (base_inf && expo_inf) return FixedReal(false, true);
		//have to follow the order
		if (base0) return 0;
		if (expo0) return FixedReal(true); //1
		if (expo_n_inf) return 0;
		if (base_inf) return exponent > 0 ? FixedReal(false, true) : 0;
		if (expo_inf) return base > 0 ? FixedReal(false, true) : FixedReal(nullptr);
		if (base_n_inf) {
			if (exponent.IsInteger()) {
				if (exponent < 0) return 0;
				if (exponent.IsEven()) return FixedReal(false, true);
				else return FixedReal(true, true);
			}
			else return FixedReal(nullptr);
		}
	}
	if (exponent.IsInteger()) {
		FixedReal answer{ 1 };
		int _index = static_cast<int>(exponent);
		if (base == 10) {
			return (answer << _index);
		}
		if (_index > 0) {
			for (int i = _index - 1;i >= 0;i--) {
				answer *= base;
			}
		}
		else {
			for (int i = -1 * _index - 1;i >= 0;i--) {
				answer /= base;
			}
		}
		return answer;
	}
#ifdef _CMATH_
	return std::pow(static_cast<double>(base), static_cast<double>(exponent));
#else
	FixedReal answer{ 1 };
	int _index = static_cast<int>(exponent);
	if (base == 10) {
		return (answer << _index);
	}
	if (_index > 0) {
		for (int i = _index - 1;i >= 0;i--) {
			answer *= base;
		}
	}
	else {
		for (int i = -1 * _index - 1;i >= 0;i--) {
			answer /= base;
		}
	}
	return answer;
#endif // _CMATH_
}
FixedReal PowerOfTen(const int index) noexcept {
	FixedReal answer{ 1 };
	return answer << index;
}
FixedReal SquareRoot(const FixedReal& _Val) noexcept {
	if (_Val.data_dec_ == nullptr || _Val.sign_) {
		return FixedReal(nullptr);
	}
	if (_Val.infinity_bit_) {
		return FixedReal(false, true);
	}
	if (_Val == 0) {
		return 0;
	}
	real_number::FixedReal answer{};
	real_number::FixedReal remainder{ _Val };
	int highest;
	if (_Val.DigitHighest() >= 0) {
		highest = _Val.DigitHighest() / 2;
	}
	else
	{
		highest = (_Val.DigitHighest() - 1) / 2;
	}
	answer.Reserve(std::max(highest + 1, base::kAccuracy));
	answer[highest] = base::SqrtUnder100(10 * _Val.At(2 * highest + 1) + _Val.At(2 * highest));
	remainder -= FixedReal{ answer.At(highest) * answer.At(highest) } << (2 * highest);
	if (remainder == 0) {
		return answer;
	}
	real_number::FixedReal k{};
	for (int i = highest - 1;i >= -1 * base::kAccuracy;i--) {
		for (int j = 9;j > 0;j--) {
			k = (((answer >> i) * 2 + j) * j) << 2 * i;
			if (k <= remainder) {
				answer[i] = j;
				remainder -= k;
				if (remainder == 0) {
					return answer;
				}
				break;
			}
		}
	}
	return answer;
}
FixedReal PrintSquareRoot(const FixedReal& _Val, std::ostream& _Ostr) noexcept {
	if (_Val.data_dec_ == nullptr || _Val.sign_) {
		_Ostr << "NaN" << std::endl;
		return FixedReal(nullptr);
	}
	if (_Val.infinity_bit_) {
		_Ostr << "INFINITY" << std::endl;
		return FixedReal(false, true);
	}
	if (_Val == 0) {
		_Ostr << 0 << std::endl;
		return 0;
	}
	real_number::FixedReal answer{};
	real_number::FixedReal remainder{ _Val };
	int highest;
	if (_Val.DigitHighest() >= 0) {
		highest = _Val.DigitHighest() / 2;
	}
	else
	{
		highest = (_Val.DigitHighest() - 1) / 2;
	}
	answer.Reserve(std::max(highest + 1, base::kAccuracy));
	answer[highest] = base::SqrtUnder100(10 * _Val.At(2 * highest + 1) + _Val.At(2 * highest));
	remainder -= FixedReal{ answer.At(highest) * answer.At(highest) } << (2 * highest);
	if (remainder == 0) {
		if (highest >= 0) {
			_Ostr << answer.At(highest);
			for (int i = 0;i < highest;i++) {
				_Ostr << 0;
			}
		}
		else
		{
			_Ostr << "0.";
			for (int i = 0;i < -1 * highest - 1;i++) {
				_Ostr << '0';
			}
			_Ostr << answer.At(highest);
		}
		std::cout << std::endl;
		return answer;
	}
	if (highest > 0) {
		_Ostr << answer.At(highest);
	}
	else if (highest == 0)
	{
		_Ostr << answer.At(highest);
		_Ostr << '.';
	}
	else
	{
		_Ostr << "0.";
		for (int i = 0;i < -1 * highest - 1;i++) {
			_Ostr << '0';
		}
		_Ostr << answer.At(highest);
	}
	if (remainder == 0) {
		_Ostr << std::endl;
		return answer;
	}
	real_number::FixedReal k{};
	for (int i = highest - 1;i >= -1 * base::kAccuracy;i--) {
		for (int j = 9;j > 0;j--) {
			k = (((answer >> i) * 2 + j) * j) << 2 * i;
			if (k <= remainder) {
				answer[i] = j;
				remainder -= k;
				_Ostr << j;
				if (remainder == 0) {
					_Ostr << std::endl;
					return answer;
				}
				break;
			}
		}
		if (answer.At(i) == 0) {
			_Ostr << '0';
		}
		if (i == 0) {
			_Ostr << '.';
		}
	}
	_Ostr << std::endl;
	return answer;
}
FixedReal RoundOff(const FixedReal& _Val, const int _digit) noexcept {
	if (_Val.infinity_bit_ || _Val.data_dec_ == nullptr) {
		return _Val;
	}
	if (_digit > _Val.DigitInt()) {
		return 0;
	}
	if (_digit < -1 * _Val.DigitDec()) {
		return _Val;
	}
	real_number::FixedReal answer{};
	if (_digit == _Val.reserved_size_) {
		answer.Reserve(_digit + 1);
		if (_Val.At(_digit - 1) > 4) {
			answer[_digit] = 1;
			return answer;
		}
		return 0;
	}
	answer = _Val;
	if (_Val.At(_digit - 1) > 4) {
		++answer[_digit];
		for (int i = _digit;i < answer.reserved_size_;i++) {
			if (answer.At(i) == 10) {
				answer[i] = 0;
				++answer[i + 1];
			}
		}
	}
	return RoundDown(answer, _digit);
}
FixedReal RoundUp(const FixedReal& _Val, const int _digit) noexcept {
	if (_Val.infinity_bit_ || _Val.data_dec_ == nullptr) {
		return _Val;
	}
	if (_digit > _Val.DigitInt()) {
		return 0;
	}
	if (_digit < -1 * _Val.DigitDec()) {
		return _Val;
	}
	real_number::FixedReal answer{};
	if (_digit == _Val.reserved_size_) {
		answer.Reserve(_digit + 1);
		answer[_digit] = 1;
		return answer;
	}
	answer = _Val;
	++answer[_digit];
	for (int i = _digit;i < answer.reserved_size_;i++) {
		if (answer.At(i) == 10) {
			answer[i] = 0;
			++answer[i + 1];
		}
	}
	return RoundDown(answer, _digit);
}
FixedReal RoundDown(const FixedReal& _Val, const int _digit) noexcept {
	if (_Val.infinity_bit_ || _Val.data_dec_ == nullptr) {
		return _Val;
	}
	if (_digit >= _Val.DigitInt()) {
		return 0;
	}
	if (_digit < -1 * _Val.DigitDec()) {
		return _Val;
	}
	real_number::FixedReal answer{ _Val };
	for (int i = _digit - 1;i >= -1 * answer.reserved_size_;i--) {
		answer[i] = 0;
	}
	return answer;
}

REAL_NUMBER_END
