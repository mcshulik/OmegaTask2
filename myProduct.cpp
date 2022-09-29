#include "myProduct.h"
#include <algorithm>

std::vector<MyProduct> MyProduct::readFromFile(std::ifstream& in)
{
	MyProduct myProductItem;
	std::vector<MyProduct> vecItem;
	char str[256];
	std::string tempString;
	in.getline(str, 256);
 	while (true)
	{
		in.getline(myProductItem.number, 255, ';');
		tempString = myProductItem.number;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(myProductItem.number, tempString.c_str());
		in.getline(myProductItem.category, 255, ';'); 
		in.getline(myProductItem.subcategory2, 255, ';');
		in.getline(myProductItem.subcategory3, 255, ';');
		in.getline(myProductItem.vendorCode, 255, ';');
		tempString = myProductItem.vendorCode;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(myProductItem.vendorCode, tempString.c_str());
		in.getline(myProductItem.name, 255, ';');
		in.getline(myProductItem.price, 255, ';');
		tempString = myProductItem.price;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(myProductItem.price, tempString.c_str());
		in.getline(myProductItem.quantity, 255, '\n');
		tempString = myProductItem.quantity;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(myProductItem.quantity, tempString.c_str());
		if (!in)
			return vecItem;
		else 
			if (myProductItem.number[0] != '\0' && myProductItem.category[0] != '\0' && myProductItem.subcategory2[0] != '\0' && myProductItem.subcategory3[0] != '\0'
				&& myProductItem.vendorCode[0] != '\0' && myProductItem.name[0] != '\0' && myProductItem.price[0] != '\0' && myProductItem.quantity[0] != '\0')
				vecItem.push_back(myProductItem);
	}
}

std::ostream& operator<<(std::ostream& out, MyProduct* myProduct)
{
	out << myProduct->number << " | " << myProduct->category << " | " << myProduct->subcategory2 << " | " << myProduct->subcategory3 << " | "
		<< myProduct->vendorCode << " | " << myProduct->name << " | " << myProduct->price << " | " << myProduct->quantity;
	return out;
}
