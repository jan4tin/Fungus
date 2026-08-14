USTRUCT()
struct {%if length(ModuleName) > 0 %}{{ModuleName}}_API{% endif %} S{{ClassName}}
{
#pragma region FUNGUS : {{ClassName}} DECLARATION

	GENERATED_STRUCT_BODY();
public:

    {{ClassName}}();

{% include "function.header.tpl" %}

{% include "properties.header.tpl" %}

#pragma endRegion // FUNGUS : {{ClassName}} DECLARATION

}