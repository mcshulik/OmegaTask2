#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class MyProduct
{
public:
	static std::vector<MyProduct> readFromFile(std::ifstream& in);
	static void find(int choose, int substringSearch);
private:
	char number[255], category[255], subcategory2[255], subcategory3[255], vendorCode[255], name[255], price[255], quantity[255];
};
