// === BEGIN AUTOGEN FUNCTIONS ===
## for function in functions 
    {%if function.shouldAddSpecifiers %}UFUNCTION({{ function.attributes }}){% endif %}
    {{ function.return_type }} {{ function.name }}({%if length(function.param_type) > 0 %} {{ function.param_type }} inValue {% endif %});

## endfor
// === END AUTOGEN FUNCTIONS ===