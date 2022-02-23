#include "Multiplayer/Systems/Persistent/MultiplayerApplyInputSystem.h"

#include "EngineUtils.h"
#include "Shared/Components/SpeedComponent.h"
#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Components/MultiPlayerComponent.h"
#include "Multiplayer/Components/MultiplayerInputComponent.h"
#include "Multiplayer/Components/MultiplayerLocalControlComponent.h"
#include "Multiplayer/Resources/MultiPlayerInput.h"

void MultiplayerApplyInputSystemInner(USystemsWorld& Systems, const UMultiPlayerInput& PlayerInput)
{
	Systems
		.Query<UMultiplayerInputComponent,
			UMultiPlayerComponent,
			UMultiplayerLocalControlComponent>()
		.ForEach(
			[&](auto& QueryItem)
			{
				auto* Input = QueryItem.Get<1>();

				Input->RelativeMovement = PlayerInput.RelativeMovement;
				Input->RelativeLook = PlayerInput.RelativeLook;
			});

	Systems
		.Query<UMultiplayerInputComponent,
			UVelocityComponent,
			USpeedComponent,
			UMultiPlayerComponent>()
		.ForEach(
			[](auto& QueryItem)
			{
				auto* Actor = QueryItem.Get<0>();
				const auto* Input = QueryItem.Get<1>();
				auto* Velocity = QueryItem.Get<2>();
				const auto Speed = QueryItem.Get<3>()->Value;
				auto Rotation = Actor->GetActorRotation();

				Rotation.Yaw += Input->RelativeLook.X;
				Rotation.Pitch -= Input->RelativeLook.Y;
				Rotation.Pitch = FMath::Clamp(Rotation.Pitch, -45.0f, 45.0f);
				Actor->SetActorRotation(Rotation);

				Rotation.Pitch = 0;
				Rotation.Roll = 0;
				const auto Delta =
					FVector(-Input->RelativeMovement.Y, Input->RelativeMovement.X, 0);
				Velocity->Value = Rotation.RotateVector(Delta) * Speed;
			});
}

void MultiplayerServerApplyInputSystem(USystemsWorld& Systems)
{
	auto* PlayerInput = Systems.Resource<UMultiPlayerInput>();
	if (IsValid(PlayerInput) == false)
	{
		return;
	}

	MultiplayerApplyInputSystemInner(Systems, *PlayerInput);
}

void MultiplayerClientApplyInputSystem(USystemsWorld& Systems)
{
	auto* PlayerInput = Systems.Resource<UMultiPlayerInput>();
	if (IsValid(PlayerInput) == false)
	{
		return;
	}

	Systems
		.Query<UMultiplayerInputComponent,
			UMultiPlayerComponent,
			UMultiplayerLocalControlComponent>()
		.ForEach(
			[&](auto& QueryItem)
			{
				const auto* Actor = QueryItem.Get<0>();
				auto* Input = QueryItem.Get<1>();

				Input->ServerUpdate(PlayerInput->RelativeMovement, PlayerInput->RelativeLook);
			});

	MultiplayerApplyInputSystemInner(Systems, *PlayerInput);
}
