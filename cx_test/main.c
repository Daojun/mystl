#include "stdafx.h"

#include <iostream>
#include "testcases.h"

using namespace std;

void Help()
{
  cout<<"You can use main like this:"<<endl;
  cout<<"main 0 //to test case 0"<<endl;
}

int main(int argc, char**argv)
{
  CTestCaseSuit cases;
  if (argc == 1)
  {
    Help();
    cout<<cases.listAllTestCases();
  }
  else 
  {
    string strCaseIndex = argv[1];
    cases.excuteTestCase( stoi(strCaseIndex ));
  }  
}
