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

std::ostream& operator<<(std::ostream& out, MyProduct* myProduct)
{
	out << myProduct->number << " | " << myProduct->category << " | " << myProduct->subcategory2 << " | " << myProduct->subcategory3 << " | "
		<< myProduct->vendorCode << " | " << myProduct->name << " | " << myProduct->price << " | " << myProduct->quantity;
	return out;
}
