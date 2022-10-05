#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class MyProduct
{
public:
	static std::vector<MyProduct> readFromFile(std::ifstream& in);
	friend std::ostream& operator<<(std::ostream& out, MyProduct* myProduct);
//private:
	char category[255], subcategory2[255], subcategory3[255], name[255];
	int number, quantity, vendorCode;
	double price;
};
