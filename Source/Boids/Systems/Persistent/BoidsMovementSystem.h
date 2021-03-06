#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SharedChannel.h"
#include "Systems/Public/System.h"

#include "Boids/Resources/GameEvents.h"

#include "BoidsMovementSystem.generated.h"

class USystemsWorld;

UCLASS()
class BOIDS_API UBoidsMovementSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Init(USystemsWorld& Systems) override;

	virtual void Run(USystemsWorld& Systems) override;

private:
	TReceiverChannel<FMovementStep> MovementStep = {};
};
