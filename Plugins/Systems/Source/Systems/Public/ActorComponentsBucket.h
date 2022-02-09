#pragma once

#include "CoreMinimal.h"

#include "ActorComponentsBucket.generated.h"

class UActorComponent;

USTRUCT()
struct SYSTEMS_API FActorComponentsBucket
{
	GENERATED_BODY()

public:
	void Add(UActorComponent* Component);

	void Remove(UActorComponent* Component);

	bool IsEmpty() const
	{
		//// [ignore]
		return this->Added.Num() <= 0 && this->Removed.Num() <= 0;
		//// [/ignore]
	}

	TArray<UActorComponent*> GetAdded() const
	{
		//// [ignore]
		return this->Added;
		//// [/ignore]
	}

	TArray<uint32> GetRemoved() const
	{
		//// [ignore]
		return this->Removed.Array();
		//// [/ignore]
	}

	UPROPERTY()
	AActor* Actor = nullptr;

private:
	UPROPERTY()
	TArray<UActorComponent*> Added = {};

	UPROPERTY()
	TSet<uint32> Removed = {};
};
