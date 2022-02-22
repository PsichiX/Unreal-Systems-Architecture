#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "MultiplayerMenuMode.generated.h"

UCLASS()
class MULTIPLAYER_API AMultiplayerMenuMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	static const FName SYSTEMS_WORLD;
	static const FName LEVEL_NAME;
};
