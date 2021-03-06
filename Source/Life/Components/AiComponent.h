#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "Life/AI/Reasoner/UtilityAiBehavior.h"
#include "Life/AI/Reasoner/UtilityAiReasoner.h"

#include "AiComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API UAiComponent : public USystemsActorComponent
{
	GENERATED_BODY()

	friend void AiSystem(USystemsWorld& Systems);

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UUtilityAiBehavior* Behavior = nullptr;

	UPROPERTY()
	float DecideAccum = 0;

private:
	void TryDecide(USystemsWorld& Systems, float Threshold, float DeltaTime);

	UPROPERTY()
	UUtilityAiReasoner* Reasoner = nullptr;
};
