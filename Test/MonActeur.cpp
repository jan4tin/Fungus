#include "MonActeur.h"

AMonActeur::AMonActeur() {}

void AMonActeur::BeginPlay()
{
  Super::BeginPlay();
}

void AMonActeur::SetHealth(int newValue)
{
  if(newValue != Health)
  {
      Health = newValue;
      UPDATE_EVENT(Health);
  }
}

void AMonActeur::SetMana(float newValue)
{
  if(newValue != Mana)
  {
      Mana = newValue;
      UPDATE_EVENT(Mana);
  }
}

void AMonActeur::SetIsDead(bool newValue)
{
  if(newValue != IsDead)
  {
      IsDead = newValue;
      UPDATE_EVENT(IsDead);
  }
}

void AMonActeur::Hello(string inValue)
{
  // To implement
}

void AMonActeur::AskHello(bool inValue)
{
  // To implement
}

