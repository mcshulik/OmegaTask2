#include "myProduct.h"

int main()
{
	std::ifstream in("Catalog.csv");
	if (!in.is_open())
	{
		std::cout << "Error: couldn't open file";
		return 0;
	}
	else
	{
		std::vector<MyProduct> vecItem = MyProduct::readFromFile(in);
		
	}
	in.close();
}
