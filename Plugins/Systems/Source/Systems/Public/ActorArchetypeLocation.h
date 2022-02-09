#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"

#include "ActorArchetypeLocation.generated.h"

USTRUCT()
struct SYSTEMS_API FActorArchetypeLocation
{
	GENERATED_BODY();

	UPROPERTY()
	FArchetypeSignature Signature = {};

	UPROPERTY()
	uint32 Index = 0;
};
