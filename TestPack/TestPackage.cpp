#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;
int main()
{
	/*std::fstream fIn;
	fIn.open("C:\\Users\thzha_000\Desktop\finaltest.txt", std::ios::in);*/

	//Tokenizer testing
	std::string fileSpec = "../Tokenizer/test_R4.txt";

	std::cout << "---------------------------------------" << "\n";
	std::cout << "---------  Testing Tokenizer ----------" << "\n";
	std::cout << "---------------------------------------" << "\n";
	std::cout << "--------Operating authentic code-------" << "\n";
	std::cout << "Requirements 5 is well shown in this process" << "\n\n";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	else
		std::cout << "\n Read file successfull!" << "\n";
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

	toker.reset();


	// SemiExp testing
	Toker toker2;
	std::cout << "---------------------------------------" << "\n";
	std::cout << "------- Testing SemiExpression --------" << "\n";
	std::cout << "---------------------------------------" << "\n";
	std::cout << "--------Operating authentic code-------" << "\n\n";
	std::cout << "Requirements 6-8 is well shown in this process" << "\n\n";

	std::fstream in2(fileSpec);
	if (!in2.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	else
		std::cout << "\n Read file successfull!" << "\n";
	toker2.attach(&in2);
	SemiExp semi(&toker2);
	while (semi.get())
	{
		std::cout << "\n  -------------- semiExpression --------------";
		semi.show();
	}

	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -------------- semiExpression --------------";
		semi.show();
		std::cout << "\n\n";
	}

	toker2.reset();

	// Test Requirement 4 

	std::string fileSpec_R4 = "../Tokenizer/test_R4.txt";

	std::vector<std::string> alter_setSpecialSingleChar = { "=", "+" };
	std::vector<std::string> alter_setSpecialDoubleChar = { "++" , "--" };

	std::cout << "------------------- Requiement 4 ------------------" << "\n";
	std::cout << "---------------------------------------------------" << "\n";
	std::cout << "------Using a small piece of text for testing------" << "\n\n";
	Toker toker1;
	std::ifstream in1(fileSpec_R4);
	if (!in1.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	else
		std::cout << "\n Read file successfull!" << "\n";
	toker.attach(&in1);
	do
	{
		std::string tok = toker1.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in1.good());
	std::cout << "\n\n";

	toker1.reset();		// reset static member in consumestate class

	std::cout << "-----setSpecialSingleChars setSpecialCharPairs-----" << "\n";
	std::cout << "---------------------------------------------------" << "\n\n";
	Toker toker3;
	std::cout << " change the default set of single special chars to: { \" = \", \"+ \" } " << "\n\n";
	std::cout << " change the default set of single special chars to: { \"++ \" , \"--\" } " << "\n\n";
	std::cout << "**After the change, Tokernizer is not able to detect += as a combined punctuator.**" << "\n\n";
	std::cout << "This change can be seen at last line 6 and 7." << "\n\n";

	toker3.setSingleSpecial("replace", alter_setSpecialSingleChar);
	toker3.setDoubleSpecial("replace", alter_setSpecialDoubleChar);
	std::ifstream in3(fileSpec_R4);
	if (!in3.good())
	{
		std::cout << "\n  can't open " << fileSpec_R4 << "\n\n";
		return 1;
	}
	else
		std::cout << "\n Read file successfull!" << "\n";
	toker.attach(&in3);
	do
	{
		std::string tok = toker3.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in3.good());
	std::cout << "\n\n";

	toker3.reset();
	return 0;
}