/////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Tianhang Zhang, Syracuse University                //
//              tzhan116@syr.edu	                               //
// Src Package: Jim Fawcett										   //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

// trim empty lines
void SemiExp::trimFront(std::string& token)
{
	int numNewline = 0;
	if (token == "\n")
	{
		do {
			token = _pToker->getTok();
			numNewline++;
		} while (token == "\n");
	}
}

// find the position of a element in _tokens
size_t SemiExp::find(const std::string& token)
{
	for (size_t i = 0; i < length(); i++)
	{
		if (token == _tokens[i])
			return i;
	}
	return length();	// fail to find the given element
}

// push new elements to the back of _tokens
void SemiExp::push_back(const std::string& tok)
{
	_tokens.push_back(tok);
}

// remove the first found tok in _tokens
bool SemiExp::remove(const std::string& tok)
{
	size_t pos = find(tok);
	if (pos < length())
	{
		_tokens.erase(_tokens.begin() + pos);
		//for (size_t j = pos; j < length() - 1; j++)
		//	_tokens[j] = _tokens[j + 1];
		return true;
	}
	return false;
}

// remove the tok _tokens[i]
bool SemiExp::remove(size_t i)
{
	if (i < length() && i >= 0)
	{		
		_tokens.erase(_tokens.begin() + i);
		//for (size_t j = i; j < length() - 1; j++)
		//	_tokens[j] = _tokens[j + 1];
		return true;
	}
	return false;
}

// convert letters in a token to lower case
void SemiExp::toLower()
{
	for (size_t i = 0; i < length(); i++)
	{
		std::string currStr = _tokens[i];
		for (int j = 0; j < currStr.length(); j++)
		{
			if (currStr[j] >= 65 && currStr[j] <= 90)
				_tokens[i][j] += 32;
		}
	}
}

// clear the whole _tokens
void SemiExp::clear()
{
	_tokens.clear();
}

bool SemiExp::get()
{
  if (_pToker == nullptr)
    throw(std::logic_error("no Toker reference"));
  _tokens.clear();
  int tokencount = 0;
  while (true)
  {
    std::string token = _pToker->getTok();
    if (token == "")
      break;
	tokencount++;
	trimFront(token);
	_tokens.push_back(token);

	if (tokencount == 3 && iftest == true)
	{
		iftest = false;
		std::cout << "---------------------------------------" << "\n";
		std::cout << "------- Testing SemiExpression --------" << "\n";
		std::cout << "---------------------------------------" << "\n";
		std::cout << "------------- Requirement 9 -----------" << "\n\n";
		std::cout << " ----------------- Test push_back ----------------- " << "\n\n";
		std::vector<std::string> test = { "int", "string", ";", "char", "UPPERCASE"};
		for (int i = 0; i < test.size(); i++)
		{
			std::cout << " Push " << test[i] << " into _tokens:" << "\n\n";
			std::cout << " New _tokens is: " << "\n";
			push_back(test[i]);
			show();
			std::cout << "\n";
		}
		std::cout << " ----------------- Test find ----------------- " << "\n\n";
		std::cout << "\"string\" is found at position " << find("string") << " in _tokens." << "\n";

		std::cout << " ------------- Test operator [] ------------- " << "\n\n";
		for (size_t i = 0; i < length(); i++)
			std::cout << " Utilizing operator [] to obtain the #" << i << " element in _tokens:" << "  " << _tokens[i] << "\n";
		std::cout << " ----------------- Test toLower ----------------- " << "\n\n";
		std::cout << " Original _tokens is: " << "\n";
		show();
		toLower();
		std::cout << " Converted to lowercase we have: " << "\n";
		show();
		std::cout << " ----------------- Test remove ----------------- " << "\n\n";
		test[4] = "uppercase";
		for (int i = test.size() - 1; i >= 0; i--)
		{
			std::cout << " Remove " << test[i] << " frome _tokens:" << "\n\n";
			std::cout << " New _tokens is: " << "\n";
			remove(test[i]);
			show();
			std::cout << "\n";
		}
	}
	std::vector<std::string>::iterator ifjudge;
	ifjudge = std::find(judge.begin(), judge.end(), token);		//determine if the token is a special one
	if (token == "{")
	{
		return true;
	}
	else if (token == "}")
	{
		return true;
	}
	else if (token == "#")
	{
		while (token != ">")
		{
			token = _pToker->getTok();
			_tokens.push_back(token);
		}
		return true;
	}
	else if (ifjudge != judge.end())
	{
		int num = 0;
		while (true)
		{
			if (token=="(")		// in case the case statement include several braces, we care only the most outer pair
			{
				num = num + 1;
			}	
			token = _pToker->getTok();
			_tokens.push_back(token);
			if (token == ")")
			{
				num = num - 1;
			}
			if (token == ")"&&num==0)
			{
				return true;
			}
		}		
	}
	else if (token.compare(0, 2, "//") == 0 || token.compare(0, 2, "/*") == 0)		// leave out the comments
	{
		remove(token);
		return true;
	}
	else if (token.compare("else") == 0)
	{
		return true;
	}
	else if (token == ";")
	{
		return true;
	}
  }
  return false;
}

Token SemiExp::operator[](size_t n)
{
  if (n < 0 || n >= _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}

size_t SemiExp::length()
{
  return _tokens.size();
}

void SemiExp::show()
{
  std::cout << "\n  ";
  for (auto token : _tokens)
    if(token != "\n")
      std::cout << token << " ";
  std::cout << "\n";
}

#ifdef TEXT_SemiExp
int main()
{
  Toker toker;
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  std::string fileSpec = "../Tokenizer/Test.txt";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);

  SemiExp semi(&toker);
	while(semi.get())
	{
	std::cout << "\n  -- semiExpression --";
	semi.show();
	}

  /*
     May have collected tokens, but reached end of stream
     before finding SemiExp terminator.
   */
  if (semi.length() > 0)
  {
    std::cout << "\n  -- semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }
  return 0;
}

#endif