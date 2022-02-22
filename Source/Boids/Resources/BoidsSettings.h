#pragma once

#include "CoreMinimal.h"

#include "BoidsSettings.generated.h"

UENUM(BlueprintType)
enum class EBoidsProximityColorMode : uint8
{
	None,
	Separation,
	Alignment,
	Cohesion,
	Hunt
};

UCLASS(BlueprintType)
class BOIDS_API UBoidsSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TimeScale = 1;

	UPROPERTY(EditAnywhere)
	float SeparationPerceptionRange = 100;

	UPROPERTY(EditAnywhere)
	float AlignmentPerceptionRange = 100;

	UPROPERTY(EditAnywhere)
	float CohesionPerceptionRange = 100;

	UPROPERTY(EditAnywhere)
	float HuntPerceptionRange = 100;

	UPROPERTY(EditAnywhere)
	float VisibilityConeAngleDegrees = 90;

	UPROPERTY(EditAnywhere)
	float AlignmentFactor = 1;

	UPROPERTY(EditAnywhere)
	float CohesionFactor = 1;

	UPROPERTY(EditAnywhere)
	float SeparationFactor = 1;

	UPROPERTY(EditAnywhere)
	float HuntFactor = 1;

	UPROPERTY(EditAnywhere)
	float MinVelocity = 50;

	UPROPERTY(EditAnywhere)
	float MaxVelocity = 50;

	UPROPERTY(EditAnywhere)
	int PopulationNumber = 100;

	UPROPERTY(EditAnywhere)
	EBoidsProximityColorMode ProximityColorMode = EBoidsProximityColorMode::None;

	UPROPERTY(EditAnywhere)
	UClass* BoidClass = nullptr;

	UPROPERTY(EditAnywhere)
	UClass* FoodClass = nullptr;
};
