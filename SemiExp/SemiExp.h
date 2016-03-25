#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
/////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                  //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Tianhang Zhang, Syracuse University                //
//              tzhan116@syr.edu	                               //
// Src Package: Jim Fawcett										   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a 
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
* Build Process:
* --------------
* Required Files: 
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
* 
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------
* - add public :, protected :, private : as terminators
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "itokcollection.h"


namespace Scanner
{
  using Token = std::string;

  class SemiExp : public ITokCollection
  {
  public:
    SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
    SemiExp& operator=(const SemiExp&) = delete;
    bool get();
    std::string operator[](size_t n);
    size_t length();
    void show();
	void trimFront(std::string& token);
	size_t find(const std::string& tok);
	void push_back(const std::string& tok);
	bool remove(const std::string& tok);
    bool remove(size_t i);
	void toLower();
	void clear();
	//void display();
  private:
    std::vector<Token> _tokens;
	///static std::vector<std::string> test_tokens;
	bool iftest = true;
    Toker* _pToker;
	std::vector<std::string> judge = { "while", "for", "else if", "if" };		// special tokens that goes together with "()"
  };
}
#endif
