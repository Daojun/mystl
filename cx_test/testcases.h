#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "test_io_iterator.h"
#include "test_thread.h"
#include "test_socketserver.h"
#include <vector>
#include <string>
using namespace std;

class CTestCase;

class CTestCaseSuit
{
public:
	CTestCaseSuit()
	{
		//Add test cases here
		m_testCases.push_back(new CTestIOIterator());
		m_testCases.push_back(new CTestThread());
		m_testCases.push_back(new CTestSocketServer());
	}
	~CTestCaseSuit()
	{
		for (auto x : m_testCases)
			delete x;
	}
public:
	string listAllTestCases()
	{
		int i = 0;
		string helpText = "Test cases:\n";
		for (auto &x : m_testCases)
		{
			helpText += ('0' + i);
			helpText += " ";
			helpText += x->getCaseName();
			helpText += "\n";
			++i;
		}
		return helpText;
	}
	bool excuteTestCase(int index)
	{
		if (index >= 0 && index < m_testCases.size())
			return m_testCases[index]->excute();
		else
		{
			cout << "Case " << index << " is not existed" << endl;
			return false;
		}
	}
private:
	std::vector< CTestCase* > m_testCases;
};

#endif
