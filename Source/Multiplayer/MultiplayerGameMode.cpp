#include "Multiplayer/MultiplayerGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMultiplayerGameMode::InitGame(const FString& MapName,
	const FString& Options,
	FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(), APlayerStart::StaticClass(), this->PlayersSpawnPoints);
}

AActor* AMultiplayerGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (this->PlayersSpawnPoints.Num() > 0)
	{
		return this->PlayersSpawnPoints.Pop(false);
	}
	return nullptr;
}
