//File begin

USTRUCT()
struct UI_API SStructTest
{
#pragma region FUNGUS : StructTest DECLARATION

	GENERATED_STRUCT_BODY();
public:

    StructTest();

// === BEGIN AUTOGEN FUNCTIONS ===
    UFUNCTION(BlueprintSetter, BlueprintCallable)
    void SetIsAStruct( bool inValue );

    UFUNCTION(BlueprintGetter, BlueprintCallable)
    bool GetIsAStruct();

// === END AUTOGEN FUNCTIONS ===

// === BEGIN AUTOGEN PROPERTIES ===
    UPROPERTY(BlueprintSetter = SetIsAStruct, BlueprintGetter = GetIsAStruct, BlueprintReadOnly)
    bool IsAStruct = true;;

// === END AUTOGEN PROPERTIES ===

#pragma endRegion // FUNGUS : StructTest DECLARATION

}


//File End