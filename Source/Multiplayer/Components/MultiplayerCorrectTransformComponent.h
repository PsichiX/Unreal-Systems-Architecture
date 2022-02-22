#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Metronome.h"
#include "Systems/Public/SystemsComponent.h"

#include "MultiplayerCorrectTransformComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class MULTIPLAYER_API UMultiplayerCorrectTransformComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UMultiplayerCorrectTransformComponent();

	virtual void BeginPlay() override;

	UFUNCTION(Client, Unreliable)
	void ClientUpdate(FVector Position, FRotator Rotation);

	UPROPERTY(EditAnywhere)
	FMetronome Metronome = {};
};
