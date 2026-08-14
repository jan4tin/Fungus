#pragma once

#include "CoreMinimal.h"
#include "{{BaseInclude}}"
#include "{{ClassName}}.generated.h"

UCLASS()
class {%if length(ModuleName) > 0 %}{{ModuleName}}_API{% endif %} {{ClassPrefix}}{{ClassName}} : public {{BaseClass}}
{
#pragma region FUNGUS : {{ClassName}} DECLARATION

    GENERATED_BODY();

public:
    {{ClassPrefix}}{{ClassName}}();

{% include "function.header.tpl" %}

protected:
{{InitFunction}}

private:

{% include "properties.header.tpl" %}

#pragma endRegion // FUNGUS : {{ClassName}} DECLARATION

};