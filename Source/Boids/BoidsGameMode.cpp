#include "Boids/BoidsGameMode.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Shared/Components/FoodComponent.h"
#include "Shared/Components/HungerComponent.h"
#include "Shared/Components/SpaceBoundsComponent.h"
#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Components/ColorComponent.h"
#include "Boids/Components/ImpulseComponent.h"
#include "Boids/Components/ObstacleComponent.h"
#include "Boids/Components/RadiusComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Boids/Resources/GameEvents.h"
#include "Boids/Systems/Persistent/BoidsAlignmentSystem.h"
#include "Boids/Systems/Persistent/BoidsApplyImpulseSystem.h"
#include "Boids/Systems/Persistent/BoidsCohesionSystem.h"
#include "Boids/Systems/Persistent/BoidsDebugDrawSystem.h"
#include "Boids/Systems/Persistent/BoidsEatSystem.h"
#include "Boids/Systems/Persistent/BoidsFaceDirectionSystem.h"
#include "Boids/Systems/Persistent/BoidsHungerSystem.h"
#include "Boids/Systems/Persistent/BoidsHuntSystem.h"
#include "Boids/Systems/Persistent/BoidsKeepInSpaceBoundsSystem.h"
#include "Boids/Systems/Persistent/BoidsLimitVelocitySystem.h"
#include "Boids/Systems/Persistent/BoidsMovementSystem.h"
#include "Boids/Systems/Persistent/BoidsPopulationControlSystem.h"
#include "Boids/Systems/Persistent/BoidsProximityColorSystem.h"
#include "Boids/Systems/Persistent/BoidsSeparationSystem.h"

const FName ABoidsGameMode::SYSTEMS_WORLD = FName(TEXT("Boids"));

void ABoidsGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(ThisClass::SYSTEMS_WORLD,
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UBoidComponent>();
				Systems.RegisterComponent<UObstacleComponent>();
				Systems.RegisterComponent<UImpulseComponent>();
				Systems.RegisterComponent<UVelocityComponent>();
				Systems.RegisterComponent<UColorComponent>();
				Systems.RegisterComponent<USpaceBoundsComponent>();
				Systems.RegisterComponent<UFoodComponent>();
				Systems.RegisterComponent<UHungerComponent>();
				Systems.RegisterComponent<URadiusComponent>();

				Systems.InstallResourceRaw(this->BoidsSystemsRunCriteria);
				Systems.InstallResourceRaw(this->BoidsSettings);
				Systems.InstallResource<UUiChangeDetection>();
				Systems.InstallResource<UGameEvents>();

				Systems.InstallLambdaSystem(
					BoidsPopulationControlSystem, FInstallSystemOptions("BoidsPopulationControl"));
				Systems.InstallLambdaSystem(
					BoidsHungerSystem, FInstallSystemOptions("BoidsHunger"));
				Systems.InstallLambdaSystem(
					BoidsAlignmentSystem, FInstallSystemOptions("BoidsAlignment"));
				Systems.InstallLambdaSystem(
					BoidsCohesionSystem, FInstallSystemOptions("BoidsCohesion"));
				Systems.InstallLambdaSystem(
					BoidsSeparationSystem, FInstallSystemOptions("BoidsSeparation"));
				Systems.InstallLambdaSystem(BoidsHuntSystem, FInstallSystemOptions("BoidsHunt"));
				Systems.InstallLambdaSystem(
					BoidsApplyImpulseSystem, FInstallSystemOptions("BoidsApplyImpulse"));
				Systems.InstallLambdaSystem(
					BoidsLimitVelocitySystem, FInstallSystemOptions("BoidsLimitVelocity"));
				Systems.InstallSystem<UBoidsMovementSystem>(FInstallSystemOptions("BoidsMovement"));
				Systems.InstallLambdaSystem(
					BoidsKeepInSpaceBoundsSystem, FInstallSystemOptions("BoidsKeepInSpaceBounds"));
				Systems.InstallLambdaSystem(
					BoidsFaceDirectionSystem, FInstallSystemOptions("BoidsFaceDirection"));
				Systems.InstallLambdaSystem(BoidsEatSystem, FInstallSystemOptions("BoidsEat"));
				Systems.InstallLambdaSystem(
					UiChangeDetectionSystem, FInstallSystemOptions("UiChangeDetection"));

#if UE_BUILD_DEVELOPMENT
				Systems.InstallLambdaSystem(
					BoidsProximityColorSystem, FInstallSystemOptions("BoidsProximityColor"));
				Systems.InstallLambdaSystem(
					BoidsDebugDrawSystem, FInstallSystemOptions("BoidsDebugDraw"));
#endif
			});
	}
}

void ABoidsGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->ReleaseSystemsWorld(ThisClass::SYSTEMS_WORLD);
	}
}
