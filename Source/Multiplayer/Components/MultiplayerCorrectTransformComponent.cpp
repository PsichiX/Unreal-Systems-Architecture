#include "Multiplayer/Components/MultiplayerCorrectTransformComponent.h"

UMultiplayerCorrectTransformComponent::UMultiplayerCorrectTransformComponent()
{
	SetNetAddressable();
	SetIsReplicatedByDefault(true);
}

void UMultiplayerCorrectTransformComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Metronome.Randomize();
}

void UMultiplayerCorrectTransformComponent::ClientUpdate_Implementation(FVector Position,
	FRotator Rotation)
{
	if (IsValid(GetOwner()))
	{
		GetOwner()->SetActorLocation(Position);
		GetOwner()->SetActorRotation(Rotation);
	}
}
