#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"

#include "ConsumedActorComponents.generated.h"

class UActorComponent;

USTRUCT()
struct SYSTEMS_API FConsumedActorComponents
{
	GENERATED_BODY();

	UPROPERTY()
	FArchetypeSignature Signature = {};

	UPROPERTY()
	TArray<UActorComponent*> Components = {};
};
