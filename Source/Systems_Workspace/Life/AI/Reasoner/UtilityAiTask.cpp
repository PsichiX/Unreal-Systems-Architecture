#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiTask.h"

#include "Systems/Public/SystemsWorld.h"

bool UUtilityAiTask::OnEnter(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory,
	bool bChanged)
{
	if (bChanged)
	{
		for (const auto& Type : this->RegisterComponents)
		{
			auto* Component = Actor->FindComponentByClass(Type);
			if (IsValid(Component))
			{
				Systems.AddComponent(Component);
			}
		}
	}

	return true;
}

void UUtilityAiTask::OnExit(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory)
{
	for (const auto& Type : this->RegisterComponents)
	{
		auto* Component = Actor->FindComponentByClass(Type);
		if (IsValid(Component))
		{
			Systems.RemoveComponent(Component);
		}
	}
}