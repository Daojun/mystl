#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <future>

#include "testcase.h"
using namespace std;

int data = 0;
mutex mutexPrint;

queue<int> shareQueue;
condition_variable mcond;



//std::this_thread::sleep_for(chrono:seconds(1));

void PrintA()
{
	for (int i = 0; i < 1000; i++)
	{
		unique_lock<mutex> lock(mutexPrint);
		data = data - 1;
		cout << "A" << data;
	}
}

void PrintB()
{
	for (int i = 0; i < 1000; i++)
	{
		unique_lock<mutex> lock(mutexPrint);
		data = data + 1;
		cout << "B" << data;
	}
}

void Producer()
{
	int i = 0;
	while (i < 100)
	{
		unique_lock<mutex> lock{ mutexPrint };
		shareQueue.push(i);
		cout << "Producer Push " << i << endl;
		i++;
		mcond.notify_one();
	}
}

void Consumer()
{
	int i = 0;
	unique_lock<mutex> lock{ mutexPrint };
	while (i < 100)
	{
		if (shareQueue.empty())
		{
			mcond.wait(lock);
		}
		cout << "Comsumer Pop " << shareQueue.front() << endl;
		shareQueue.pop();
		i++;
	}
}

void ProducerWithPromise(promise<vector<int>> & producerVec)
{
	vector<int> vec{ 1, 2, 3, 4, 5 };
	producerVec.set_value(vec);
}

void ConsumerWithFuture(future<vector<int>> & consumerVec)
{
	for (auto x : consumerVec.get())
	{
		cout << x << endl;
	}
}

int get1()
{
	return 1;
}

int put1()
{
	return 1;
}

bool ExcuteTestcase()
{
	//1 mutex
	thread testA{ PrintA };
	thread testB{ PrintB };
	testA.join();
	testB.join();

	//2 condition_variable
	thread testC{ Consumer };
	thread testD{ Producer };
	testC.join();
	testD.join();
	cout << "Final data = " << data << endl;

	//3 promise/future
	promise<vector<int>> producerVec;
	future<vector<int>> consumerVec = { producerVec.get_future() };
	//should use move to have right value
	thread testE{ ConsumerWithFuture, move(consumerVec) };
	thread testF{ ProducerWithPromise, move(producerVec) };
	testE.join();
	testF.join();

	//4 package_task
	using Task_type = int();
	packaged_task < Task_type > pt0{ get1 };
	packaged_task < Task_type > pt1{ put1 };
	future<int> f0{ pt0.get_future() };
	future<int> f1{ pt1.get_future() };
	thread t1{ move(pt0) };
	thread t2{ move(pt1) };
	int result = f0.get() + f1.get();
	cout << "package_task result 1+1 = " << result << endl;
	//why the join is still needed?
	t1.join();
	t2.join();

	return true;
}

class CTestThread : public CTestCase
{
public:
	//Test Name
	virtual string getCaseName()
	{
		return __FILE__;
	}
	//Run testcases
	virtual bool excute()
	{
		ExcuteTestcase();
		std::cout << "Thread case finished" << std::endl;
		return true;
	}

private:
	//package_task<> pt{};
};

#endif
