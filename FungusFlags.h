#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <FungusLexerTypes.h>

using namespace std;


class FlagHelper {
public: 
    static bool CheckPropertyFlags(const vector<string>& flags)
    {
        for (const auto& flag : flags)
        {
            if (flag != "set" && flag != "get" && flag != "update" && flag != "public" && flag != "setcode" && flag != "edit" && flag != "codeonly")
            {
                cerr << "Erreur : " << flag << " est un flag invalide pour une property" << endl;
                return false;
            }
        }

        return true;
    }

    static bool HasSetCallbackFlag(const vector<string>& flags)
    {
        return std::find(flags.begin(), flags.end(), "set") != flags.end();
    }

    static bool HasSetCallbackFlag_CodeOnly(const vector<string>& flags)
    {
        return std::find(flags.begin(), flags.end(), "setcode") != flags.end();
    }

    static bool IsCodeOnly(const vector<string>& flags)
    {
        return std::find(flags.begin(), flags.end(), "codeonly") != flags.end();
    }

    static bool HasGetCallbackFlag(const vector<string>& flags)
    {
        return std::find(flags.begin(), flags.end(), "get") != flags.end();
    }

    static bool HasUpdateFlag(const vector<string>& flags)
    {
        return std::find(flags.begin(), flags.end(), "update") != flags.end();
    }

    static bool CheckFunctionsFlags(const vector<string>& flags)
    {
        for (const auto& flag : flags)
        {
            if (flag != "callable" && flag != "implementable" && flag != "codeOnly")
            {
                cerr << "Erreur : " << flag << " est un flag invalide pour une function" << endl;
                return false;
            }
        }

        return true;
    }

    static string getUPropertyAttributes(const vector<string>& flags, bool& ShouldAddSpecifiers, const Property& inProperty) {
        ostringstream oss;
        // Par défaut, on pourrait considérer un comportement si aucun flag spécifique n'est défini.
        bool hasPublic = false;
        ShouldAddSpecifiers = true;

        // Parcourir la liste des flags une seule fois
        for (const auto& flag : flags) {

            //return si c'est code only
            if (flag == "codeonly") {
                ShouldAddSpecifiers = false;
                return oss.str();
            }
            if (flag == "edit") {
                oss << "EditAnywhere, ";
            }
            else if (flag == "set") {
                oss << "BlueprintSetter = " << "Set" << inProperty.name << ", ";
            }
            else if (flag == "get")
            {
                oss << "BlueprintGetter = " << "Get" << inProperty.name << ", ";
            }
            else if (flag == "public") {
                oss << "BlueprintReadWrite, ";
                hasPublic = true;
            }
            // On peut ajouter d'autres conditions pour d'autres flags.
            // Par exemple, pour "get", "update", etc.
        }

        // Si aucun flag "edit" n'a été trouvé, on peut définir un comportement par défaut (par ex. BlueprintReadOnly)
        if (!hasPublic)
            oss << "BlueprintReadOnly";

        string result = oss.str();
        // Supprimer la dernière virgule et espace s'ils existent
        if (result.size() >= 2 && (result[result.size() - 2] == ','))
            result = result.substr(0, result.size() - 2);
        return result;
    }

    static string getUFunctionAttributes(const vector<string>& flags, bool& ShouldAddSpecifiers) {
        ostringstream oss;
        ShouldAddSpecifiers = true;

        for (const auto& flag : flags) {
            if (flag == "callable") {
                oss << "BlueprintCallable, ";
            }
            else if (flag == "implementable") {
                oss << "BlueprintImplementable, ";
            }
            else if (flag == "codeOnly") {
                ShouldAddSpecifiers = false;
                return oss.str();
            }
            else if (flag == "setter")
            {
                oss << "BlueprintSetter, ";
            }
            else if (flag == "getter")
            {
                oss << "BlueprintGetter, ";
            }
            // Ajoutez d'autres cas si nécessaire.
        }

        string result = oss.str();
        // Supprimer la dernière virgule et espace s'ils existent
        if (result.size() >= 2 && (result[result.size() - 2] == ','))
            result = result.substr(0, result.size() - 2);
        return result;
    }
};
