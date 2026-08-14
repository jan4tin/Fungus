#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonActeur.generated.h"

UCLASS()
class GPP_API AMonActeur : public AActor
{
#pragma region FUNGUS : MonActeur DECLARATION

    GENERATED_BODY()

public:
    AMonActeur();

    // === BEGIN AUTOGEN FUNCTIONS ===
    
    void Hello(string inValue);
    UFUNCTION(BlueprintCallable)
    void AskHello(bool inValue);
    // === END AUTOGEN FUNCTIONS ===

protected:
virtual void BeginPlay() override;

private:

    // === BEGIN AUTOGEN PROPERTIES ===
    UPROPERTY(BlueprintReadOnly)
    int Health = 100;
    UPROPERTY(BlueprintReadOnly)
    float Mana = 0.0f;
    UPROPERTY(BlueprintReadOnly)
    bool IsDead = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EaseTiming = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Add = 0;;
    // === END AUTOGEN PROPERTIES ===
#pragma endRegion // FUNGUS : MonActeur DECLARATION

};