// Fungus.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

#include <FungusLexerTypes.h>
#include <Parser.h>

#include <FungusFlags.h>

using namespace std;

nlohmann::json buildJsonContext(const UnrealObject& cls) {
    nlohmann::json ctx;

    const bool isWidget = cls.baseClass == "UserWidget";

    ctx["ClassName"] = cls.className;
    ctx["BaseClass"] = isWidget ? "UUserWidget" : cls.baseClass;
    ctx["ClassPrefix"] = isWidget ? "U" : "A";
    ctx["BaseInclude"] = isWidget ? "Blueprint/UserWidget.h" : "GameFramework/Actor.h";
    ctx["InitFunction"] = isWidget ? "virtual void NativeConstruct() override;" : "virtual void BeginPlay() override;";
    ctx["ModuleName"] = cls.moduleName;
    ctx["Type"] = cls.objectType;

    // propriétés
    for (const auto& p : cls.properties) 
    {
        bool ShouldAddSpecifiers = true;
        const string& Specifiers = FlagHelper::getUPropertyAttributes(p.flags, ShouldAddSpecifiers, p);

        ctx["properties"].push_back({
            { "type", p.type },
            { "name", p.name },
            { "value", p.value },
            { "shouldAddSpecifiers", ShouldAddSpecifiers},
            { "attributes", Specifiers }
            });
    }

    // fonctions
    for (const auto& f : cls.functions) 
    {
        bool ShouldAddSpecifiers = true;
        const string& Specifiers = FlagHelper::getUFunctionAttributes(f.flags, ShouldAddSpecifiers);

        ctx["functions"].push_back({
            { "name", f.name },
            { "return_type", f.returnType },
            { "param_type", f.paramType },
            { "shouldAddSpecifiers", ShouldAddSpecifiers },
            { "attributes", Specifiers }
            });
    }

    return ctx;
}

// Génération du code header (.h)
string generateHeader(const UnrealObject& cls) 
{   
    inja::Environment env("./tpl/");

    env.set_include_callback([&env](const std::filesystem::path& path, const std::string& template_name) {
        return env.parse(template_name);
        });

   /* env.set_include_callback([&env](const std::filesystem::path& path, const std::string& name) {

        // Chargement normal depuis fichier
        std::ifstream file(path + "/" + name + ".tpl");
        if (!file.is_open()) {
            return "// ERROR: Template not found: " + name;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
        });*/

    auto context = buildJsonContext(cls);

    std::string result = env.render_file("file.header.tpl", context);
    return result;
}

// Génération du code source (.cpp)
string generateCpp(const UnrealObject& cls) {
    ostringstream oss;
    string prefix = (cls.baseClass == "UserWidget") ? "U" : "A";
    oss << "#include \"" << cls.className << ".h\"\n\n";

    // Implémentation du constructeur
    oss << prefix << cls.className << "::" << prefix << cls.className << "() {}\n\n";

    // Implémentation de la fonction d'initialisation (NativeConstruct ou BeginPlay)
    if (cls.baseClass == "UserWidget")
    {
        oss << "void " << prefix << cls.className << "::NativeConstruct()\n{\n";
        oss << "  Super::NativeConstruct();\n";
        oss << "}\n\n";
    }
    else
    {
        oss << "void " << prefix << cls.className << "::BeginPlay()\n{\n";
        oss << "  Super::BeginPlay();\n";
        oss << "}\n\n";
    }

    // Implémentation des setters pour les propriétés qui en ont
    for (const auto& prop : cls.properties) {
        string capName = prop.name;
        // Générer un setter si le flag "set" ou "setcode" est présent
        if (find(prop.flags.begin(), prop.flags.end(), "set") != prop.flags.end() ||
            find(prop.flags.begin(), prop.flags.end(), "setcode") != prop.flags.end())
        {
            oss << "void " << prefix << cls.className << "::Set" << capName << "(" << prop.type
                << " newValue)\n{\n";
            oss << "  if(newValue != " << capName << ")\n  {\n";
            oss << "      " << capName << " = newValue;\n";
            // S'il y a le flag update, on ajoute un appel à UPDATE_EVENT
            if (find(prop.flags.begin(), prop.flags.end(), "update") != prop.flags.end())
                oss << "      UPDATE_EVENT(" << capName << ");\n";
            oss << "  }\n";
            oss << "}\n\n";
        }
    }

    // Implémentation des fonctions définies dans le bloc FUNCTIONS
    for (const auto& func : cls.functions) {
        oss << func.returnType << " " << prefix << cls.className << "::" << func.name
            << "(" << func.paramType << " inValue)\n{\n";
        oss << "  // To implement\n";
        oss << "}\n\n";
    }

    return oss.str();
}

// Analyse du fichier de définition et génération des fichiers .h et .cpp
void parseAndGenerate(ifstream& file) 
{
    GlobalParser glbParser;
    File filetoGenerate;
    if (!glbParser.main(file, filetoGenerate))
    {
        return;
    }

    //PRENDRE LA PREMIERE CLASS POUR LINSTANT 
    const UnrealObject& cls = *filetoGenerate.objectToGenerate[0];

    // Générer le header et le source
    string headerCode = generateHeader(cls);
    string cppCode = generateCpp(cls);

    // Écriture des fichiers de sortie (ex: UI.h et UI.cpp)
    ofstream headerFile(cls.className + ".h");
    if (headerFile.is_open()) {
        headerFile << headerCode;
        headerFile.close();
    }
    else {
        cerr << "Erreur : Impossible d'écrire le fichier header" << endl;
    }

    ofstream cppFile(cls.className + ".cpp");
    if (cppFile.is_open()) {
        cppFile << cppCode;
        cppFile.close();
    }
    else {
        cerr << "Erreur : Impossible d'écrire le fichier source" << endl;
    }

    cout << "Fichiers générés : " << cls.className << ".h et " << cls.className << ".cpp" << endl;
}

int main(int argc, char* argv[]) {

    string filePath;
    // Vérifier si un argument a été passé
    if (argc < 2) {
        cerr << "Erreur : Aucun fichier fourni." << endl;
        //return 1;
        filePath = "NewStructTest.fungus";
    }
    else
    {
        filePath = argv[1];
    }
    //string filePath = argv[1];
    ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << endl;
        return 1;
    }

    cout << "Lecture du fichier : " << filePath << endl;

    parseAndGenerate(inputFile);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
