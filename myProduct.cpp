#include "myProduct.h"

std::vector<MyProduct> MyProduct::readFromFile(std::ifstream& in)
{
	MyProduct myProductItem;
	std::vector<MyProduct> vecItem;
	char str[256];
	in.getline(str, 256);
	while (true)
	{
		in.getline(myProductItem.number, 255, ';');
		in.getline(myProductItem.category, 255, ';');
		in.getline(myProductItem.subcategory2, 255, ';');
		in.getline(myProductItem.subcategory3, 255, ';');
		in.getline(myProductItem.vendorCode, 255, ';');
		in.getline(myProductItem.name, 255, ';');
		in.getline(myProductItem.price, 255, ';');
		in.getline(myProductItem.quantity, 255, '\n');
		if (!in)
			return vecItem;
		else vecItem.push_back(myProductItem);
	}
}

void MyProduct::find(int choose, int substringSearch)
{
	return;
}