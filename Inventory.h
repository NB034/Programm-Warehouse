#pragma once
#include <fstream>
#include <vector>
#include <list>
#include "Date.h"

class Product {
	std::string name;
	int amount;
	myClasses::Date dateOfReceipt;
	float price;
public:
	Product(const std::string& name = "[Unnamed]", int amount = 0, const myClasses::Date& dateOfReceipt = 0, const float& price = 0) {
		setName(name);
		setAmount(amount);
		setDateOfReceipt(dateOfReceipt);
		setPrice(price);
	}

	void setName(const std::string& name) {
		if (name.length() == 0) throw std::exception("Name can't be empty.");
		this->name = name;
	}
	void setAmount(int amount) {
		if (amount < 0) throw std::exception("Amount can't be negative.");
		this->amount = amount;
	}
	void setDateOfReceipt(const myClasses::Date& dateOfReceipt) {
		this->dateOfReceipt = dateOfReceipt;
	}
	void setPrice(const float& price) {
		if (price < 0) throw std::exception("Price can't be negative.");
		this->price = price;
	}

	const std::string& getName() const { return name; }
	int getAmount() const { return amount; }
	const myClasses::Date& getDateOfReceipt() const { return dateOfReceipt; }
	const float& getPrice() const { return price; }

	std::string getString() const {
		return std::string(name + ';' + std::to_string(amount) + ';' + (std::string)dateOfReceipt + ';' + std::to_string(price));
	}
};

std::ostream& operator<<(std::ostream& out, const Product& product) {
	return out << product.getName() << " - (A)" << product.getAmount() << " - " << product.getDateOfReceipt() << " - (P)" << product.getPrice();
}

bool operator<(const Product& left, const Product& right) {
	return left.getName() < right.getName();
}

/////////////////////////////////////////////////////

class Inventory {
	std::list<Product> list;
	static std::string filename;
public:
	Inventory() {
		std::ifstream file(filename);
		std::string buf;
		std::vector<std::string> line;
		while (!file.eof()) {
			getline(file, buf);
			if (!buf.empty()) {
				int it;
				while (buf.find(';') != std::string::npos) {
					it = buf.find(';');
					line.push_back(buf.substr(0, it));
					buf = buf.substr(it + 1);
				}
				list.push_back({ line[0], stoi(line[1]), myClasses::Date(line[2]), stof(buf) });
				line.clear();
			}
		}
		file.close();
		std::cout << "Inventory is loaded.";
		Sleep(800);
	}

	~Inventory() {
		std::ofstream file(filename);
		file.clear();
		auto it = begin(list);
		while (it != end(list)) {
			file << it->getString() << std::endl;
			it++;
		}
		file.close();
		system("cls");
		std::cout << "Inventory is saved.";
		Sleep(800);
	}

	void addProduct(Product& p) { list.push_back(p); }

	std::list<Product>& getList() { return list; }
};

std::string Inventory::filename = "Inventory.csv";