#include "ReactiveSystems/Public/Systems/SystemsChangeDetectionSystem.h"

#include "ReactiveSystems/Public/Resources/SystemsChangeDetection.h"
#include "Systems/Public/SystemsWorld.h"

void SystemsChangeDetectionSystem(USystemsWorld& Systems)
{
	auto* ChangeDetection = Systems.Resource<USystemsChangeDetection>();
	if (IsValid(ChangeDetection))
	{
		const auto Signature = FSystemsChangeSignature(
			Systems.LastChangedComponents(), Systems.LastChangedResources());
		ChangeDetection->BroadcastChanges(Signature, Systems);
	}
}