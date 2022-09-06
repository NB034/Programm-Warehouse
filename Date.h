#pragma once
#include <iostream>
#include <string>

namespace myClasses {

	// Class

	class Date {
		int day;
		int month;
		int year;
		static int amountOfDays[12];
	public:
		Date(int day = 1, int month = 1, int year = 1970);
		Date(const std::string& date);

		void setDay(int day);
		void setMonth(int month);
		void setYear(int year);

		void setDate(int day);
		void setDate(int day, int month);
		void setDate(int day, int month, int year);

		inline int getDay() const { return this->day; }
		inline int getMonth() const { return this->month; }
		inline int getYear() const { return this->year; }

		void printDate() const;

		Date& operator++();
		Date& operator--();

		Date operator++(int);
		Date operator--(int);

		Date& operator+=(int days);
		Date& operator-=(int days);

		operator std::string() const;
	};

	int Date::amountOfDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	// Global functions

	bool isLeap(int year) {
		if (year % 4 != 0) return false;
		if (year % 100 != 0) return true;
		if (year % 400 != 0) return false;
		return true;
	}

	std::ostream& operator<<(std::ostream& out, const Date& date) {
		date.printDate();
		return out;
	}

	std::istream& operator>>(std::istream& in, Date& date) {
		int tmp;
		std::cout << "Day: ";
		in >> tmp;
		date.setDay(tmp);
		std::cout << "Month: ";
		in >> tmp;
		date.setMonth(tmp);
		std::cout << "Year: ";
		in >> tmp;
		date.setYear(tmp);
		return in;
	}

	Date operator+(Date date, int days) {
		Date result{ date };
		result += days;
		return result;
	}

	Date operator-(Date date, int days) {
		return Date(date) -= days;
	}

	bool operator<(const Date& left, const Date& right) {
		if (left.getYear() < right.getYear()) return true;
		if (left.getYear() > right.getYear()) return false;
		if (left.getMonth() < right.getMonth()) return true;
		if (left.getMonth() > right.getMonth()) return false;
		if (left.getDay() < right.getDay()) return true;
		return false;
	}

	bool operator==(const Date& left, const Date& right) {
		if (left.getYear() == right.getYear())
			if (left.getMonth() == right.getMonth())
				return left.getDay() == right.getDay();
		return false;
	}

	bool operator>(const Date& left, const Date& right) {
		return !(left < right || left == right);
	}

	bool operator<=(const Date& left, const Date& right) {
		return left < right || left == right;
	}

	bool operator>=(const Date& left, const Date& right) {
		return left > right || left == right;
	}

	bool operator!=(const Date& left, const Date& right) {
		return !(left == right);
	}

	// Member functions 

	Date::Date(int day, int month, int year) {
		this->setMonth(month);
		this->setDay(day);
		this->setYear(year);
	}

	Date::Date(const std::string& date) {
		if (date.length() != 10 || std::count(begin(date), end(date), '.') != 2)
			throw std::exception("Wrong format. String can't be converted to date.");
		setMonth(stoi(date.substr(date.find('.') + 1, date.rfind('.'))));
		setDay(stoi(date.substr(0, date.find('.'))));
		setYear(stoi(date.substr(date.rfind('.') + 1)));
	}

	void Date::setDay(int day) {
		if (day < 1)
			day = 1;
		if (isLeap(this->year) && this->month == 2 && 29 < day)
			day = 29;
		if (this->amountOfDays[this->month - 1] < day)
			day = this->amountOfDays[this->month - 1];
		this->day = day;
	}

	void Date::setMonth(int month) {
		if (month < 1) month = 1;
		if (12 < month) month = 12;
		this->month = month;
	}

	void Date::setYear(int year) {
		if (year < 1900 || 2099 < year) {
			this->year = 1970;
			return;
		}
		if (isLeap(this->year) && !isLeap(year) && this->month == 2 && this->day == 29)
			(*this)--;
		this->year = year;
	}

	void Date::setDate(int day) {
		setDay(day);
	}

	void Date::setDate(int day, int month) {
		setDay(day);
		setMonth(month);
	}

	void Date::setDate(int day, int month, int year) {
		setDay(day);
		setMonth(month);
		setYear(year);
	}

	void Date::printDate() const {
		if (day < 10) std::cout << '0';
		std::cout << day << '.';
		if (month < 10) std::cout << '0';
		std::cout << month << '.' << year;
	}

	Date& Date::operator++() {
		day++;
		if (day > amountOfDays[month - 1]) {
			day = 1;
			month++;
			if (month > 12) {
				month = 1;
				year++;
				if (year > 2099)
					year--;
			}
		}
		return *this;
	}

	Date& Date::operator--() {
		day--;
		if (day < 1) {
			int tmp = month - 2;
			if (tmp < 0)
				tmp = 11;
			day = amountOfDays[tmp];
			month--;
			if (month < 1) {
				month = 12;
				year--;
				if (year < 1900)
					year++;
			}
		}
		return *this;
	}

	Date Date::operator++(int) {
		Date tmp{ *this };
		++(*this);
		return tmp;
	}

	Date Date::operator--(int) {
		Date tmp{ *this };
		--(*this);
		return tmp;
	}

	Date& Date::operator+=(int days) {
		for (size_t i = 0; i < days; i++) (*this)++;
		return *this;
	}

	Date& Date::operator-=(int days) {
		for (size_t i = 0; i < days; i++) (*this)--;
		return *this;
	}

	Date::operator std::string() const {
		std::string date;
		if (day < 10) date += '0';
		date += std::to_string(day) + '.';
		if (month < 10) date += '0';
		date += std::to_string(month) + '.' + std::to_string(year);
		return date;
	}
}