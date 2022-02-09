#pragma once

#include "CoreMinimal.h"

#include "LifeDebugSystems.generated.h"

UCLASS(BlueprintType)
class SYSTEMS_WORKSPACE_API ULifeDebugSystems : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bShowFollowPaths = false;
};
