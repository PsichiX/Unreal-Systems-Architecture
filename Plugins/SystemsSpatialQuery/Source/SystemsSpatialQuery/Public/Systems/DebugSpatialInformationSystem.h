#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "DebugSpatialInformationSystem.generated.h"

class USpatialInformation;

UENUM()
enum class EDebugSpatialInformationDrawMode : uint8
{
	Scale,
	Color,
};

USTRUCT(BlueprintType)
struct FDebugSpatialInformationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EDebugSpatialInformationDrawMode DrawMode = EDebugSpatialInformationDrawMode::Scale;

	UPROPERTY(EditAnywhere,
		Meta = (EditCondition = "DrawMode == EDebugSpatialInformationDrawMode::Scale", EditConditionHides))
	FColor Color = FColor::MakeRandomColor().WithAlpha(10.0);

	UPROPERTY(EditAnywhere,
		Meta = (EditCondition = "DrawMode == EDebugSpatialInformationDrawMode::Scale", EditConditionHides))
	double Scale = 1.0;

	UPROPERTY(EditAnywhere,
		Meta = (EditCondition = "DrawMode == EDebugSpatialInformationDrawMode::Color", EditConditionHides))
	double ValueRangeLimit = 1.0;

	UPROPERTY(EditAnywhere,
		Meta = (EditCondition = "DrawMode == EDebugSpatialInformationDrawMode::Color", EditConditionHides))
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
	TMap<FName, FDebugSpatialInformationData> InformationColors = {};

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialInformation> ResourceType = {};
};