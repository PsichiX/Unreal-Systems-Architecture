#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "Systems/Public/SystemsGameMode.h"

#include "MultiplayerGameMode.generated.h"

UCLASS()
class MULTIPLAYER_API AMultiplayerGameMode : public ASystemsGameMode
{
	GENERATED_BODY()

private:
	virtual void InitGame(const FString& MapName,
		const FString& Options,
		FString& ErrorMessage) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY()
	TArray<AActor*> PlayersSpawnPoints = {};
};
