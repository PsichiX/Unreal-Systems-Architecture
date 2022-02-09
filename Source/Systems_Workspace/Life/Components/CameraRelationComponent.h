#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "CameraRelationComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UCameraRelationComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsVisible = false;

	UPROPERTY()
	float Distance = 0;

	UPROPERTY()
	FVector Difference = FVector(0);
};
