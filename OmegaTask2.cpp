#include <windows.h>
#include "myProduct.h"

int calcSearchTime(SYSTEMTIME timeBefore, SYSTEMTIME timeAfter)
{
	timeAfter.wMilliseconds -= timeBefore.wMilliseconds;
	if (timeAfter.wMinute < timeBefore.wMinute)
		timeAfter.wMinute += 60;
	timeAfter.wMinute -= timeBefore.wMinute;
	if (timeAfter.wSecond < timeBefore.wSecond)
	{
		timeAfter.wSecond += 60;
		timeAfter.wMinute--;
	}
	timeAfter.wSecond -= timeBefore.wSecond;
	if (timeAfter.wMilliseconds < timeBefore.wMilliseconds)
	{
		timeAfter.wMilliseconds += 1000;
		timeAfter.wSecond--;
	}
	return timeAfter.wMilliseconds + timeAfter.wSecond * 1000 + timeAfter.wMinute * 1000 * 60;
}

int main()
{
	SYSTEMTIME timeBefore, timeAfter;
	std::ifstream in("Catalog.csv");
	if (!in.is_open())
	{
		std::cout << "Error: couldn't open file";
		return 0;
	}
	std::vector<MyProduct> vecItem = MyProduct::readFromFile(in);
	int choose = 9, substringSearch = 3;
	while (choose)
	{
		std::cout << "Choose search field" << std::endl;
		std::cout << "0 - exit" << std::endl;
		std::cout << "1 - number" << std::endl;
		std::cout << "2 - category" << std::endl;
		std::cout << "3 - subcategory2" << std::endl;
		std::cout << "4 - subcategory3" << std::endl;
		std::cout << "5 - vendorCode" << std::endl;
		std::cout << "6 - name" << std::endl;
		std::cout << "7 - price" << std::endl;
		std::cout << "8 - quantity" << std::endl;
		std::cin >> choose;
		if (!std::cin)
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			choose = 9;
			continue;
		}
		if (choose > 0 && choose < 9)
		{
			while(substringSearch > 2 || substringSearch < 1)
			{
				std::cout << "1 - substring search" << std::endl;
				std::cout << "2 - string search" << std::endl;
				std::cin >> substringSearch; 
				if (!std::cin)
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
			}
		}
		GetLocalTime(&timeBefore);
		MyProduct::find(choose, substringSearch);
		GetLocalTime(&timeAfter);
		std::cout << "Search time: " << calcSearchTime(timeBefore, timeAfter) << std::endl;
	}
	in.close();
}
