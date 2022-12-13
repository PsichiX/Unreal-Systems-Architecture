#include "Boids.h"

#include "Systems/Public/SystemsReflection.h"

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

#define LOCTEXT_NAMESPACE "FBoidsModule"
#define SYSTEMS_NAMESPACE "Boids"

void FBoidsModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(BoidsAlignmentSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsApplyImpulseSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsCohesionSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsDebugDrawSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsEatSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsFaceDirectionSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsHungerSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsHuntSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsKeepInSpaceBoundsSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsLimitVelocitySystem);
	REGISTER_SYSTEM_FUNCTION(BoidsPopulationControlSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsProximityColorSystem);
	REGISTER_SYSTEM_FUNCTION(BoidsSeparationSystem);
}

void FBoidsModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(BoidsAlignmentSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsApplyImpulseSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsCohesionSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsDebugDrawSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsEatSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsFaceDirectionSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsHungerSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsHuntSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsKeepInSpaceBoundsSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsLimitVelocitySystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsPopulationControlSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsProximityColorSystem);
	UNREGISTER_SYSTEM_FUNCTION(BoidsSeparationSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_GAME_MODULE(FBoidsModule, Boids);
