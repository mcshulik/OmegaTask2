#include <windows.h>
#include <algorithm>
#include "myProduct.h"
#include <mutex>

std::vector<MyProduct*> foundObjects;
CRITICAL_SECTION lpCriticalSection;

struct structForThread
{
	std::string keys[8];
	int keysInt[8] = { 0 };
	double keysDouble[8] = { 0 };
	volatile int index = 0, left = 0, right = 0;
	int fields[8] = { 0 };
	volatile std::atomic<bool> passedVector[3];
	volatile std::atomic<bool>stopThread = false;
	std::vector<MyProduct*> objectsForFound;
	std::vector<std::vector<MyProduct*>> foundObjectsThread;
};

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

void findBordersInt(std::vector<MyProduct*> vecPtr, int &leftBorder, int &rightBorder, int field, int keyInt)
{
	int mid = 0, left = leftBorder, right = rightBorder;
	while (leftBorder < rightBorder)
	{
		mid = (leftBorder + rightBorder) / 2;
		switch (field)
		{
		case 1:
		{
			if (vecPtr[mid]->number >= keyInt)
				rightBorder = mid - 1;
			else
				leftBorder = mid + 1;
			break;
		}
		case 5:
		{
			if (vecPtr[mid]->vendorCode >= keyInt)
				rightBorder = mid - 1;
			else
				leftBorder = mid + 1;
			break;
		}
		case 8:
		{
			if (vecPtr[mid]->quantity >= keyInt)
				rightBorder = mid - 1;
			else
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
			if (vecPtr[mid]->number > keyInt)
				rightBorder = mid - 1;
			else
				leftBorder = mid + 1;
			break;
		}
		case 5:
		{
			if (vecPtr[mid]->vendorCode > keyInt)
				rightBorder = mid - 1;
			else
				leftBorder = mid + 1;
			break;
		}
		case 8:
		{
			if (vecPtr[mid]->quantity > keyInt)
				rightBorder = mid - 1;
			else
				leftBorder = mid + 1;
			break;
		}
		}
	}
	leftBorder = left;
	return;
}

void findBordersDouble(std::vector<MyProduct*> vecPtr, int& leftBorder, int& rightBorder, double keyDouble)
{
	int mid = 0, left = leftBorder, right = rightBorder;
	while (leftBorder < rightBorder)
	{
		mid = (leftBorder + rightBorder) / 2;
		if (vecPtr[mid]->price >= keyDouble)
			rightBorder = mid - 1;
		else
			leftBorder = mid + 1;
		break;
	}

	mid = (left + right) / 2;
	left = leftBorder;
	leftBorder = 0;
	rightBorder = vecPtr.size();

	while (leftBorder < rightBorder)
	{
		mid = (leftBorder + rightBorder) / 2;
		if (vecPtr[mid]->price > keyDouble)
			rightBorder = mid - 1;
		else
			leftBorder = mid + 1;
		break;
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
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->number < b->number; });
		break;
	}
	case 2:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->number < b->number; });
		break;
	}
	case 3:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->number < b->number; });
		break;
	}
	case 4:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->number < b->number; });
		break;
	}
	case 5:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->vendorCode < b->vendorCode; });
		break;
	}
	case 6:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->number < b->number; });
		break;
	}
	case 7:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->price < b->price; });
		break;
	}
	case 8:
	{
		std::sort(vecPtr.begin(), vecPtr.end(), [](MyProduct* a, MyProduct* b) {return a->quantity < b->quantity; });
		break;
	}
	}
}

bool myCompare(MyProduct* productItem, int fields[8], std::string keys[8], double keyDouble[8], int keyInt[8])
{
	int iInt = 0, iDouble = 0, iStr = 0;
	for (int i = 0; i < 8; i++)
	{
		if (!fields[i])
			return true;
		switch (fields[i])
		{
		case 1:
		{
			if (productItem->number != keyInt[iInt])
				return false;
			iInt++;
			break;
		}
		case 2:
		{
			std::string tempStr (productItem->category);
			int tempInt = tempStr.find(keys[iStr]);
			if (tempInt < 0)
				return false;
			iStr++;
			break;
		}
		case 3:
		{
			std::string tempStr(productItem->subcategory2);
			int tempInt = tempStr.find(keys[iStr]);
			if (tempInt < 0)
				return false;
			iStr++;
			break;
		}
		case 4:
		{
			std::string tempStr(productItem->subcategory3);
			int tempInt = tempStr.find(keys[iStr]);
			if (tempInt < 0)
				return false;
			iStr++;
			break;
		}
		case 5:
		{
			if (productItem->vendorCode != keyInt[iInt])
				return false;
			iInt++;
			break;
		}
		case 6:
		{
			std::string tempStr(productItem->name);
			int tempInt = tempStr.find(keys[iStr]);
			if (tempInt < 0)
				return false;
			iStr++;
			break;
		}
		case 7:
		{
			if (productItem->price != keyDouble[iDouble])
				return false;
			iDouble++;
			break;
		}
		case 8:
		{
			if (productItem->quantity != keyInt[iInt])
				return false;
			iInt++;
			break;
		}
		}
	}
	return true;
}

void find(std::vector<std::vector<MyProduct*>> vecPtrs, int fields[8], std::string keys[8], double keysDouble[8], int keysInt[8], structForThread& myStruct)
{
	std::vector<MyProduct*> threadVec;
	EnterCriticalSection(&lpCriticalSection);
	foundObjects.clear();
	myStruct.foundObjectsThread.clear();
	char key[255];
	int keyInt = 0;
	double keyDouble = 0;
	strcpy_s(key, keys[0].c_str());
	int left = 0, right = vecPtrs[0].size(), quantityOfProducts = 0, whileStop = 0;
	if (fields[0] == 1 || fields[0] == 5 || fields[0] == 8)
		findBordersInt(vecPtrs[fields[0] - 1], left, right, fields[0], keysInt[0]);
	if (fields[0] == 7)
		findBordersDouble(vecPtrs[fields[0] - 1], left, right, keysDouble[0]);
	std::cout << "----------------------------------------------------------" << std::endl;
	myStruct.objectsForFound = vecPtrs[fields[0] - 1];
	myStruct.left = left;
	if (right == vecPtrs[fields[0] - 1].size())
		right--;
	myStruct.right = right;
	for (int i = 0; i < 8; i++)
	{
		myStruct.fields[i] = fields[i];
		myStruct.keys[i] = keys[i];
		myStruct.keysDouble[i] = keysDouble[i];
		myStruct.keysInt[i] = keysInt[i];
	}
	for (int i = 0; i < 3; i++)
		myStruct.passedVector[i] = false;
	LeaveCriticalSection(&lpCriticalSection);
	for (int i = left; i <= right; i += 4)
	{
		if (myCompare(vecPtrs[fields[0] - 1][i], fields, keys, keysDouble, keysInt))
		{
			EnterCriticalSection(&lpCriticalSection);
			threadVec.push_back(vecPtrs[fields[0] - 1][i]);
			LeaveCriticalSection(&lpCriticalSection);
		}
	}
	while (whileStop != 3)
	{
		whileStop = 0;
		for (int i = 0; i < 3; i++)
		{
			if (myStruct.passedVector[i])
				whileStop++;
		}
	}
	for (int i = 0; i < threadVec.size(); i++)
		foundObjects.push_back(threadVec[i]);
	for (int i = 0; i < myStruct.foundObjectsThread.size(); i++)
	{
		for (int j = 0; j < myStruct.foundObjectsThread[i].size(); j++)
			foundObjects.push_back(myStruct.foundObjectsThread[i][j]);
	}
	EnterCriticalSection(&lpCriticalSection);
	if(foundObjects.size())
		std::sort(foundObjects.begin(), foundObjects.end(), [](MyProduct* a, MyProduct* b) {return a->number < b->number; });
	for (unsigned int i = 0; i < foundObjects.size(); i++)
	{
		std::cout << foundObjects[i] << std::endl;
		if (i >= 99)
		{
			std::cout << "Only 100 products are displayed, the list is not complete!" << std::endl;
			std::cout << "Total number of items found: " << foundObjects.size() << std::endl;
			break;
		}
	}
	if (!foundObjects.size())
		std::cout << "No matching properties found" << std::endl;
	std::cout << "----------------------------------------------------------" << std::endl;
	foundObjects.clear();
	for (int i = 0; i < 3; i++)
		myStruct.passedVector[i] = false;
	myStruct.foundObjectsThread.clear();
	LeaveCriticalSection(&lpCriticalSection);
	threadVec.clear();
}

DWORD WINAPI threadFunc(LPVOID myStruct)
{
	std::vector<MyProduct*> threadVec;
	int index = (*(structForThread*)myStruct).index;
	(*(structForThread*)myStruct).passedVector[index - 1] = false;
	while (true)
	{
		if ((*(structForThread*)myStruct).stopThread)
			break;
		if ((*(structForThread*)myStruct).passedVector[index - 1] == false)
		{
			if ((*(structForThread*)myStruct).right < (*(structForThread*)myStruct).left + index)
				(*(structForThread*)myStruct).passedVector[index - 1] = true;
			for (int i = (*(structForThread*)myStruct).left + index; i <= (*(structForThread*)myStruct).right; i += 4)
			{
				if (myCompare((*(structForThread*)myStruct).objectsForFound[i], (*(structForThread*)myStruct).fields, (*(structForThread*)myStruct).keys, 
					(*(structForThread*)myStruct).keysDouble, (*(structForThread*)myStruct).keysInt))
				{
					EnterCriticalSection(&lpCriticalSection);
					threadVec.push_back((*(structForThread*)myStruct).objectsForFound[i]);
					LeaveCriticalSection(&lpCriticalSection);
				}
				EnterCriticalSection(&lpCriticalSection);
				LeaveCriticalSection(&lpCriticalSection);
				if (i + 4 > (*(structForThread*)myStruct).right)
				{
					EnterCriticalSection(&lpCriticalSection);
					(*(structForThread*)myStruct).foundObjectsThread.push_back(threadVec);
					threadVec.clear();
					(*(structForThread*)myStruct).passedVector[index - 1] = true;
					LeaveCriticalSection(&lpCriticalSection);
					break;
				}
			}
		}
	}
	return 0;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251); 
	InitializeCriticalSection(&lpCriticalSection);
	structForThread myStruct;
	std::vector<HANDLE> handleVec;
	DWORD myThreadID;
	for (int i = 0; i < 3; i++)
	{
		Sleep(100);
		myStruct.index++;
		handleVec.push_back(CreateThread(0, 0, threadFunc, &myStruct, 0, &myThreadID));
		if (!handleVec[i])
			std::cout << "Erorr: Stream creation error" << std::endl;
	}
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
	std::string keys[8] = { "\0" };
	double keysDouble[8] = { 0 };
	int keysInt[8] = { 0 };
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
		if (fieldsQuantity < 1)
			break;
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
				i--;
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
				if (fieldNumber == 1 || fieldNumber == 5 || fieldNumber == 8)
				{
					int i2 = 0;
					while (keysInt[i2] != 0)
						i2++;
					keysInt[i2] = std::atoi(keyChar);
				}
				else
					if (fieldNumber == 7)
					{
						int i2 = 0;
						while (keysDouble[i2] != 0)
							i2++;
						keysDouble[i2] = std::atof(keyChar);
					}
					else
					{
						int i2 = 0;
						char tempStr[255];
						while (true)
						{
							strcpy_s(tempStr, keys[i2].c_str());
							if (!strcmp(tempStr, "\0"))
								break;
							i2++;
						}
						keysDouble[i2] = std::atof(keyChar);
						std::string key = std::string(keyChar);
						keys[i2] = key;
					}
			}
			else
			{
				i--;
				continue;
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
						for (int i3 = 0; i3 < 6; i3++)
							keys[i3] = keys[i3 + 1];
						keys[7] = "\0";
						break;
					}
				}
			}
		}
		GetLocalTime(&timeBefore);
		find(vecPtrs, fields, keys, keysDouble, keysInt, myStruct);
		GetLocalTime(&timeAfter);
		std::cout << "Search time in ms: " << calcSearchTime(timeBefore, timeAfter) << std::endl;
		for (int i = 0; i < 8; i++)
		{
			fields[i] = 0;
			keys[i] = '\0';
			keysInt[i] = 0;
			keysDouble[i] = 0;
		}
	}
	myStruct.stopThread = true;
	for (int i = 0; i < 3; i++)
		CloseHandle(handleVec[i]);
	in.close();
	return 0;
}
