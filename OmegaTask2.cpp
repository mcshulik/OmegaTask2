#include <windows.h>
#include <algorithm>
#include "myProduct.h"

int calcSearchTime(SYSTEMTIME timeBefore, SYSTEMTIME timeAfter)
{
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

void find(std::vector<std::vector<MyProduct*>> vecPtrs, int fields[10], int substringSearch, std::vector<std::string> keys)
{


	// функция поиска пока что работает только для поиска по 1 любому полю и только по полному совпадению строки
	

	//int fieldNumber = 0;
	std::vector<MyProduct> foundObjects;
	char key[255];
	strcpy_s(key, keys[0].c_str());
	bool ProducAvailability = false;
	int l = 0, r = vecPtrs[0].size(), mid, quantityOfProducts = 0;
	while (l < r)
	{
		mid = (l + r) / 2;
		switch (fields[0])
		{
		case 0:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->number, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->number, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->number, key) < 0)
					l = mid + 1;
			break;
		}
		case 1:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->category, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->category, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->category, key) < 0)
					l = mid + 1;
			break;
		}
		case 2:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->subcategory2, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->subcategory2, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->subcategory2, key) < 0)
					l = mid + 1;
			break;
		}
		case 3:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->subcategory3, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->subcategory3, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->subcategory3, key) < 0)
					l = mid + 1;
			break;
		}
		case 4:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->vendorCode, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->vendorCode, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->vendorCode, key) < 0)
					l = mid + 1;
			break;
		}
		case 5:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->name, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->name, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->name, key) < 0)
					l = mid + 1;
			break;
		}
		case 6:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->price, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->price, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->price, key) < 0)
					l = mid + 1;
			break;
		}
		case 7:
		{
			if (!strcmp(vecPtrs[fields[0]][mid]->quantity, key))
				ProducAvailability = true;
			if (strcmp(vecPtrs[fields[0]][mid]->quantity, key) >= 0)
				r = mid - 1;
			else
				if (strcmp(vecPtrs[fields[0]][mid]->quantity, key) < 0)
					l = mid + 1;
			break;
		}
		}
	}
	std::cout << "----------------------------------------------------------" << std::endl;
	if (!ProducAvailability)
		std::cout << "No matching properties found" << std::endl;
	else
	{
		while (true)
		{
			if (fields[0] == 0)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->number, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (fields[0] == 1)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->category, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (fields[0] == 2)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->subcategory2, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (fields[0] == 3)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->subcategory3, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (fields[0] == 4)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->vendorCode, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (fields[0] == 5)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->name, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (fields[0] == 6)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->price, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (fields[0] == 7)
			{
				if (!strcmp(vecPtrs[fields[0]][l]->quantity, key))
				{
					quantityOfProducts++;
					std::cout << vecPtrs[fields[0]][l] << std::endl;
					l++;
				}
				else
					break;
			}
			if (quantityOfProducts == 10)
			{
				std::cout << "Only 10 products are displayed, the list is not complete!" << std::endl;
				break;
			}
		}
	}
	std::cout << "----------------------------------------------------------" << std::endl;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SYSTEMTIME timeBefore, timeAfter;
	std::ifstream in("Catalog.csv");
	if (!in.is_open())
	{
		std::cout << "Error: couldn't open file";
		return 0;
	}
	std::vector<MyProduct> vecObjects = MyProduct::readFromFile(in);

	std::vector<std::vector<MyProduct*>> vecPtrs;
	//std::sort(vecObjects.begin(), vecObjects.end(), [](MyProduct a, MyProduct b) {return atoi(a.number) > atoi(b.number); });
	std::vector<MyProduct*> vecPtr;
	for (unsigned int i = 0; i < vecObjects.size(); i++)
	{
		MyProduct* temp = &vecObjects[i];
		vecPtr.push_back(temp);
	}
	for (int i = 0; i < 8; i++)
		vecPtrs.push_back(vecPtr);
	std::sort(vecPtrs[0].begin(), vecPtrs[0].end(), [](MyProduct* a, MyProduct* b) {return atoi(a->number) < atoi(b->number); });
	std::sort(vecPtrs[1].begin(), vecPtrs[1].end(), [](MyProduct* a, MyProduct* b) {return strcmp(a->category, b->category) < 0; });
	std::sort(vecPtrs[2].begin(), vecPtrs[2].end(), [](MyProduct* a, MyProduct* b) {return strcmp(a->subcategory2, b->subcategory2) < 0; });
	std::sort(vecPtrs[3].begin(), vecPtrs[3].end(), [](MyProduct* a, MyProduct* b) {return strcmp(a->subcategory3, b->subcategory3) < 0; });
	std::sort(vecPtrs[4].begin(), vecPtrs[4].end(), [](MyProduct* a, MyProduct* b) {return atoi(a->vendorCode) < atoi(b->vendorCode); });
	std::sort(vecPtrs[5].begin(), vecPtrs[5].end(), [](MyProduct* a, MyProduct* b) {return strcmp(a->name, b->name) < 0; });
	std::sort(vecPtrs[6].begin(), vecPtrs[6].end(), [](MyProduct* a, MyProduct* b) {return atof(a->price) < atof(b->price); });
	std::sort(vecPtrs[7].begin(), vecPtrs[7].end(), [](MyProduct* a, MyProduct* b) {return atoi(a->quantity) < atoi(b->quantity); });


	int choose = 9, substringSearch = 3, fieldsQuantity = 1, fields[8];
	for (int i = 0; i < 8; i++)
		fields[i] = 0;
	std::vector <std::string> keys;
	while (fieldsQuantity)
	{
		std::cout << "Enter the number of fields to search" << std::endl;
		std::cin >> fieldsQuantity;
		if (!std::cin)
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			fieldsQuantity = 1;
			continue;
		}
		if (fieldsQuantity > 8)
			continue;
		for (int i = 0; i < fieldsQuantity; i++)
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
			if (!choose)
				break;
			if (choose > 0 && choose < 9)
			{
				while (substringSearch > 2 || substringSearch < 1)
				{
					std::cout << "1 - string search" << std::endl;
					std::cout << "2 - substring search" << std::endl;
					std::cin >> substringSearch;
					if (!std::cin)
					{
						std::cin.clear();
						std::cin.ignore(INT_MAX, '\n');
					}
				}
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				char keyChar[255];
				std::cout << "Enter (sub)string" << std::endl;
				std::cin.getline(keyChar, 255, '\n');
				std::string key = std::string(keyChar);
				keys.push_back(key);
			}
			fields[i] = choose - 1;
		}
		GetLocalTime(&timeBefore);
		find(vecPtrs, fields, substringSearch, keys);
		GetLocalTime(&timeAfter);
		keys.clear();
		std::cout << "Search time in ms: " << calcSearchTime(timeBefore, timeAfter) << std::endl;
	}
	in.close();
}
