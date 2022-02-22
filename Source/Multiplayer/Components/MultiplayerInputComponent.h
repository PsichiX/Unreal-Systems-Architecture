#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "MultiplayerInputComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class MULTIPLAYER_API UMultiplayerInputComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UMultiplayerInputComponent();

	UFUNCTION(Server, Reliable)
	void ServerUpdate(FVector2D InRelativeMovement, FVector2D InRelativeLook);

	UPROPERTY(VisibleAnywhere)
	FVector2D RelativeMovement = FVector2D(0);

	UPROPERTY(VisibleAnywhere)
	FVector2D RelativeLook = FVector2D(0);
};
