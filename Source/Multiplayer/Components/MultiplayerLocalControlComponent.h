#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "MultiplayerLocalControlComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class MULTIPLAYER_API UMultiplayerLocalControlComponent : public USystemsActorComponent
{
	GENERATED_BODY()

private:
	virtual bool CanBeRegisteredToSystemsWorld() const override;
};
