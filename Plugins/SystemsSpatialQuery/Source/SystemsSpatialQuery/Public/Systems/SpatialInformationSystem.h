#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "SpatialInformationSystem.generated.h"

class USpatialGraph;
class USystemsWorld;
class USpatialInformation;
class USpatialGraph;

UENUM(BlueprintType)
enum class ESpatialPropagationFixedStepFrequency : uint8
{
	None = 0,
	/// Unbearable
	Hertz1 = 1,
	/// Ultra Low
	Hertz5 = 5,
	/// Semi Low
	Hertz10 = 10,
	/// Low
	Hertz15 = 15,
	/// Suboptimal
	Hertz20 = 20,
	/// Optimal
	Hertz30 = 30,
	/// Medium
	Hertz45 = 45,
	/// High
	Hertz60 = 60,
	/// Semi High
	Hertz90 = 90,
	/// Ultra High
	Hertz120 = 120,
};

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialPropagationSettings
{
	GENERATED_BODY()

	/// Speed of propagation in Unreal spatial units per second.
	UPROPERTY(EditAnywhere)
	double Speed = 100.0;

	UPROPERTY(EditAnywhere)
	double DampingFactor = 0.0;
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialInformationSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

private:
	const FSpatialPropagationSettings& FindPropagationSettings(FName Id) const;

	void PerformStep(double DeltaTime, USpatialInformation* Information, const USpatialGraph* Graph) const;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialGraph> ResourceType = {};

	/// Default information propagation settings for any information ID.
	UPROPERTY(EditAnywhere)
	FSpatialPropagationSettings DefaultPropagation = {};

	/// Used to override propagation speed for given information ID.
	UPROPERTY(EditAnywhere)
	TMap<FName, FSpatialPropagationSettings> SpecializedPropagation = {};

	UPROPERTY(EditAnywhere)
	ESpatialPropagationFixedStepFrequency FixedStepFrequency = ESpatialPropagationFixedStepFrequency::None;

	UPROPERTY(EditAnywhere,
		Meta = (EditCondition = "FixedStepFrequency != ESpatialPropagationFixedStepFrequency::None"))
	uint32 FixedStepSubstepsLimit = 1;

	UPROPERTY()
	double FixedStepAccumulator = 0.0;
};