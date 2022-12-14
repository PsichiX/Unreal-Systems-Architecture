#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "SystemsGameMode.generated.h"

class USystemsPipeline;

/// Base class for game mode that has to install [`class: USystemsPipeline`]().
///
/// # Note
/// > Given pipeline will be installed only on server.
UCLASS()
class SYSTEMS_API ASystemsGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/// Systems pipeline to install.
	UPROPERTY(EditAnywhere)
	TObjectPtr<USystemsPipeline> SystemsPipeline = {};
};
