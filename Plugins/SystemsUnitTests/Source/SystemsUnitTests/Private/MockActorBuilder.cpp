#include "SystemsUnitTests/Public/MockActorBuilder.h"

#include "Engine/World.h"

FMockActorBuilder::FMockActorBuilder() : Actor(nullptr)
{
}

FMockActorBuilder::FMockActorBuilder(UWorld* TheWorld,
	UClass* ActorType,
	FVector Position,
	FRotator Rotation)
	: Actor(nullptr)
{
	if (IsValid(TheWorld))
	{
		auto* NewActor =
			TheWorld->SpawnActor<AActor>(ActorType, Position, Rotation);
		if (IsValid(NewActor))
		{
			this->Actor = NewActor;
		}
	}
}

FMockActorBuilder& FMockActorBuilder::AddComponentRaw(
	const UClass* ComponentType)
{
	if (IsValid(this->Actor) && IsValid(ComponentType) &&
		ComponentType->IsChildOf<UActorComponent>())
	{
		auto* Component =
			NewObject<UActorComponent>(this->Actor, ComponentType);
		if (IsValid(Component))
		{
			Component->CreationMethod = EComponentCreationMethod::Native;
			auto* SceneComponent = Cast<USceneComponent>(Component);
			if (IsValid(SceneComponent))
			{
				SceneComponent->AttachToComponent(
					this->Actor->GetRootComponent(),
					FAttachmentTransformRules::KeepRelativeTransform);
			}
			Component->RegisterComponent();
		}
	}
	return *this;
}

AActor* FMockActorBuilder::GetActor()
{
	return this->Actor;
}
