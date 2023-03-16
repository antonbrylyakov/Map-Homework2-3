#include <iostream>
#ifdef _WIN32
#include "windows.h"
#endif

#include <thread>
#include "SafeDataSwap.hpp"
using namespace SafeDataSwap;

void swapDataNTimesLk(Data<int>& d1, Data<int>& d2, size_t n)
{
	// Пытаемся создать взаимоблокировку
	for (auto i = 0; i < n; ++i)
	{
		exchangeDataLk(d1, d2);
	}
}

void swapDataNTimesUl(Data<int>& d1, Data<int>& d2, size_t n)
{
	for (auto i = 0; i < n; ++i)
	{
		exchangeDataUl(d1, d2);
	}
}

void swapDataNTimesSl(Data<int>& d1, Data<int>& d2, size_t n)
{
	for (auto i = 0; i < n; ++i)
	{
		exchangeDataSl(d1, d2);
	}
}

void swapDataNTimesDl(Data<int>& d1, Data<int>& d2, size_t n)
{
	for (auto i = 0; i < n; ++i)
	{
		exchangeDataDeadlock(d1, d2);
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif

	Data<int> d1(1);
	Data<int> d2(2);
	int nTimes = 1000;

	std::cout << "Начинам многократный обмен данными (lock)" << std::endl;
	{
		std::thread t1(swapDataNTimesLk, std::ref(d1), std::ref(d2), nTimes);
		std::thread t2(swapDataNTimesLk, std::ref(d2), std::ref(d1), nTimes);

		t1.join();
		t2.join();
	}
	std::cout << "Завершено" << std::endl;

	std::cout << "Начинам многократный обмен данными (unique_lock)" << std::endl;
	{
		std::thread t1(swapDataNTimesUl, std::ref(d1), std::ref(d2), nTimes);
		std::thread t2(swapDataNTimesUl, std::ref(d2), std::ref(d1), nTimes);

		t1.join();
		t2.join();
	}
	std::cout << "Завершено" << std::endl;

	std::cout << "Начинам многократный обмен данными (scoped_lock)" << std::endl;
	{
		std::thread t1(swapDataNTimesSl, std::ref(d1), std::ref(d2), nTimes);
		std::thread t2(swapDataNTimesSl, std::ref(d2), std::ref(d1), nTimes);

		t1.join();
		t2.join();
	}
	std::cout << "Завершено" << std::endl;

	/*
	std::cout << "Начинам многократный обмен данными с вероятной взаимоблокировкой" << std::endl;
	{
		std::thread t1(swapDataNTimesDl, std::ref(d1), std::ref(d2), nTimes);
		std::thread t2(swapDataNTimesDl, std::ref(d2), std::ref(d1), nTimes);

		t1.join();
		t2.join();
	}
	std::cout << "Завершено" << std::endl;
	*/
}