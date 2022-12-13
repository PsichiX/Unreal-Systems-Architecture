#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"
#include "Systems/Public/SystemsGameState.h"

#include "MultiplayerGameState.generated.h"

UCLASS()
class MULTIPLAYER_API AMultiplayerGameState : public ASystemsGameState
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;
};
