#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "MultiplayerGameMode.generated.h"

class UMultiplayerCommands;

UCLASS()
class MULTIPLAYER_API AMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	static const FName SYSTEMS_WORLD;
	static const FName LEVEL_NAME;

private:
	virtual void InitGame(const FString& MapName,
		const FString& Options,
		FString& ErrorMessage) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY(EditAnywhere)
	UMultiplayerCommands* MultiplayerCommands = nullptr;

	UPROPERTY()
	TArray<AActor*> PlayersSpawnPoints = {};
};
