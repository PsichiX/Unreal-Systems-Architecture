#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "DebugSpatialInformationSystem.generated.h"

class USpatialInformation;

USTRUCT(BlueprintType)
struct FDebugSpatialInformationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::MakeRandomColor().WithAlpha(10.0);

	UPROPERTY(EditAnywhere)
	double Scale = 1.0;
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API UDebugSpatialInformationSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

private:
	UPROPERTY(EditAnywhere)
	int SphereSegments = 32;

	UPROPERTY(EditAnywhere)
	TMap<FName, FDebugSpatialInformationData> InformationColors = {};

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialInformation> ResourceType = {};
};