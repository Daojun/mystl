#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include <iostream>
#include <thread>
#include <mutex>
#include "testcase.h"
using namespace std;

int data = 0;
mutex mutexPrint;

//std::this_thread::sleep_for(chrono:seconds(1));

void PrintA()
{ 
  for (int i=0; i<1000; i++)
  {
	unique_lock<mutex> lock(mutexPrint);
    data = data-1;
	cout << "A" << data;
  }
}

void PrintB()
{
  for (int i=0; i<1000; i++)
  {
	unique_lock<mutex> lock(mutexPrint);
    data = data+1;
	cout << "B" << data;
  }
}

bool ExcuteTestcase()
{
  thread testA{PrintA};
  thread testB{PrintB};
  testA.join();
  testB.join();
  cout<<"Final data = "<<data<<endl;
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
    std::cout<<"Thread case finished"<<std::endl;
    return true;
  }
};

#endif
