#include <windows.h>
#include <algorithm>
#include "myProduct.h"
//#include <thread>

std::vector<MyProduct*> foundObjects;

struct structForThread
{
	int index = 0;
	bool stop = false;
	std::vector<MyProduct*> foundObjects;
};


//DWORD threadFunc(int &i)
DWORD WINAPI threadFunc(LPVOID i)
{
	while (!(*(structForThread*)i).stop)
	{

	}

	return 0;
}

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

void findBorders(std::vector<MyProduct*> vecPtr, int &leftBorder, int &rightBorder, std::string key, int field)
{
	char keyChar[255];
	strcpy_s(keyChar, key.c_str());
	int mid = 0, left = leftBorder, right = rightBorder;
	while (leftBorder < rightBorder)
	{
		mid = (leftBorder + rightBorder) / 2;
		switch (field)
		{
		case 1:
		{
			if (atoi(vecPtr[mid]->number) >= atoi(keyChar))
				rightBorder = mid - 1;
			else
				if (atoi(vecPtr[mid]->number) < atoi(keyChar))
					leftBorder = mid + 1;
			break;
		}
		case 2:
		{
			if (strcmp(vecPtr[mid]->category, keyChar) >= 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->category, keyChar) < 0)
					leftBorder = mid + 1;
			break;
		}
		case 3:
		{
			if (strcmp(vecPtr[mid]->subcategory2, keyChar) >= 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->subcategory2, keyChar) < 0)
					leftBorder = mid + 1;
			break;
		}
		case 4:
		{
			if (strcmp(vecPtr[mid]->subcategory3, keyChar) >= 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->subcategory3, keyChar) < 0)
					leftBorder = mid + 1;
			break;
		}
		case 5:
		{
			if (atoi(vecPtr[mid]->vendorCode) >= atoi(keyChar))
				rightBorder = mid - 1;
			else
				if (atoi(vecPtr[mid]->vendorCode) < atoi(keyChar))
					leftBorder = mid + 1;
			break;
		}
		case 6:
		{
			if (strcmp(vecPtr[mid]->name, keyChar) >= 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->name, keyChar) < 0)
					leftBorder = mid + 1;
			break;
		}
		case 7:
		{
			if (atof(vecPtr[mid]->price) >= atof(keyChar))
				rightBorder = mid - 1;
			else
				if (atof(vecPtr[mid]->price) < atof(keyChar))
					leftBorder = mid + 1;
			break;
		}
		case 8:
		{
			if (atoi(vecPtr[mid]->quantity) >= atoi(keyChar))
				rightBorder = mid - 1;
			else
				if (atoi(vecPtr[mid]->quantity) < atoi(keyChar))
					leftBorder = mid + 1;
			break;
		}
		}
	}

	mid = (left + right) / 2;
	left = leftBorder;
	leftBorder = 0;
	rightBorder = vecPtr.size();

	while (leftBorder < rightBorder)
	{
		mid = (leftBorder + rightBorder) / 2;
		switch (field)
		{
		case 1:
		{
			if (atoi(vecPtr[mid]->number) > atoi(keyChar))
				rightBorder = mid - 1;
			else
				if (atoi(vecPtr[mid]->number) <= atoi(keyChar))
					leftBorder = mid + 1;
			break;
		}
		case 2:
		{
			if (strcmp(vecPtr[mid]->category, keyChar) > 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->category, keyChar) <= 0)
					leftBorder = mid + 1;
			break;
		}
		case 3:
		{
			if (strcmp(vecPtr[mid]->subcategory2, keyChar) > 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->subcategory2, keyChar) <= 0)
					leftBorder = mid + 1;
			break;
		}
		case 4:
		{
			if (strcmp(vecPtr[mid]->subcategory3, keyChar) > 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->subcategory3, keyChar) <= 0)
					leftBorder = mid + 1;
			break;
		}
		case 5:
		{
			if (atoi(vecPtr[mid]->vendorCode) > atoi(keyChar))
				rightBorder = mid - 1;
			else
				if (atoi(vecPtr[mid]->vendorCode) <= atoi(keyChar))
					leftBorder = mid + 1;
			break;
		}
		case 6:
		{
			if (strcmp(vecPtr[mid]->name, keyChar) > 0)
				rightBorder = mid - 1;
			else
				if (strcmp(vecPtr[mid]->name, keyChar) <= 0)
					leftBorder = mid + 1;
			break;
		}
		case 7:
		{
			if (atof(vecPtr[mid]->price) > atof(keyChar))
				rightBorder = mid - 1;
			else
				if (atof(vecPtr[mid]->price)<= atof(keyChar))
					leftBorder = mid + 1;
			break;
		}
		case 8:
		{
			if (atoi(vecPtr[mid]->quantity) > atoi(keyChar))
				rightBorder = mid - 1;
			else
				if (atoi(vecPtr[mid]->quantity) <= atoi(keyChar))
					leftBorder = mid + 1;
			break;
		}
		}
	}
	leftBorder = left;
	return;
}

void mySort(std::vector<MyProduct*> &vecPtr, int fieldNumber)
{
	switch (fieldNumber)
	{
	case 1:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->number) < atoi(b->number); });
		break;
	}
	case 2:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->number) < atoi(b->number); });
		break;
	}
	case 3:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->number) < atoi(b->number); });
		break;
	}
	case 4:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->number) < atoi(b->number); });
		break;
	}
	case 5:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->vendorCode) < atoi(b->vendorCode); });
		break;
	}
	case 6:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->number) < atoi(b->number); });
		break;
	}
	case 7:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atof(a->price) < atof(b->price); });
		break;
	}
	case 8:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->quantity) < atoi(b->quantity); });
		break;
	}
	}
}

bool myCompare(MyProduct* productItem, int fields[8], std::vector<std::string> keys)
{
	for (int i = 0; i < 8; i++)
	{
		if (!fields[i])
			return true;
		switch (fields[i])
		{
		case 1:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->number, tempKey))
				return false;
			break;
		}
		case 2:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->category, tempKey))
				return false;
			break;
		}
		case 3:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->subcategory2, tempKey))
				return false;
			break;
		}
		case 4:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->subcategory3, tempKey))
				return false;
			break;
		}
		case 5:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->vendorCode, tempKey))
				return false;
			break;
		}
		case 6:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->name, tempKey))
				return false;
			break;
		}
		case 7:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->price, tempKey))
				return false;
			break;
		}
		case 8:
		{
			char tempKey[255];
			strcpy_s(tempKey, keys[i].c_str());
			if (strcmp(productItem->quantity, tempKey))
				return false;
			break;
		}
		}
	}
	return false;
}

void find(std::vector<std::vector<MyProduct*>> vecPtrs, int fields[8], std::vector<std::string> keys)
{
	std::vector<MyProduct> foundObjects;
	char key[255];
	strcpy_s(key, keys[0].c_str());
	int left = 0, right = vecPtrs[0].size(), quantityOfProducts = 0;
	if (fields[0] == 1 || fields[0] == 5 || fields[0] == 7 || fields[0] == 8)
		findBorders(vecPtrs[fields[0] - 1], left, right, keys[0], fields[0]);
	std::cout << "----------------------------------------------------------" << std::endl;
	for (int i = left; i <= right; i++)
	{
		if (myCompare(vecPtrs[fields[0] - 1][i], fields, keys))
		{
			quantityOfProducts++;
			std::cout << vecPtrs[fields[0] - 1][i] << std::endl;
		}
		if (quantityOfProducts >= 10)
			break;
	}
	if (!quantityOfProducts)
		std::cout << "No matching properties found" << std::endl;
	else
		if (quantityOfProducts >= 10)
			std::cout << "Only 10 products are displayed, the list is not complete!" << std::endl;
	std::cout << "----------------------------------------------------------" << std::endl;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	structForThread myStruct;
	std::vector<HANDLE> handleVec;
	HANDLE hThread;
	DWORD myThreadID;
	myStruct.stop = true;
	hThread = CreateThread(0, 0, threadFunc, &myStruct, 0, &myThreadID);
	if(hThread)
		CloseHandle(hThread);
	SYSTEMTIME timeBefore, timeAfter;
	std::ifstream in("Catalog.csv");
	if (!in.is_open())
	{
		std::cout << "Error: couldn't open file";
		return 0;
	}
	std::vector<MyProduct> vecObjects = MyProduct::readFromFile(in);

	if(!vecObjects.size())
	{
		std::cout << "Error: file wasn't read!" << std::endl;
		return 0;
	}
	std::vector<std::vector<MyProduct*>> vecPtrs;
	std::vector<MyProduct*> vecPtr;
	for (unsigned int i = 0; i < vecObjects.size(); i++)
	{
		MyProduct* temp = &vecObjects[i];
		vecPtr.push_back(temp);
	}
	for (int i = 0; i < 8; i++)
	{
		vecPtrs.push_back(vecPtr);
		mySort(vecPtrs[i], i + 1);
	}

	int fieldNumber = 9, fieldsQuantity = 1, fields[8];
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
			std::cin >> fieldNumber;
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				fieldNumber = 9;
				continue;
			}
			if (!fieldNumber)
				break;
			if (fieldNumber > 0 && fieldNumber < 9)
			{
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				char keyChar[255];
				std::cout << "Enter substring" << std::endl;
				std::cin.getline(keyChar, 255, '\n');
				std::string key = std::string(keyChar);
				keys.push_back(key);
			}
			fields[i] = fieldNumber;
			if (fieldNumber == 1 || fieldNumber == 5 || fieldNumber == 7 || fieldNumber == 8)
			{
				for (int i2 = 0; i2 < 8; i2++)
				{
					if (!fields[i2])
						break;
					if ((fields[i2] >= 2 && fields[i2] <= 4) || fields[i2] == 6)
					{
						fields[i] = fields[i2];
						fields[i2] = fieldNumber;
						std::string tempString;
						tempString = keys[i];
						keys[i] = keys[i2];
						keys[i2] = tempString;
						break;
					}
				}
			}
		}
		GetLocalTime(&timeBefore);
		find(vecPtrs, fields, keys);
		GetLocalTime(&timeAfter);
		std::cout << "Search time in ms: " << calcSearchTime(timeBefore, timeAfter) << std::endl;
		keys.clear();
		for (int i = 0; i < 8; i++)
			fields[i] = 0;
	}
	in.close();
}
