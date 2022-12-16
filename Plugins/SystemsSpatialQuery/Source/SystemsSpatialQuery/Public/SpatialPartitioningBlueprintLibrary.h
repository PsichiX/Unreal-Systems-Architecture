#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialPartitioning.h"

#include "SpatialPartitioningBlueprintLibrary.generated.h"

class USystemsWorld;

UENUM(BlueprintType)
enum class ESpatialPartitioningStatusBranch : uint8
{
	Success,
	Failure,
};

UCLASS()
class SYSTEMSSPATIALQUERY_API USpatialPartitioningBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static TArray<AActor*> GetActorsInArea(USpatialPartitioning* Target, const FArea& Area);

	UFUNCTION(BlueprintPure)
	static TArray<AActor*> GetActorsInRadius(USpatialPartitioning* Target, FVector Center, float Radius);

	UFUNCTION(BlueprintPure, Meta = (ExpandEnumAsExecs = "Branches", AutoCreateRefTerm = "Ignored"))
	static AActor* FindClosestActor(USpatialPartitioning* Target,
		FVector Position,
		const TArray<AActor*>& Ignored,
		ESpatialPartitioningStatusBranch& Branches);
};