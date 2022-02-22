#pragma once

#include "CoreMinimal.h"

#include "MultiplayerCommands.generated.h"

UCLASS(BlueprintType)
class MULTIPLAYER_API UMultiplayerCommands : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UClass* RocketClass = nullptr;

	UPROPERTY(EditAnywhere)
	float RocketInitialSpeed = 2000;

	UPROPERTY(EditAnywhere)
	float RocketCooldown = 1;

	TQueue<AActor*> RocketsToSpawnForActors = {};

	TQueue<AActor*> RequestedActorsToKill = {};
};
