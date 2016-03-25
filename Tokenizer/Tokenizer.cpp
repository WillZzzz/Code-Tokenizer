/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Tianhang Zhang, Syracuse University                //
//              tzhan116@syr.edu	                               //
// Src Package: Jim Fawcett										   //
/////////////////////////////////////////////////////////////////////
/*
  Helper code that does not attempt to handle all tokenizing
  special cases like escaped characters.
*/
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>

namespace Scanner
{
  class ConsumeState
  {
  public:

    ConsumeState();
    ConsumeState(const ConsumeState&) = delete;
    ConsumeState& operator=(const ConsumeState&) = delete;
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
      _pState->eatChars();
      _pState = nextState();
    }
    bool canRead() { return _pIn->good(); }
    std::string getTok() { return token; }
    bool hasTok() { return token.size() > 0; }
    ConsumeState* nextState();
	void resetFirst() { c_first = true; d_first = true; }	// reset the static member to enable multiple instances of Toker
  protected:
	static bool c_first, d_first;
    static std::string token;
    static std::istream* _pIn;
    static int prevChar;
    static int currChar;
	static int nextChar;
    static ConsumeState* _pState;
    static ConsumeState* _pEatCppComment;
    static ConsumeState* _pEatCComment;
    static ConsumeState* _pEatWhitespace;
    static ConsumeState* _pEatPunctuator;
    static ConsumeState* _pEatAlphanum;
    static ConsumeState* _pEatNewline;
	static ConsumeState* _pEatQuotedString;
	static ConsumeState* _pEatSpecialSingleChar;
	static ConsumeState* _pEatSpecialDoubleChar;
  };
};

using namespace Scanner;
// default sets of special single and double characters
std::vector<std::string> setSpecialSingleChar = { "[", "]", "(", ")", "{", "}", ",", ":", ";", "*", "=", "#", ".", "?", "&", "+", "-", "~", "!", "?", "%", "<", ">", "^", "|", "'" };
std::vector<std::string> setSpecialDoubleChar = { "*=","?=","%=","+=","-=","&=","?=","|=","->","++","--","##","<<",">>","!=","<=",">=","==","&&","||", "};" };
std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
int ConsumeState::nextChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatQuotedString = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingleChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialDoubleChar = nullptr;
bool ConsumeState::c_first = true;
bool ConsumeState::d_first = true;

void testLog(const std::string& msg);

ConsumeState* ConsumeState::nextState()
{
  if (!(_pIn->good()))
  {
    return nullptr;
  }
  int chNext = _pIn->peek();
  if (chNext == EOF)
  {
    _pIn->clear();
    // if peek() reads end of file character, EOF, then eofbit is set and
    // _pIn->good() will return false.  clear() restores state to good
  }
  if (std::isspace(currChar) && currChar != '\n')
  {
    testLog("state: eatWhitespace");
    return _pEatWhitespace;
  }
  if (currChar == '/' && chNext == '/')
  {
    testLog("state: eatCppComment");
    return _pEatCppComment;
  }
  if (currChar == '/' && chNext == '*')
  {
    testLog("state: eatCComment");
    return _pEatCComment;
  }
  if (currChar == '\n')
  {
    testLog("state: eatNewLine");
    return _pEatNewline;
  }
  // Add '_' to Alphanum
  if (currChar == '_' || std::isalnum(currChar))
  {
    testLog("state: eatAlphanum");
    return _pEatAlphanum;
  }
  if (currChar == '"')
  {
	  testLog("state: eatQuotedString");
	  return _pEatQuotedString;
  }
  if (ispunct(currChar))
  {
    testLog("state: eatPunctuator");
    return _pEatPunctuator;
  }

  if (!_pIn->good())
  {
    //std::cout << "\n  end of stream with currChar = " << currChar << "\n\n";
    return _pEatWhitespace;
  }
  throw(std::logic_error("invalid type"));
}

class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating whitespace";
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};

class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C++ comment";
    do {
		token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '\n');
  }
};

class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C comment";
    do {
		token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '*' || _pIn->peek() != '/');
	token += currChar;
	currChar = _pIn->get();
	token += currChar;
	currChar = _pIn->get();
  }
};

// determine if the punctuators are specail double, single characters or normal punctuators
class EatPunctuator : public ConsumeState
{
private:
	std::string currStr;
	std::string nextStr;
	std::string mergeStr;
	std::vector<std::string>::iterator doubleStr;
	std::vector<std::string>::iterator singleStr;
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating punctuator";
    do {
		nextChar = _pIn->peek();
		currStr = currChar;
		nextStr = nextChar;
		mergeStr = currStr + nextStr;
		singleStr = find(setSpecialSingleChar.begin(), setSpecialSingleChar
			.end(), currStr);
		doubleStr = find(setSpecialDoubleChar.begin(), setSpecialDoubleChar.end(), mergeStr);
		if (doubleStr != setSpecialDoubleChar.end())
		{
			token += mergeStr;
			currChar = _pIn->get();
			currChar = _pIn->get();
			return;
		}
		if (singleStr != setSpecialSingleChar.end())
		{
			token += currChar;
			currChar = _pIn->get();
			return;
		}
		token += currChar;
		currChar = _pIn->get();
	} while (ispunct(currChar));
  }
};

class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar == '_' || isalnum(currChar));
	if (token == "else" && currChar == ' ')
	{
		token += currChar;
		nextChar = _pIn->peek();
		if (nextChar == 'i')
		{
			token += nextChar;
			currChar = _pIn->get();
			currChar = _pIn->get();
			token += currChar;
			currChar = _pIn->get();
		}
	}
	if ((token == "public" || token == "private" || token == "protected") && currChar == ':') {
		token += currChar;
		currChar = _pIn->get();

	}
  }
};

class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

class EatQuotedString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		do {	
		nextChar = _pIn->peek();
		if (currChar == '\\' && nextChar == '"')	// if detected "\" inside a quoted string, determine if the next is """, 
		{											// if so, don't terminate at this """.
		//token += nextChar;
		currChar = _pIn->get();
		}
		token += currChar;
		currChar = _pIn->get();
		} while (currChar != '"');
		token += currChar;
		currChar = _pIn->get();
	}
};



ConsumeState::ConsumeState()
{
  //first = true;
  if (c_first)
  {
    c_first = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
	_pEatQuotedString = new EatQuotedString();
    _pEatNewline = new EatNewline();
    _pState = _pEatWhitespace;
  }
}

ConsumeState::~ConsumeState()
{
  //first = true;
  if (d_first)
  {
    d_first = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pEatQuotedString;
  }
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

void Toker::reset() { pConsumer->resetFirst(); }

bool Toker::setSingleSpecial(std::string str, std::vector<std::string> newV)
{
	if (str == "append")
	{
		setSpecialSingleChar.insert(setSpecialSingleChar.end(), newV.begin(), newV.end());	//might incurr duplicate elements
		return true;
	}
	else if (str == "replace")
	{
		setSpecialSingleChar.clear();
		setSpecialSingleChar = newV;
		return true;
	}
	return false;
}

bool Toker::setSingleSpecial(std::string str)
{
	if (str == "reset")
	{
		setSpecialSingleChar = { "[", "]", "(", ")", "{", "}", ",", ":", ";", "*", "=", "#", ".", "?", "&", "+", "-", "~", "!", "?", "%", "<", ">", "^", "|", "'" };
		return true;
	}
	return false;
}

bool Toker::setDoubleSpecial(std::string str, std::vector<std::string> newV)
{
	if (str == "append")
	{
		setSpecialDoubleChar.insert(setSpecialDoubleChar.end(), newV.begin(), newV.end());	// might incurr duplicate elements
		return true;
	}
	else if (str == "replace")
	{
		//setSpecialDoubleChar.clear();
		setSpecialDoubleChar = newV;
		return true;
	}
	return false;
}

bool Toker::setDoubleSpecial(std::string str)
{
	if (str == "reset")
	{
		setSpecialDoubleChar = { "*=","?=","%=","+=","-=","&=","?=","|=","->","++","--","##","<<",">>","!=","<=",">=","==","&&","||", "};" };
		return true;
	}
	return false;
}

bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}

std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  //std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../TokenizerAlone/Test.txt";

  std::ifstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  Toker toker;
  toker.attach(&in);
  do
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  } while (in.good());

  std::cout << "\n\n";
  return 0;
}
#endif
