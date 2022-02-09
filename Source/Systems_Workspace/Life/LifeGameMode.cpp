#include "Systems_Workspace/Life/LifeGameMode.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/AgeComponent.h"
#include "Systems_Workspace/Life/Components/AiComponent.h"
#include "Systems_Workspace/Life/Components/AnimalComponent.h"
#include "Systems_Workspace/Life/Components/CameraRelationComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeEmitterComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeReceiverComponent.h"
#include "Systems_Workspace/Life/Components/CursorComponent.h"
#include "Systems_Workspace/Life/Components/FaceCameraComponent.h"
#include "Systems_Workspace/Life/Components/FlatMovementComponent.h"
#include "Systems_Workspace/Life/Components/FlipbookBlendSpaceComponent.h"
#include "Systems_Workspace/Life/Components/FlipbookProxyComponent.h"
#include "Systems_Workspace/Life/Components/FollowPathComponent.h"
#include "Systems_Workspace/Life/Components/GodComponent.h"
#include "Systems_Workspace/Life/Components/HealthComponent.h"
#include "Systems_Workspace/Life/Components/HumanComponent.h"
#include "Systems_Workspace/Life/Components/SpeedComponent.h"
#include "Systems_Workspace/Life/Components/SpriteProxyComponent.h"
#include "Systems_Workspace/Life/Components/StaminaComponent.h"
#include "Systems_Workspace/Life/Components/StrengthComponent.h"
#include "Systems_Workspace/Life/Components/WaterComponent.h"
#include "Systems_Workspace/Life/Components/WoodComponent.h"
#include "Systems_Workspace/Life/Resources/LifeDebugSystems.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"
#include "Systems_Workspace/Life/Resources/LifeSpawner.h"
#include "Systems_Workspace/Life/Systems/Persistent/AgingSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/AiSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/ApplyAgeToSpriteSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/ApplyBlendSpaceFlipbookSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/ApplyCursorToWorldSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/CameraRelationSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/ClearVisionSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/DebugFollowPathSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/FaceCameraSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/FlatMovementSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/FollowPathSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/HungerSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/MoveCameraSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/RotateCameraSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/SpawnSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/WaterSystem.h"
#include "Systems_Workspace/Life/Systems/Persistent/ZoomCameraSystem.h"
#include "Systems_Workspace/Shared/Components/FoodComponent.h"
#include "Systems_Workspace/Shared/Components/HungerComponent.h"
#include "Systems_Workspace/Shared/Components/SpaceBoundsComponent.h"

const FName ALifeGameMode::SYSTEMS_WORLD = FName(TEXT("Life"));

void ALifeGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(SYSTEMS_WORLD,
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
