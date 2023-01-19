#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "DebugSpatialDiscretizationSystem.generated.h"

class USpatialDiscretization;

UCLASS(BlueprintType)
class UDebugSpatialDiscretizationSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

private:
	UPROPERTY(EditAnywhere)
	FColor TrianglesColor = FColor::Yellow.WithAlpha(10);

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialDiscretization> ResourceType = {};
};