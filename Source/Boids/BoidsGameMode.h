#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "BoidsGameMode.generated.h"

class UBoidsSystemsRunCriteria;
class UBoidsSettings;

UCLASS()
class BOIDS_API ABoidsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	static const FName SYSTEMS_WORLD;

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	UBoidsSystemsRunCriteria* BoidsSystemsRunCriteria;

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	UBoidsSettings* BoidsSettings;

private:
	virtual void InitGame(const FString& MapName,
		const FString& Options,
		FString& ErrorMessage) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
