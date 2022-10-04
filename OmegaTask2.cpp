#include <windows.h>
#include <algorithm>
#include "myProduct.h"
#include <mutex>

std::vector<MyProduct*> foundObjects;
std::mutex myMutex;

struct structForThread
{
	std::vector<std::string> keys;
	int index = 0, fields[8] = {0}, left = 0, right = 0;
	std::atomic<bool> passedVector[3];
	std::atomic<bool> performFunction = false;
	std::atomic<bool>stopThread = false;
	std::vector<MyProduct*> objectsForFound;
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
			std::string tempStr (productItem->category);
			if (!tempStr.find(keys[i]), 0)
				return false;
			break;
		}
		case 3:
		{
			std::string tempStr(productItem->subcategory2);
			if (!tempStr.find(keys[i]), 0)
				return false;
			break;
		}
		case 4:
		{
			std::string tempStr(productItem->subcategory3);
			if (!tempStr.find(keys[i]), 0)
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
			std::string tempStr(productItem->name);
			if (!tempStr.find(keys[i]), 0)
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
	return true;
}

void find(std::vector<std::vector<MyProduct*>> vecPtrs, int fields[8], std::vector<std::string> keys, structForThread &myStruct)
{
	char key[255];
	strcpy_s(key, keys[0].c_str());
	int left = 0, right = vecPtrs[0].size(), quantityOfProducts = 0;
	int whileStop = 0;
	if (fields[0] == 1 || fields[0] == 5 || fields[0] == 7 || fields[0] == 8)
		findBorders(vecPtrs[fields[0] - 1], left, right, keys[0], fields[0]);
	std::cout << "----------------------------------------------------------" << std::endl;
	myStruct.objectsForFound = vecPtrs[fields[0] - 1];
	myStruct.left = left;
	if (right == vecPtrs[fields[0] - 1].size())
		right--;
	myStruct.right = right;
	myStruct.keys = keys;
	for(int i = 0; i < 8; i++)
		myStruct.fields[i] = fields[i];
	myStruct.performFunction = true;
	for (int i = left; i <= right; i += 4)
	{
		if (i + 4 >= right)
			int a = 0;
		if (myCompare(vecPtrs[fields[0] - 1][i], fields, keys))
		{
			myMutex.lock();
			foundObjects.push_back(vecPtrs[fields[0] - 1][i]);
			myMutex.unlock();
		}
		myMutex.lock();
		if (foundObjects.size() >= 10)
		{
			myMutex.unlock();
			myStruct.performFunction = false;
			break;
		}
		myMutex.unlock();
	}
	while (whileStop != 3)
	{
		if (!myStruct.performFunction)
			break;
		whileStop = 0;
		for (int i = 0; i < 3; i++)
		{
			if (myStruct.passedVector[i])
				whileStop++;
		}
	}
	myStruct.performFunction = false;
	myMutex.lock();
	if(foundObjects.size())
		std::sort(foundObjects.begin(), foundObjects.end(), [](MyProduct* a, MyProduct* b) {return atoi(a->number) < atoi(b->number); });
	for (unsigned int i = 0; i < foundObjects.size(); i++)
	{
		std::cout << foundObjects[i] << std::endl;
		if (i >= 9)
		{
			std::cout << "Only 10 products are displayed, the list is not complete!" << std::endl;
			break;
		}
	}
	if (!foundObjects.size())
		std::cout << "No matching properties found" << std::endl;
	std::cout << "----------------------------------------------------------" << std::endl;
	foundObjects.clear();
	for (int i = 0; i < 3; i++)
		myStruct.passedVector[i] = false;
	myMutex.unlock();
}

DWORD WINAPI threadFunc(LPVOID myStruct)
{
	int index = (*(structForThread*)myStruct).index;
	bool performFunctionCopy;
	(*(structForThread*)myStruct).passedVector[index - 1] = false;
	while (true)
	{
		if ((*(structForThread*)myStruct).stopThread)
			break;
		if ((*(structForThread*)myStruct).performFunction && (*(structForThread*)myStruct).passedVector[index - 1] == false)
		{
			if ((*(structForThread*)myStruct).right < (*(structForThread*)myStruct).left + index)
				(*(structForThread*)myStruct).passedVector[index - 1] = true;
			for (int i = (*(structForThread*)myStruct).left + index; i <= (*(structForThread*)myStruct).right; i += 4)
			{
				if (myCompare((*(structForThread*)myStruct).objectsForFound[i], (*(structForThread*)myStruct).fields, (*(structForThread*)myStruct).keys))
				{
					myMutex.lock();
					foundObjects.push_back((*(structForThread*)myStruct).objectsForFound[i]);
					myMutex.unlock();
				}
				myMutex.lock();
				if (foundObjects.size() >= 10)
				{
					(*(structForThread*)myStruct).performFunction = false;
					myMutex.unlock();
					break;
				}
				myMutex.unlock();
				if (i + 4 > (*(structForThread*)myStruct).right)
				{
					(*(structForThread*)myStruct).passedVector[index - 1] = true;
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
		find(vecPtrs, fields, keys, myStruct);
		GetLocalTime(&timeAfter);
		std::cout << "Search time in ms: " << calcSearchTime(timeBefore, timeAfter) << std::endl;
		keys.clear();
		for (int i = 0; i < 8; i++)
			fields[i] = 0;
	}
	myStruct.stopThread = true;
	for (int i = 0; i < 3; i++)
		CloseHandle(handleVec[i]);
	in.close();
	return 0;
}
