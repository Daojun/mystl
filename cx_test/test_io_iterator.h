#ifndef TEST_IO_ITERATOR_H
#define TEST_IO_ITERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <regex>
#include "testcase.h"
using namespace std;

struct phoneBook
{
  string name;
  int num;
};

ostream & operator << (ostream & oo, const phoneBook &pb)
{
  oo<<pb.name;
  oo<<" ";
  oo<<pb.num;
  oo<<endl;
  return oo;
}

istream & operator >> (istream & ii, phoneBook &pb)
{
  ii>>pb.name;
  ii>>pb.num; 
  return ii;
}

void writePhoneBook()
{
  vector<phoneBook> myPhoneBook = {{"John", 87654321}, {"Coco", 12345678}};
  string filePath = "phone_book.txt";
  ofstream  os{filePath};
  ostream_iterator<phoneBook> oi{os};
  copy(myPhoneBook.begin(), myPhoneBook.end(), oi);
}

void readPhoneBook()
{
  string filePath = "phone_book.txt";
  ifstream is{filePath};
  istream_iterator<phoneBook> ii{is};
  istream_iterator<phoneBook> eof{};
  vector<phoneBook> myPhoneBook{ii, eof};
  for (auto x:myPhoneBook)
    {
      cout<<x;
    }
}

void regexTest()
{
	regex pat(R"(\w{2}\s*\d{5}(-\d{4})?)");
	cout << "REG: "<<R"(\w{ 2 }\s*\d{ 5 }(-\d{ 4 }) ? )" << endl;
	//cout << pat << endl;
	int lineno = 0;
	for (string line; getline(cin, line);)
	{
		++lineno;
		smatch matches;
		if (regex_search(line, matches, pat))
			cout << lineno << ": " << matches[0] << endl;
	}
}

class CTestIOIterator : public CTestCase
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
    writePhoneBook();
    readPhoneBook();
	regexTest();
    std::cout<<"IO_Test case finished"<<std::endl;
    return true;
  }
};

#endif
