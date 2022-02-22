#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Metronome.h"
#include "Systems/Public/SystemsComponent.h"

#include "FaceCameraComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API UFaceCameraComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FMetronome Metronome = {};
};
