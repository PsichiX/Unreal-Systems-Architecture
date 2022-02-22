#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "MultiplayerSpawnRocketComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class MULTIPLAYER_API UMultiplayerSpawnRocketComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UMultiplayerSpawnRocketComponent();

	UFUNCTION(Server, Reliable)
	void ServerExecute();

	UPROPERTY()
	float Cooldown = 0;
};
