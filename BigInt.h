#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>
#include <exception>
class BigInt {
private:
	class divide_by_zero : std::exception {
	public:
		divide_by_zero() {};
	};
	std::vector<int> digits;
	bool sign = false;
	static const int base = 1'000'000'000;
	void normalize() {
		while (this->digits.size() > 1 && this->digits.back() == 0) {
			this->digits.pop_back();
		}
		// этот код нужен, чтобы у нас не было отрицательного нуля
		if (this->digits.size() == 1 && this->digits[0] == 0) 
			this->sign = false;
	}
	void num_to_BI(long long n, bool assignment)
	{
		if (assignment)
			digits.resize(0);
		if (n < 0)
			sign = true;
		n = abs(n);
		while (n) 
		{
			digits.push_back(n % base);
			n /= base;
		}
	}
	void str_to_BI(const std::string& s, bool  assignment)
	{
		if (assignment)
			digits.resize(0);
		if (s.length() == 0) 
		{
			sign = false;
			digits.push_back(0);
		}
		else 
		{
			sign = s[0] == '-';
			for (long long i = s.length() - sign; i > 0; i -= 9) 
			{
				if (i < 9) 
				{
					digits.push_back(atoi(s.substr(0, i).c_str()));
				}
				else 
				{
					digits.push_back(atoi(s.substr(i - 9, 9).c_str()));
				}
			}
			normalize();
		}
	}
	void shift_right() {
		if (this->digits.size() == 0) {
			this->digits.push_back(0);
			return;
		}
		this->digits.push_back(this->digits.back());
		for (size_t i = this->digits.size() - 2; i > 0; --i) 
			this->digits[i] = this->digits[i - 1];
		this->digits[0] = 0;
	}
public:
	bool odd() const { // He4eTHocTb
		if (this->digits.size() == 0) return false;
		return this->digits[0] & 1;
	}
	bool even() const { // 4eTHocTb
		return !this->odd();
	}
	friend void swap(BigInt& a, BigInt& b) 
	{
		std::swap(a.digits, b.digits);
		std::swap(a.sign, b.sign);
	}
	friend std::ostream& operator<<(std::ostream& out, const BigInt& bi)
	{
		auto i = bi.digits.rbegin();
		out << *i;
		for (++i; i != bi.digits.rend(); ++i) 
		{
			out << std::setfill('0') << std::setw(9) << *i;
		}
		return out;
	}
	friend std::istream& operator>>(std::istream& in, BigInt& bi)
	{
		std::string s;
		in >> s;
		bi.str_to_BI(s, true);
		return in;
	}

	BigInt() : digits(0), sign(false) {}
	BigInt(const BigInt&) = default;
	BigInt(BigInt&& b) noexcept { swap(*this, b); }

	BigInt(int a) 
	{
		num_to_BI(a, false);
	}
	BigInt(long long a) 
	{
		num_to_BI(a, false);
	}
	BigInt(const std::string& s) 
	{
		str_to_BI(s, false);
	}

	BigInt& operator=(int a)
	{
		num_to_BI(a, true);
		return *this;
	}
	BigInt& operator=(long long a)
	{
		num_to_BI(a, true);
		return *this;
	}
	BigInt& operator=(const std::string& s)
	{
		str_to_BI(s, true);
		return *this;
	}
	BigInt& operator=(const BigInt& left) 
	{
		digits = left.digits;
		sign = left.sign;
		return *this;
	}
	BigInt& operator=(BigInt&& left) noexcept
	{
		swap(*this, left);
		return *this;
	}

	friend const BigInt operator-(const BigInt& a) 
	{
		BigInt c(a);
		c.sign = !a.sign;
		return c;
	}
	friend const BigInt operator+(const BigInt& a) 
	{
		return a;
	}

	bool operator==(const BigInt& right) const
	{
		if (sign != right.sign) 
			return false;
		if (digits.empty()) 
			return right.digits.empty() ||
				(right.digits.size() == 1 && right.digits[0] == 0);
		if (right.digits.empty())
			return (digits.size() == 1 && digits[0] == 0);
		if (digits.size() != right.digits.size()) 
			return false;
		for (size_t i = 0; i < digits.size(); ++i) 
			if (digits[i] != right.digits[i]) 
				return false;

		return true;
	}
	bool operator< (const BigInt& right) const 
	{
		if (*this == right) return false; // равны? 
		if (sign) 
		{											   // - <
			if (right.sign) return ((-right) < (-(*this)));// -
			else return true;							   // +
		}
		else if (right.sign) return false;			  // + < -
		else if (digits.size() != right.digits.size())// + < +
				return digits.size() < right.digits.size();
		else 
			for (long long i = digits.size() - 1; i >= 0; --i) 		
				if (digits[i] != right.digits[i]) 
					return digits[i] < right.digits[i];
		return false;
	}
	bool operator> (const BigInt& right) const 
	{
		return right < *this;
	}
	bool operator!=(const BigInt& right) const {
		return !(*this == right);
	}
	bool operator>=(const BigInt& right) const {
		return !(*this < right);
	}
	bool operator<=(const BigInt& right) const {
		return !(*this > right);
	}

	friend const BigInt operator-(BigInt left, const BigInt& right) 
	{
		if (right.sign) return left + (-right);
		else if (left.sign) return -(-left + right);
		else if (left < right) return -(right - left);
		int carry = 0;
		for (size_t i = 0; i < right.digits.size() || carry != 0; ++i) 
		{
			left.digits[i] -= carry + (i < right.digits.size() ? right.digits[i] : 0);
			carry = left.digits[i] < 0;
			if (carry != 0) left.digits[i] += left.base;
		}
		left.normalize();
		return left;
	}
	friend const BigInt operator+(BigInt left, const BigInt& right) 
	{
		if (left.sign) 
		{
			if (right.sign) return -(-left + (-right));
			else return right - (-left);
		}
		else if (right.sign) return left - (-right);
		int carry = 0;
		for (int i = 0; i < std::max(left.digits.size(), right.digits.size()) || carry != 0; ++i) 
		{
			if (i == left.digits.size()) left.digits.push_back(0);
			left.digits[i] += carry + (i < right.digits.size() ? right.digits[i] : 0);
			carry = left.digits[i] >= left.base;
			if (carry != 0) left.digits[i] -= left.base;
		}
		return left;
	}
	friend const BigInt operator*(const BigInt& left, const BigInt& right) {
		BigInt result;
		result.digits.resize(left.digits.size() + right.digits.size());
		for (size_t i = 0; i < left.digits.size(); ++i) {
			int carry = 0;
			for (size_t j = 0; j < right.digits.size() || carry != 0; ++j) {
				long long cur = result.digits[i + j] +
					left.digits[i] * 1LL * (j < right.digits.size() ? right.digits[j] : 0) + carry;
				result.digits[i + j] = static_cast<int>(cur % BigInt::base);
				carry = static_cast<int>(cur / BigInt::base);
			}
		}
		result.sign = left.sign != right.sign;
		result.normalize();
		return result;
	}
	friend const BigInt operator/(const BigInt& left, const BigInt& right) {
		BigInt null(0);
		if (right == null) 
			throw divide_by_zero();
		BigInt b = right;
		b.sign = false;
		BigInt result, current;
		result.digits.resize(left.digits.size());
		for (long long i = static_cast<long long>(left.digits.size()) - 1; i >= 0; --i) {
			current.shift_right();
			current.digits[0] = left.digits[i];
			current.normalize();
			int x = 0, l = 0, r = BigInt::base;
			while (l <= r) {
				int m = (l + r) / 2;
				BigInt t = b;
				t *= m;
				if (t <= current) {
					x = m;
					l = m + 1;
				}
				else r = m - 1;
			}

			result.digits[i] = x;
			current = current - b * BigInt(x);
		}
		result.sign = left.sign != right.sign;
		result.normalize();
		return result;
	}
	friend const BigInt operator%(const BigInt& left, const BigInt& right) {
		BigInt result = left - (left / right) * right;
		if (result.sign) 
			result += right;
		return result;
	}

	template<class T>
	friend const BigInt operator-(const BigInt& left, T right) {
		return left - BigInt(right);
	}
	template<class T>
	friend const BigInt operator-(T left, const BigInt& right) {
		return right - left;
	}
	template<class T>
	friend const BigInt operator+(const BigInt& left, T right) {
		return left + BigInt(right);
	}
	template<class T>
	friend const BigInt operator+(T left, const BigInt& right) {
		return right + left;
	}
	template<class T>
	friend const BigInt operator*(BigInt left, T right) {
		return left * BigInt(right);
	}
	template<class T>
	friend const BigInt operator*(T left, const BigInt& right) {
		return right * left;
	}
	template<class T>
	friend const BigInt operator/(BigInt left, T right) {
		return left / BigInt(right);
	}
	template<class T>
	friend const BigInt operator/(T left, const BigInt& right) {
		return BigInt(left) / right;
	}
	template<class T>
	friend const BigInt operator%(const BigInt& left, T right) {
		return left % BigInt(right);
	}
	template<class T>
	friend const BigInt operator%(T left, const BigInt& right) {
		return BigInt(left) % right;
	}

	BigInt& operator %=(const BigInt& value) {
		return *this = (*this % value);
	}
	BigInt& operator /=(const BigInt& value) {
		return *this = (*this / value);
	}
	BigInt& operator *=(const BigInt& value) {
		return *this = (*this * value);
	}
	BigInt& operator +=(const BigInt& value) {
		return *this = (*this + value);
	}
	BigInt& operator -=(const BigInt& value) {
		return *this = (*this - value);
	}

	template<class T>
	BigInt& operator %=(const T& value) {
		return *this = (*this % value);
	}
	template<class T>
	BigInt& operator /=(const T& value) {
		return *this = (*this / value);
	}
	template<class T>
	BigInt& operator *=(const T& value) {
		return *this = (*this * value);
	}
	template<class T>
	BigInt& operator +=(const T& value) {
		return *this = (*this + value);
	}
	template<class T>
	BigInt& operator -=(const T& value) {
		return *this = (*this - value);
	}
	
	const BigInt operator++() {
		return (*this += 1);
	}
	const BigInt operator++(int) {
		BigInt copy = *this;
		*this += 1;
		return copy;
	}
	const BigInt operator--() {
		return *this -= 1;
	}
	const BigInt operator--(int) {
		BigInt copy = *this;
		*this -= 1;
		return copy;
	}

	const BigInt pow(BigInt n) const {
		BigInt a(*this), result(1), nul(0);
		while (n != nul) {
			if (n.odd()) 
				result *= a;
			a *= a;
			n /= 2;
		}
		return result;
	}
	friend const BigInt min(const BigInt& a, const BigInt& b) {
		return a < b ? a : b;
	}
	friend const BigInt max(const BigInt& a, const BigInt& b) {
		return a > b ? a : b;
	}

	operator bool() const {
		return *this != BigInt(0);
	}
	operator std::string() const {
		std::string ans = "";
		auto i = digits.rbegin();
		ans = std::to_string(*i);
		++i;
		std::string c = "000000000";
		for (; i != digits.rend(); ++i) {
			auto add = std::to_string(*i);
			if (add.size() < 9) {
				add = c.substr(0, 9 - add.size()) + add;
			}
			ans = ans + add;
		}
		return ans;
	}

	void help() {
		using namespace std;
		setlocale(LC_ALL, "Rus");
		cout << "Это мой класс BigInt \nДоп. Методы:\n pow(BigInt) - возвращает возведение в степень\n" <<
		" help() - выводит данное сообщение\n Примичание: для стабильной работы рекомендую считать только в BigInt-ax";
	}
};

#endif //BIGINT_H