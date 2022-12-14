#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "SystemsGameState.generated.h"

class USystemsPipeline;

/// Base class for game state that has to install [`class: USystemsPipeline`]().
///
/// # Note
/// > Given pipeline will be installed only on client.
UCLASS()
class SYSTEMS_API ASystemsGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void PostInitializeComponents() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/// Systems pipeline to install.
	UPROPERTY(EditAnywhere)
	TObjectPtr<USystemsPipeline> SystemsPipeline = {};
};
