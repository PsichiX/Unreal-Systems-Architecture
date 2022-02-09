#include "ReactiveUserInterfaceSystems/Public/Systems/UiChangeDetectionSystem.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsWorld.h"

void UiChangeDetectionSystem(USystemsWorld& Systems)
{
	auto* ChangeDetection = Systems.Resource<UUiChangeDetection>();
	if (IsValid(ChangeDetection))
	{
		const auto Signature = FUiChangeSignature(
			Systems.LastChangedComponents(), Systems.LastChangedResources());
		ChangeDetection->BroadcastChanges(Signature, Systems);
	}
}
