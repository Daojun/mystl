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

promise<vector<int>> producerVec;
future<vector<int>> consumerVec = { producerVec.get_future() };

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

void ProducerWithPromise()
{
	vector<int> vec{ 1, 2, 3, 4, 5 };
	producerVec.set_value(vec);
}

void ConsumerWithFuture()
{
	for (auto x : consumerVec.get())
	{
		cout << x << endl;
	}
}

bool ExcuteTestcase()
{
	thread testA{ PrintA };
	thread testB{ PrintB };
	testA.join();
	testB.join();

	thread testC{ Consumer };
	thread testD{ Producer };
	testC.join();
	testD.join();

	thread testE{ ConsumerWithFuture };
	thread testF{ ProducerWithPromise };
	testE.join();
	testF.join();

	cout << "Final data = " << data << endl;
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
};

#endif
