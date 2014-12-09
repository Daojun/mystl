#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include <iostream>
#include <thread>
#include <mutex>
#include "testcase.h"
using namespace std;

int data = 0;

//std::this_thread::sleep_for(chrono:seconds(1));

void PrintA()
{  
  for (int i=0; i<1000000; i++)
  {
    data = data-1;
  }
}

void PrintB()
{
  for (int i=0; i<1000000; i++)
  {
    data = data+1;
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
