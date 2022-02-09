#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "LifeGameMode.generated.h"

class ULifeSettings;
class ULifeDebugSystems;

UCLASS()
class SYSTEMS_WORKSPACE_API ALifeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	static const FName SYSTEMS_WORLD;

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	ULifeSettings* LifeSettings;

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	ULifeDebugSystems* LifeDebugSystems;

private:
	virtual void InitGame(const FString& MapName,
		const FString& Options,
		FString& ErrorMessage) override;

	virtual void BeginPlay() override;
};
