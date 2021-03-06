#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
/////////////////////////////////////////////////////////////////////
// itokcollection.h -                   //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Prototype for CSE687 Pr1,						   //
//				CSE687 - Object Oriented Design					   //
// Author:      Tianhang Zhang, Syracuse University                //
//              tzhan116@syr.edu	                               //
// Src Package: Jim Fawcett										   //
/////////////////////////////////////////////////////////////////////
/*
  Module Purpose:
  ===============
  ITokCollection is an interface that supports substitution of different
  types of scanners for parsing.  In this solution, we illustrate that
  by binding two different types of collections, SemiExp and XmlParts,
  to this interface.  This is illustrated in the test stubs for the
  SemiExpression and XmlElementParts modules.

  Maintenance History:
  ====================
  ver 1.1 : 02 Jun 11
  - added merge, remove overload, and default param in get
  ver 1.0 : 17 Jan 09
  - first release
*/

struct ITokCollection
{
  virtual bool get()=0;
  virtual size_t length()=0;
  virtual std::string operator[](size_t n)=0;
  virtual size_t find(const std::string& tok)=0;
  virtual void push_back(const std::string& tok)=0;
  //virtual bool merge(const std::string& firstTok, const std::string& secondTok)=0;
  virtual bool remove(const std::string& tok)=0;
  virtual bool remove(size_t i)=0;
  virtual void toLower()=0;
  virtual void trimFront(std::string& token)=0;
  virtual void clear()=0;
  virtual void show()=0;
  virtual ~ITokCollection() {};
};

#endif
