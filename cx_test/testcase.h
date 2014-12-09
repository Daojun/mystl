#ifndef C_TEST_CASE_H
#define C_TEST_CASE_H

#include <string>
using namespace std;

class CTestCase
{
 public:
  //Test Name
  virtual string getCaseName() = 0;
  //Run testcases
  virtual bool excute() = 0;
  virtual ~CTestCase(){};
};

#endif
