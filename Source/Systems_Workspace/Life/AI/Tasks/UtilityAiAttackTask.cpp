#include "Systems_Workspace/Life/AI/Tasks/UtilityAiAttackTask.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiMemory.h"
#include "Systems_Workspace/Life/Components/HealthComponent.h"
#include "Systems_Workspace/Life/Components/StrengthComponent.h"
#include "Systems_Workspace/Shared/Components/FoodComponent.h"
#include "Systems_Workspace/Shared/Components/HungerComponent.h"

bool UUtilityAiAttackTask::OnEnter(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory,
	bool bChanged)
{
	Super::OnEnter(Actor, Systems, Memory, bChanged);

	auto* TargetActor = Memory.Access(this->TargetMemoryProperty).CastObject<AActor>();
	if (IsValid(TargetActor))
	{
		const auto* Strength = Systems.Component<UStrengthComponent>(Actor);
		auto* Hunger = Systems.Component<UHungerComponent>(Actor);
		auto* TargetHealth = Systems.Component<UHealthComponent>(TargetActor);
		const auto* TargetFood = Systems.Component<UFoodComponent>(TargetActor);

		if (IsValid(Strength) && IsValid(Hunger) && IsValid(TargetHealth) && IsValid(TargetFood))
		{
			TargetHealth->Value -= Strength->Value;
			if (TargetHealth->Value <= 0)
			{
				Hunger->Value -= TargetFood->Amount;
				TargetActor->Destroy();
			}
		}

		return true;
	}
	return false;
}
