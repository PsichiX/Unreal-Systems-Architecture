#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Metronome.h"
#include "Systems/Public/SystemsComponent.h"

#include "AgeComponent.generated.h"

class USystemsWorld;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API UAgeComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Value = 100;

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve ScaleMapping = {};

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve SaturationMapping = {};

	UPROPERTY(EditAnywhere)
	FMetronome ApplyToSpriteMetronome = {};

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float RandomMax = 100;
};
