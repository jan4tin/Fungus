#include "StructTest.h"

AStructTest::AStructTest() {}

void AStructTest::BeginPlay()
{
  Super::BeginPlay();
}

void AStructTest::SetIsAStruct(bool newValue)
{
  if(newValue != IsAStruct)
  {
      IsAStruct = newValue;
  }
}

void AStructTest::SetIsAStruct(bool inValue)
{
  // To implement
}

bool AStructTest::GetIsAStruct( inValue)
{
  // To implement
}

