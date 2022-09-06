#pragma once
#include <conio.h>
#include <algorithm>
#include <Windows.h>
#include "Inventory.h"

class Marker {
	int position = 1;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
public:
	void changePosition(int line) { position = line; }

	std::ostream& operator()(std::ostream& out, int line) {
		if (position == line) {
			SetConsoleTextAttribute(hConsole, 4);
			out << ' ' << char(186) << ' ';
			SetConsoleTextAttribute(hConsole, 7);
		}
		else out << "    ";
		return out;
	}
};

////////////////////////////////////////

class Cursor {
	int upperBound, lowerBound;
	int currentPosition;
	bool pressed;
public:
	Cursor(int ub, int lb) : upperBound(ub), lowerBound(lb), currentPosition(1), pressed(false) {}

	void operator()() {
		int item = _getch();
		if (item == 0 || item == 224)
			switch (_getch()) {
			case 72:
				--currentPosition;
				if (currentPosition < upperBound) currentPosition = lowerBound;
				break;
			case 80:
				++currentPosition;
				if (currentPosition > lowerBound) currentPosition = upperBound;
				break;
			}
		else
			if (item == 13)
				pressed = true;
	}

	void press() { pressed = true; }
	void release() { pressed = false; }

	bool isPressed() const { return pressed; }
	int position() const { return currentPosition; }
	int& position() { return currentPosition; }
};

////////////////////////////////////////

class Warehouse {
	Inventory inventory;
	Marker mark;
public:
	Warehouse() {
		startProgramm();
	}

	void startProgramm() {
		mainMenu();
	}

	void mainMenu() {
		Cursor cursor(1, 7);
		while (!(cursor.isPressed() && cursor.position() == 7)) {
			mark.changePosition(cursor.position());
			system("cls");
			std::cout << "\t* Main menu *" << std::endl;
			mark(std::cout, 1) << "1. Add product;" << std::endl;
			mark(std::cout, 2) << "2. Redact product information;" << std::endl;
			mark(std::cout, 3) << "3. Delete product;" << std::endl;
			mark(std::cout, 4) << "4. View inventory;" << std::endl;
			mark(std::cout, 5) << "5. Sort;" << std::endl;
			mark(std::cout, 6) << "6. Search;" << std::endl;
			mark(std::cout, 7) << "7. Exit." << std::endl;
			std::cout << "\t[Use " << char(25) << char(24) << " to navigate.]";
			cursor();
			if (cursor.isPressed()) {
				cursor.release();
				try {
					switch (cursor.position()) {
					case 1:
						addMenu();
						break;
					case 2:
						redactMenu();
						break;
					case 3:
						deleteMenu();
						break;
					case 4:
						viewMenu();
						break;
					case 5:
						sortMenu();
						break;
					case 6:
						searchMenu();
						break;
					case 7:
						cursor.press();
						break;
					}
				}
				catch (std::exception exc) {
					system("cls");
					std::cout << "Operation failed: " << exc.what();
					Sleep(1000);
				}
			}
		}
	}

	void addMenu() {
		std::string str;
		Product newProduct;

		system("cls");
		std::cout << "\t* Add menu *" << std::endl;
		std::cout << "Enter the name: ";
		getline(std::cin, str);
		newProduct.setName(str);

		std::cout << "Enter the amount: ";
		getline(std::cin, str);
		newProduct.setAmount(stoi(str));

		std::cout << "Enter the date of receipt (dd.mm.yyyy): ";
		getline(std::cin, str);
		newProduct.setDateOfReceipt(myClasses::Date(str));

		std::cout << "Enter the price: ";
		getline(std::cin, str);
		newProduct.setPrice(stof(str));

		inventory.addProduct(newProduct);

		system("cls");
		std::cout << "\t* Add menu *" << std::endl;
		std::cout << "Product successfully added." << std::endl;
		Sleep(1000);

		if (inventory.getList().size() > 1)
			sortMenu();
	}

	void redactMenu() {
		if (inventory.getList().empty()) throw std::exception("Inventory is empty.");
		std::string name;

		system("cls");
		std::cout << "\t* Redact menu *" << std::endl;
		std::cout << "Enter the name of the product: ";
		getline(std::cin, name);
		Product product = searchByName(name);

		Cursor cursor(1, 5);
		while (!(cursor.isPressed() && cursor.position() == 5)) {
			mark.changePosition(cursor.position());
			system("cls");
			std::cout << "\t* Redact menu *" << std::endl;
			std::cout << "What to redact?" << std::endl;
			mark(std::cout, 1) << "1. Name;" << std::endl;
			mark(std::cout, 2) << "2. Amount;" << std::endl;
			mark(std::cout, 3) << "3. Date of receipt;" << std::endl;
			mark(std::cout, 4) << "4. Price;" << std::endl;
			mark(std::cout, 5) << "5. Cancel." << std::endl;
			std::cout << "\t[Use " << char(25) << char(24) << " to navigate.]";
			cursor();
			if (cursor.isPressed()) {
				switch (cursor.position()) {
				case 1:
					redactName(product);
					break;
				case 2:
					redactAmount(product);
					break;
				case 3:
					redactDate(product);
					break;
				case 4:
					redactPrice(product);
					break;
				case 5:
					break;
				}
				cursor.position() = 5;
			}
		}
		system("cls");
		std::cout << "Data redacted." << std::endl;
		Sleep(1000);
		if (inventory.getList().size() > 1)
			sortMenu();
	}

	void redactName(Product& product) {
		std::string name;
		system("cls");
		std::cout << "\t* Redact menu *" << std::endl;
		std::cout << "Enter a new name: ";
		getline(std::cin, name);
		product.setName(name);
	}

	void redactAmount(Product& product) {
		std::string amount;
		system("cls");
		std::cout << "\t* Redact menu *" << std::endl;
		std::cout << "Enter a new amount: ";
		getline(std::cin, amount);
		product.setAmount(stoi(amount));
	}

	void redactDate(Product& product) {
		std::string date;
		system("cls");
		std::cout << "\t* Redact menu *" << std::endl;
		std::cout << "Enter a new date (dd.mm.yyyy): ";
		getline(std::cin, date);
		product.setDateOfReceipt(myClasses::Date(date));
	}

	void redactPrice(Product& product) {
		std::string price;
		system("cls");
		std::cout << "\t* Redact menu *" << std::endl;
		std::cout << "Enter a new price: ";
		getline(std::cin, price);
		product.setPrice(stof(price));
	}

	void deleteMenu() {
		if (inventory.getList().empty()) throw std::exception("Inventory is empty.");
		system("cls");
		std::cout << "\t* Delete menu *" << std::endl;
		std::string name;
		std::cout << "Enter the name of the product: ";
		getline(std::cin, name);
		inventory.getList().remove_if([&name](Product& p) { return p.getName() == name; });
		std::cout << "Product deleted.";
		Sleep(1000);
	}

	void viewMenu() {
		if (inventory.getList().empty()) throw std::exception("Inventory is empty.");
		system("cls");
		std::cout << "\t* View menu *" << std::endl;
		for (Product& p : inventory.getList()) std::cout << p << std::endl;
		std::cout << std::endl;
		system("pause");
	}

	void sortMenu() {
		if (inventory.getList().empty()) throw std::exception("Inventory is empty.");
		Cursor cursor(1, 3);
		while (!cursor.isPressed()) {
			mark.changePosition(cursor.position());
			system("cls");
			std::cout << "\t* Sort menu *" << std::endl;
			mark(std::cout, 1) << "1. Sort by name;" << std::endl;
			mark(std::cout, 2) << "2. Sort by amount;" << std::endl;
			mark(std::cout, 3) << "3. Sort by price." << std::endl;
			std::cout << "\t[Use " << char(25) << char(24) << " to navigate.]";
			cursor();
			if (cursor.isPressed()) {
				switch (cursor.position()) {
				case 1:
					sortByName();
					break;
				case 2:
					sortByAmount();
					break;
				case 3:
					sortByPrice();
					break;
				}
			}
		}
		system("cls");
		std::cout << "Inventory is sorted." << std::endl;
		Sleep(1000);
	}

	void sortByName() {
		inventory.getList().sort();
	}

	void sortByAmount() {
		inventory.getList().sort([](const Product& left, const Product& right) { return left.getAmount() < right.getAmount(); });
	}

	void sortByPrice() {
		inventory.getList().sort([](const Product& left, const Product& right) { return left.getPrice() < right.getPrice(); });
	}

	void searchMenu() {
		if (inventory.getList().empty()) throw std::exception("Inventory is empty.");
		Cursor cursor(1, 3);
		while (!cursor.isPressed()) {
			mark.changePosition(cursor.position());
			system("cls");
			std::cout << "\t* Search menu *" << std::endl;
			mark(std::cout, 1) << "1. Search by name;" << std::endl;
			mark(std::cout, 2) << "2. Search by amount;" << std::endl;
			mark(std::cout, 3) << "3. Search by price." << std::endl;
			std::cout << "\t[Use " << char(25) << char(24) << " to navigate.]";
			cursor();
			if (cursor.isPressed()) {
				switch (cursor.position()) {
				case 1:
					searchByNameMenu();
					break;
				case 2:
					searchByAmountMenu();
					break;
				case 3:
					searchByPriceMenu();
					break;
				}
			}
		}
	}

	void searchByNameMenu() {
		std::string str;
		system("cls");
		std::cout << "\t* Search menu *" << std::endl;
		std::cout << "Enter the name of the product: ";
		getline(std::cin, str);
		system("cls");
		std::cout << "\t* Search menu *" << std::endl;
		std::cout << searchByName(str) << std::endl;
		system("pause");
	}

	Product& searchByName(const std::string& name) {
		auto it = std::find_if(begin(inventory.getList()), end(inventory.getList()), [&name](const Product& product) {return product.getName() == name; });
		if (it == end(inventory.getList())) throw std::exception("Product can't be found.");
		return *it;
	}

	void searchByAmountMenu() {
		std::string str;
		system("cls");
		std::cout << "\t* Search menu *" << std::endl;
		std::cout << "Enter the amount: ";
		getline(std::cin, str);
		system("cls");
		std::cout << "\t* Search menu *" << std::endl;
		for (Product& p : searchByAmount(stoi(str))) std::cout << p << std::endl;
		std::cout << std::endl;
		system("pause");
	}

	std::list<Product> searchByAmount(int amount) {
		std::list<Product> list;
		for (Product& p : inventory.getList())
			if (p.getAmount() == amount)
				list.push_back(p);
		if (list.empty()) throw std::exception("Products can't be found.");
		return list;
	}

	void searchByPriceMenu() {
		std::string str;
		system("cls");
		std::cout << "\t* Search menu *" << std::endl;
		std::cout << "Enter the price: ";
		getline(std::cin, str);
		system("cls");
		std::cout << "\t* Search menu *" << std::endl;
		for (Product& p : searchByPrice(stof(str))) std::cout << p << std::endl;
		std::cout << std::endl;
		system("pause");
	}

	std::list<Product> searchByPrice(float price) {
		std::list<Product> list;
		for (Product& p : inventory.getList())
			if (p.getPrice() == price)
				list.push_back(p);
		if (list.empty()) throw std::exception("Products can't be found.");
		return list;
	}
};