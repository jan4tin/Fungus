#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

struct UnrealObject;
struct File;
using namespace std;

//Task interface
class ParserTask
{
public:
	virtual ~ParserTask() {};

	virtual void Run(string& startLine, UnrealObject& obj) = 0;
protected:
	ParserTask(ifstream& file) : m_file(file) {};

	ifstream& m_file;
};

class ClassParserTask : public ParserTask
{
	typedef ParserTask super;
public: 
	explicit ClassParserTask(ifstream& file) : super(file) {};

	void Run(string& startLine, UnrealObject& obj) override;
};

class ModuleParserTask : public ParserTask
{
	typedef ParserTask super;
public:
	explicit ModuleParserTask(ifstream& file) : super(file) {};

	void Run(string& startLine, UnrealObject& obj) override;
};

class StructParserTask : public ParserTask
{
	typedef ParserTask super;
public:
	explicit StructParserTask(ifstream& file) : super(file) {};
	void Run(string& startLine, UnrealObject& obj) override;
};

class PropertyGroupParserTask : public ParserTask
{
	typedef ParserTask super;
public:
	explicit PropertyGroupParserTask(ifstream& file) : super(file) {};
	void Run(string& startLine, UnrealObject& obj) override;
};

class FunctionGroupParserTask : public ParserTask
{
	typedef ParserTask super;
public:
	explicit FunctionGroupParserTask(ifstream& file) : super(file) {};
	void Run(string& startLine, UnrealObject& obj) override;
};

class EnumParserTask : public ParserTask // va etre plus complexe
{
	typedef ParserTask super;
public:
	explicit EnumParserTask(ifstream& file) : super(file) {};
	void Run(string& startLine, UnrealObject& obj) override;
};


class GlobalParser
{
public:

	enum ParserState
	{
		None, 
		InClass, 
		InProperties, 
		InFunctions, 
		InPropertyGroup, 
		InFunctionGroup
	};

	GlobalParser() {};
	~GlobalParser() {};

	int main(ifstream& file, File& outFile);

private:
	ParserState m_state = ParserState::None;
};
