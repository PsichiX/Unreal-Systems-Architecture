#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/System.h"

#include "SpatialInformationSystem.generated.h"

class USpatialGraph;
class USystemsWorld;

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialPropagationSettings
{
	GENERATED_BODY()

	// UPROPERTY(EditAnywhere)
	// double Speed = 1.0;

	// UPROPERTY(EditAnywhere)
	// double Damping = 0.0;
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialInformationSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpatialGraph> ResourceType = {};

	/// Default information propagation settings for any information ID.
	UPROPERTY(EditAnywhere)
	FSpatialPropagationSettings DefaultPropagation = {};

	/// Used to override propagation speed for given information ID.
	UPROPERTY(EditAnywhere)
	TMap<FName, FSpatialPropagationSettings> SpecializedPropagation = {};
};