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
		in.getline(str, 255, ';');
		tempString = str;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(str, tempString.c_str());
		myProductItem.number = std::atoi(str);
		in.getline(myProductItem.category, 255, ';'); 
		in.getline(myProductItem.subcategory2, 255, ';');
		in.getline(myProductItem.subcategory3, 255, ';');
		in.getline(str, 255, ';');
		tempString = str;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(str, tempString.c_str());
		myProductItem.vendorCode = std::atoi(str);
		in.getline(myProductItem.name, 255, ';');
		in.getline(str, 255, ';');
		tempString = str;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(str, tempString.c_str());
		myProductItem.price = std::atof(str);
		in.getline(str, 255, '\n');
		tempString = str;
		tempString.erase(remove(tempString.begin(), tempString.end(), -96), tempString.end());
		strcpy_s(str, tempString.c_str());
		myProductItem.quantity = std::atoi(str);
		if (!in)
			return vecItem;
		else 
			if (myProductItem.number != 0 && myProductItem.category[0] != '\0' && myProductItem.subcategory2[0] != '\0' && myProductItem.subcategory3[0] != '\0'
				&& myProductItem.vendorCode != 0 && myProductItem.name[0] != '\0' && myProductItem.price != 0 && myProductItem.quantity != 0)
				vecItem.push_back(myProductItem);
	}
}

std::ostream& operator<<(std::ostream& out, MyProduct* myProduct)
{
	out << myProduct->number << " | " << myProduct->category << " | " << myProduct->subcategory2 << " | " << myProduct->subcategory3 << " | "
		<< myProduct->vendorCode << " | " << myProduct->name << " | " << myProduct->price << " | " << myProduct->quantity;
	return out;
}
