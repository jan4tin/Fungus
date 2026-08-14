//File begin


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI.generated.h"

UCLASS()
class UI_API UUI : public UUserWidget
{
#pragma region FUNGUS : UI DECLARATION

    GENERATED_BODY();

public:
    UUI();

// === BEGIN AUTOGEN FUNCTIONS ===
    UFUNCTION(BlueprintSetter, BlueprintCallable)
    void SetHealth( int inValue );

    UFUNCTION(BlueprintGetter, BlueprintCallable)
    int GetHealth();

    UFUNCTION(BlueprintSetter, BlueprintCallable)
    void SetMana( float inValue );

    UFUNCTION(BlueprintGetter, BlueprintCallable)
    float GetMana();

    
    void Hello( string inValue );

    UFUNCTION(BlueprintCallable)
    void AskHello( bool inValue );

// === END AUTOGEN FUNCTIONS ===

protected:
virtual void NativeConstruct() override;

private:

// === BEGIN AUTOGEN PROPERTIES ===
    UPROPERTY(BlueprintSetter = SetHealth, BlueprintGetter = GetHealth, BlueprintReadOnly)
    int Health = 100;

    UPROPERTY(BlueprintSetter = SetMana, BlueprintGetter = GetMana, BlueprintReadOnly)
    float Mana = 0.0f;

    UPROPERTY(BlueprintGetter = GetIsDead, BlueprintReadOnly)
    bool IsDead = false;

    UPROPERTY(BlueprintGetter = GetEaseTiming, EditAnywhere, BlueprintReadOnly)
    float EaseTiming = 0.0f;

    UPROPERTY(BlueprintGetter = GetAdd, EditAnywhere, BlueprintReadOnly)
    int Add = 0;;

// === END AUTOGEN PROPERTIES ===

#pragma endRegion // FUNGUS : UI DECLARATION

};

//File End