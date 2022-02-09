#include "Systems_Workspace/Boids/BoidsGameMode.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Components/ColorComponent.h"
#include "Systems_Workspace/Boids/Components/ImpulseComponent.h"
#include "Systems_Workspace/Boids/Components/ObstacleComponent.h"
#include "Systems_Workspace/Boids/Components/RadiusComponent.h"
#include "Systems_Workspace/Boids/Components/VelocityComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSettings.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Systems_Workspace/Boids/Resources/GameEvents.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsAlignmentSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsApplyImpulseSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsCohesionSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsDebugDrawSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsEatSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsFaceDirectionSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsHungerSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsHuntSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsKeepInSpaceBoundsSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsLimitVelocitySystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsMovementSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsPopulationControlSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsProximityColorSystem.h"
#include "Systems_Workspace/Boids/Systems/Persistent/BoidsSeparationSystem.h"
#include "Systems_Workspace/Shared/Components/FoodComponent.h"
#include "Systems_Workspace/Shared/Components/HungerComponent.h"
#include "Systems_Workspace/Shared/Components/SpaceBoundsComponent.h"

const FName ABoidsGameMode::SYSTEMS_WORLD = FName(TEXT("Boids"));

void ABoidsGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(SYSTEMS_WORLD,
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
