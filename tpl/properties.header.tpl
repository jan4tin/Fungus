// === BEGIN AUTOGEN PROPERTIES ===
## for property in properties
    {%if property.shouldAddSpecifiers %}UPROPERTY({{ property.attributes }}){% endif %}
    {{ property.type }} {{ property.name }} = {{ property.value }};

## endfor
// === END AUTOGEN PROPERTIES ===