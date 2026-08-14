#pragma once

#include <vector>
#include <string>

using namespace std;

//enum représentant le type de l'object
enum TypeIdentifier : uint8_t {
    Type_Class,
    Type_Struct,
    Type_Enum
};

// Structure représentant une propriété
struct Property {
    string type;
    string name;
    string value;
    vector<string> flags;
};

// Structure représentant une fonction
struct Function {
    string returnType;
    string name;
    string paramType; // type du paramètre unique (ex. "string" ou "bool")
    vector<string> flags;
};

// Structure représentant la classe à générer
struct UnrealObject {
    TypeIdentifier objectType;
    string className;
    string baseClass;
    string moduleName;
    vector<Property> properties;
    vector<Function> functions;
    vector<string> existingCode; //not handled yet
};

struct File{
    vector<UnrealObject*> objectToGenerate;
    vector<string> forwardDeclarations;
};