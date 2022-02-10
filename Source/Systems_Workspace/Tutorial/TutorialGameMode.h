#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "TutorialGameMode.generated.h"

UCLASS()
class SYSTEMS_WORKSPACE_API ATutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	static const FName SYSTEMS_WORLD;

private:
	virtual void InitGame(const FString& MapName,
		const FString& Options,
		FString& ErrorMessage) override;

	virtual void BeginPlay() override;
};
