#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "DebugSpatialInformationSystem.generated.h"

class USpatialInformation;
class USpatialDiscretization;

UENUM()
enum class EDebugSpatialInformationDrawMode : uint8
{
	Scale,
	Color,
	HeightField,
};

USTRUCT(BlueprintType)
struct FDebugSpatialInformationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EDebugSpatialInformationDrawMode DrawMode = EDebugSpatialInformationDrawMode::Scale;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::MakeRandomColor().WithAlpha(10.0);

	UPROPERTY(EditAnywhere)
	double Scale = 1.0;

	UPROPERTY(EditAnywhere)
	double ValueRangeLimit = 1.0;

	UPROPERTY(EditAnywhere)
	double Size = 1.0;
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
	FLinearColor PositiveColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere)
	FLinearColor NeutralColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere)
	FLinearColor NegativeColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere)
	TMap<FName, FDebugSpatialInformationData> InformationData = {};

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialInformation> InformationResourceType = {};

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialDiscretization> DiscretizationResourceType = {};
};