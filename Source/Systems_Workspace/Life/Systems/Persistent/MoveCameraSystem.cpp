#include "Systems_Workspace/Life/Systems/Persistent/MoveCameraSystem.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/GodComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"
#include "Systems_Workspace/Shared/Components/SpaceBoundsComponent.h"

const float TRACE_RANGE = 10000.0f;

void MoveCameraSystem(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto BoxOpt = Systems.Query<USpaceBoundsComponent>()
							.FilterMap<FBoxSphereBounds>(
								[](const auto& QueryItem)
								{
									const auto* SpaceBounds = QueryItem.Get<1>();
									return SpaceBounds->Bounds;
								})
							.Map<FBox>([](const auto& Bounds) { return Bounds.GetBox(); })
							.First();

	if (BoxOpt.IsSet() == false)
	{
		return;
	}

	const auto Box = BoxOpt.GetValue();
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto Speed = Settings->CameraMovementSpeed * DeltaTime;

	if (auto QueryItem = Systems.Query<UGodComponent>().First())
	{
		auto* Actor = QueryItem.GetValue().Get<0>();
		const auto* God = QueryItem.GetValue().Get<1>();

		auto Delta = FVector(0);
		Delta.X = God->CameraRelativeMovement.X * Speed;
		Delta.Y = God->CameraRelativeMovement.Y * Speed;
		Delta = Actor->GetActorRotation().RotateVector(Delta);

		auto Position = Actor->GetActorLocation();
		Position.X = FMath::Clamp(Position.X + Delta.X, Box.Min.X, Box.Max.X);
		Position.Y = FMath::Clamp(Position.Y + Delta.Y, Box.Min.Y, Box.Max.Y);
		Position.Z = 0;

		FHitResult Hit;
		if (Systems.GetWorld()->LineTraceSingleByChannel(Hit,
				Position + FVector(0, 0, TRACE_RANGE),
				Position - FVector(0, 0, TRACE_RANGE),
				ECollisionChannel::ECC_GameTraceChannel1))
		{
			Position.Z = Hit.ImpactPoint.Z;
		}
		else
		{
			Position.Z = 0;
		}
		Actor->SetActorLocation(Position);
	}
}
