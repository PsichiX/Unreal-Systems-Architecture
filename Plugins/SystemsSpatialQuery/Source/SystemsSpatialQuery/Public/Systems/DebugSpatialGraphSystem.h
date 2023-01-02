#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "DebugSpatialGraphSystem.generated.h"

class USystemsWorld;
class USpatialGraph;

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API UDebugSpatialGraphSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

private:
	UPROPERTY(EditAnywhere)
	bool bDrawNodes = true;

	UPROPERTY(EditAnywhere)
	FColor NodesColor = FColor::Red;

	UPROPERTY(EditAnywhere)
	bool bDrawConnections = true;

	UPROPERTY(EditAnywhere)
	FColor ConnectionsColor = FColor::Yellow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialGraph> ResourceType = {};
};
