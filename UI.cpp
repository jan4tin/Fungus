#include "UI.h"

UUI::UUI() {}

void UUI::NativeConstruct()
{
  Super::NativeConstruct();
}

void UUI::SetHealth(int newValue)
{
  if(newValue != Health)
  {
      Health = newValue;
      UPDATE_EVENT(Health);
  }
}

void UUI::SetMana(float newValue)
{
  if(newValue != Mana)
  {
      Mana = newValue;
      UPDATE_EVENT(Mana);
  }
}

void UUI::SetIsDead(bool newValue)
{
  if(newValue != IsDead)
  {
      IsDead = newValue;
      UPDATE_EVENT(IsDead);
  }
}

void UUI::SetHealth(int inValue)
{
  // To implement
}

int UUI::GetHealth( inValue)
{
  // To implement
}

void UUI::SetMana(float inValue)
{
  // To implement
}

float UUI::GetMana( inValue)
{
  // To implement
}

void UUI::Hello(string inValue)
{
  // To implement
}

void UUI::AskHello(bool inValue)
{
  // To implement
}

