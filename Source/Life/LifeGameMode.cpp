#include "Life/LifeGameMode.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/AgeComponent.h"
#include "Life/Components/AiComponent.h"
#include "Life/Components/AnimalComponent.h"
#include "Life/Components/CameraRelationComponent.h"
#include "Life/Components/ClearRangeComponent.h"
#include "Life/Components/ClearRangeEmitterComponent.h"
#include "Life/Components/ClearRangeReceiverComponent.h"
#include "Life/Components/CursorComponent.h"
#include "Life/Components/FaceCameraComponent.h"
#include "Life/Components/FlatMovementComponent.h"
#include "Life/Components/FlipbookBlendSpaceComponent.h"
#include "Life/Components/FlipbookProxyComponent.h"
#include "Life/Components/FollowPathComponent.h"
#include "Life/Components/GodComponent.h"
#include "Life/Components/HealthComponent.h"
#include "Life/Components/HumanComponent.h"
#include "Life/Components/SpriteProxyComponent.h"
#include "Life/Components/StaminaComponent.h"
#include "Life/Components/StrengthComponent.h"
#include "Life/Components/WaterComponent.h"
#include "Life/Components/WoodComponent.h"
#include "Life/Resources/LifeDebugSystems.h"
#include "Life/Resources/LifeSettings.h"
#include "Life/Resources/LifeSpawner.h"
#include "Life/Systems/Persistent/AgingSystem.h"
#include "Life/Systems/Persistent/AiSystem.h"
#include "Life/Systems/Persistent/ApplyAgeToSpriteSystem.h"
#include "Life/Systems/Persistent/ApplyBlendSpaceFlipbookSystem.h"
#include "Life/Systems/Persistent/ApplyCursorToWorldSystem.h"
#include "Life/Systems/Persistent/CameraRelationSystem.h"
#include "Life/Systems/Persistent/ClearVisionSystem.h"
#include "Life/Systems/Persistent/DebugFollowPathSystem.h"
#include "Life/Systems/Persistent/FaceCameraSystem.h"
#include "Life/Systems/Persistent/FlatMovementSystem.h"
#include "Life/Systems/Persistent/FollowPathSystem.h"
#include "Life/Systems/Persistent/HungerSystem.h"
#include "Life/Systems/Persistent/MoveCameraSystem.h"
#include "Life/Systems/Persistent/RotateCameraSystem.h"
#include "Life/Systems/Persistent/SpawnSystem.h"
#include "Life/Systems/Persistent/WaterSystem.h"
#include "Life/Systems/Persistent/ZoomCameraSystem.h"
#include "Shared/Components/FoodComponent.h"
#include "Shared/Components/HungerComponent.h"
#include "Shared/Components/SpaceBoundsComponent.h"
#include "Shared/Components/SpeedComponent.h"

const FName ALifeGameMode::SYSTEMS_WORLD = FName(TEXT("Life"));

void ALifeGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(ThisClass::SYSTEMS_WORLD,
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UGodComponent>();
				Systems.RegisterComponent<UFlatMovementComponent>();
				Systems.RegisterComponent<UAiComponent>();
				Systems.RegisterComponent<UFollowPathComponent>();
				Systems.RegisterComponent<USpeedComponent>();
				Systems.RegisterComponent<UHealthComponent>();
				Systems.RegisterComponent<UStrengthComponent>();
				Systems.RegisterComponent<UStaminaComponent>();
				Systems.RegisterComponent<UAgeComponent>();
				Systems.RegisterComponent<UHungerComponent>();
				Systems.RegisterComponent<UHumanComponent>();
				Systems.RegisterComponent<UAnimalComponent>();
				Systems.RegisterComponent<UWoodComponent>();
				Systems.RegisterComponent<UFoodComponent>();
				Systems.RegisterComponent<UFaceCameraComponent>();
				Systems.RegisterComponent<USpaceBoundsComponent>();
				Systems.RegisterComponent<UClearRangeComponent>();
				Systems.RegisterComponent<UClearRangeEmitterComponent>();
				Systems.RegisterComponent<UClearRangeReceiverComponent>();
				Systems.RegisterComponent<USpriteProxyComponent>();
				Systems.RegisterComponent<UFlipbookProxyComponent>();
				Systems.RegisterComponent<UFlipbookBlendSpaceComponent>();
				Systems.RegisterComponent<UCameraRelationComponent>();
				Systems.RegisterComponent<UCursorComponent>();
				Systems.RegisterComponent<UWaterComponent>();

				Systems.InstallResourceRaw(this->LifeSettings);
				Systems.InstallResourceRaw(this->LifeDebugSystems);
				Systems.InstallResource<UUiChangeDetection>();
				Systems.InstallResource<ULifeSpawner>();

				Systems.InstallLambdaSystem(AgingSystem, FInstallSystemOptions("Aging"));
				Systems.InstallLambdaSystem(HungerSystem, FInstallSystemOptions("Hunger"));
				Systems.InstallLambdaSystem(
					RotateCameraSystem, FInstallSystemOptions("RotateCamera"));
				Systems.InstallLambdaSystem(ZoomCameraSystem, FInstallSystemOptions("ZoomCamera"));
				Systems.InstallLambdaSystem(MoveCameraSystem, FInstallSystemOptions("MoveCamera"));
				Systems.InstallLambdaSystem(FollowPathSystem, FInstallSystemOptions("FollowPath"));
				Systems.InstallLambdaSystem(
					CameraRelationSystem, FInstallSystemOptions("CameraRelation"));
				Systems.InstallLambdaSystem(FaceCameraSystem, FInstallSystemOptions("FaceCamera"));
				Systems.InstallLambdaSystem(
					WaterSystem, FInstallSystemOptions("ApplyTimeScaleToWater"));
				Systems.InstallLambdaSystem(
					ApplyAgeToSpriteSystem, FInstallSystemOptions("ApplyAgeToSprite"));
				Systems.InstallLambdaSystem(ApplyBlendSpaceFlipbookSystem,
					FInstallSystemOptions("ApplyBlendSpaceFlipbook"));
				Systems.InstallLambdaSystem(
					ClearVisionSystem, FInstallSystemOptions("ClearVision"));
				Systems.InstallLambdaSystem(
					FlatMovementSystem, FInstallSystemOptions("FlatMovement"));
				Systems.InstallLambdaSystem(AiSystem, FInstallSystemOptions("Ai"));
				Systems.InstallLambdaSystem(SpawnSystem, FInstallSystemOptions("Spawn"));
				Systems.InstallLambdaSystem(
					ApplyCursorToWorldSystem, FInstallSystemOptions("ApplyCursorToWorld"));
				Systems.InstallLambdaSystem(
					UiChangeDetectionSystem, FInstallSystemOptions("UiChangeDetection"));

#if UE_BUILD_DEVELOPMENT
				Systems.InstallLambdaSystem(
					DebugFollowPathSystem, FInstallSystemOptions("DebugFollowPath"));
#endif
			});
	}
}

void ALifeGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
	}

	if (IsValid(this->LifeSettings) && IsValid(this->LifeSettings->CursorType))
	{
		GetWorld()->SpawnActor<AActor>(this->LifeSettings->CursorType, FVector(), FRotator());
	}
}

void ALifeGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->ReleaseSystemsWorld(ThisClass::SYSTEMS_WORLD);
	}
}
