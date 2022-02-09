#pragma once

#include "CoreMinimal.h"

#include "LifeSpawner.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_WORKSPACE_API ULifeSpawner : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UClass* SelectedToSpawn = nullptr;

	UFUNCTION()
	bool SpawnSelected(FVector Location);
};
