#include "Parser.h"
#include <vector>
#include <algorithm>

#include <FungusFlags.h>
#include <FungusLexerTypes.h>

string trim(const string& str) {
    const string whitespace = " \t\n\r";
    size_t start = str.find_first_not_of(whitespace);
    if (start == string::npos)
        return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

// Sépare une chaîne selon un délimiteur
vector<string> split(const string& str, char delim) {
    vector<string> tokens;
    string token;
    istringstream iss(str);
    while (getline(iss, token, delim)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

// Changer les variables en PascalCase
string capitalize(const string& str) {
    if (str.empty())
        return str;
    string res = str;
    res[0] = toupper(res[0]);
    return res;
}

int GlobalParser::main(ifstream& file, File& outFile)
{
    UnrealObject* currentObject = nullptr;
    vector<string> currentFlags; // pour stocker les flags courants dans un groupe
    string ModuleName;

    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (line.substr(0, 6) == "MODULE")
        {
            UnrealObject dummyObj;
            ModuleParserTask parser(file);
            parser.Run(line, dummyObj);
            ModuleName = dummyObj.moduleName;

            if (currentObject)
            {
                currentObject->moduleName = ModuleName;
            }
        }
        // Si on rencontre "Class", extraire nom et classe de base
        if (line.substr(0, 5) == "CLASS") 
        {
            UnrealObject* newObject = new UnrealObject();
            currentObject = newObject;
            newObject->moduleName = ModuleName;
            outFile.objectToGenerate.push_back(newObject);

            ClassParserTask parser(file);
            parser.Run(line, *currentObject);

            m_state = InClass;
            continue;
        }

        if (line.substr(0, 6) == "STRUCT") 
        {
            UnrealObject* newObject = new UnrealObject();
            currentObject = newObject;
            newObject->moduleName = ModuleName;
            outFile.objectToGenerate.push_back(newObject);

            StructParserTask parser(file);
            parser.Run(line, *currentObject);

            m_state = InClass;
            continue;
        }

        // Passage dans la section PROPERTIES
        if (line == "PROPERTIES") 
        {
            m_state = InProperties;
            if (!currentObject)
            {
                cerr << "Erreur : Properties without a class" << endl;
                return 0;
            }

            PropertyGroupParserTask parser(file);
            parser.Run(line, *currentObject);

            continue;
        }
        // Passage dans la section FUNCTIONS
        if (line == "FUNCTIONS") 
        {
            m_state = InFunctions;

            if (!currentObject)
            {
                cerr << "Erreur : Functions without a class" << endl;
                return 0;
            }

            FunctionGroupParserTask parser(file);
            parser.Run(line, *currentObject);

            continue;
        }
        // Fin d'une section
        if (line == "END") {
            if (m_state == InProperties || m_state == InFunctions)
                m_state = InClass;
            continue;
        }
    }

    return 1;
}

void ModuleParserTask::Run(string& startLine, UnrealObject& obj)
{
    istringstream iss(startLine);
    string dummy, moduleName;
    iss >> dummy >> moduleName;
    obj.moduleName = moduleName;
}

void ClassParserTask::Run(string& startLine, UnrealObject& obj)
{
    istringstream iss(startLine);
    string dummy, className, colon, baseClass;
    iss >> dummy >> className >> colon >> baseClass;
    obj.className = className;
    obj.baseClass = baseClass;

    obj.objectType = Type_Class;
}

void StructParserTask::Run(string& startLine, UnrealObject& obj)
{
    istringstream iss(startLine);
    string dummy, className;
    iss >> dummy >> className;
    obj.className = className;

    obj.objectType = Type_Struct;
}

void PropertyGroupParserTask::Run(string& startLine, UnrealObject& obj)
{
    vector<string> currentFlags;
    string& line = startLine;
    bool isInsidePropertyGroup = false;

    while (getline(m_file, line))
    {
        line = trim(line);
        if (line.empty()) continue;

        // Fin d'une section
        if (line == "END") {
            return;
        }

        // Gestion des groupes de flags (lignes commençant par '[')
        if (line.front() == '[' && line.back() == ']')
        {
            currentFlags = split(line.substr(1, line.size() - 2), ',');
            isInsidePropertyGroup = true;

            if (!FlagHelper::CheckPropertyFlags(currentFlags))
            {
                return;
            }
            continue;
        }

        if (isInsidePropertyGroup) 
        {
            // Format attendu : "int health = 100"
            istringstream iss(line);
            string type, name, equalSign, value;
            iss >> type >> name >> equalSign;
            getline(iss, value);
            value = trim(value);
            // Créer une propriété en utilisant les flags courants
            Property prop;
            prop.type = type;
            prop.name = capitalize(name);
            prop.value = value;
            prop.flags = currentFlags;
            obj.properties.push_back(prop);

            const bool hasSet = FlagHelper::HasSetCallbackFlag(currentFlags);
            const bool hasGet = FlagHelper::HasGetCallbackFlag(currentFlags);

            if (hasSet)
            {
                Function newFunction;
                newFunction.returnType = "void";
                newFunction.name = "Set" + prop.name;
                newFunction.flags.push_back("setter");
                if (!FlagHelper::HasSetCallbackFlag_CodeOnly(currentFlags))
                {
                    newFunction.flags.push_back("callable");
                }
                newFunction.paramType = prop.type;

                obj.functions.push_back(newFunction);
            }

            if (hasSet)
            {
                Function newFunction;
                newFunction.returnType = prop.type;
                newFunction.name = "Get" + prop.name;
                newFunction.flags.push_back("getter");
                newFunction.flags.push_back("callable");
                newFunction.paramType;

                obj.functions.push_back(newFunction);
            }
        }
    }
}

void FunctionGroupParserTask::Run(string& startLine, UnrealObject& obj)
{
    vector<string> currentFlags;
    string& line = startLine;
    bool isInsideFunctionGroup = false;

    while (getline(m_file, line))
    {
        line = trim(line);
        if (line.empty()) continue;

        // Fin d'une section
        if (line == "END") {
            return;
        }

        // Gestion des groupes de flags (lignes commençant par '[')
        if (line.front() == '[' && line.back() == ']')
        {
            currentFlags = split(line.substr(1, line.size() - 2), ',');
            isInsideFunctionGroup = true;

            if (!FlagHelper::CheckFunctionsFlags(currentFlags))
            {
                return;
            }
            continue;
        }

        if (isInsideFunctionGroup)
        {
            // Format attendu : "ReturnType Name:ParamType"
            //TODO:handle more than one param

            istringstream iss(line);
            string returnType, funcAndParam;
            iss >> returnType >> funcAndParam;
            // Séparer le nom de la fonction et le type de paramètre à l'aide du ':'
            size_t pos = funcAndParam.find(":");
            if (pos != string::npos) 
            {
                Function func;
                func.returnType = returnType;
                func.name = funcAndParam.substr(0, pos);
                func.paramType = funcAndParam.substr(pos + 1);
                func.flags = currentFlags;
                obj.functions.push_back(func);
            }
        }
    }
}

void EnumParserTask::Run(string& startLine, UnrealObject& obj)
{
}
