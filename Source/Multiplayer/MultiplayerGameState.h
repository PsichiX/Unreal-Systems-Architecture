#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "MultiplayerGameState.generated.h"

UCLASS()
class MULTIPLAYER_API AMultiplayerGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
