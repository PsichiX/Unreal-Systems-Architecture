#include "Multiplayer/Components/MultiplayerHitComponent.h"

#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Constants.h"
#include "Multiplayer/Resources/MultiplayerCommands.h"

void UMultiplayerHitComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetNetMode() < NM_Client)
	{
		USystemsStatics::AddComponentEverywhere(this, GetWorld());
	}

	this->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnOverlapBegin);

	const auto* Systems = USystemsStatics::GetSystemsWorld(FName(), GetWorld());
	if (IsValid(Systems))
	{
		this->RequiredSignature.Clear();
		for (const auto& Type : this->RequiredComponents)
		{
			if (const auto Index = Systems->ComponentTypeIndex(Type.Get()))
			{
				this->RequiredSignature.EnableBit(Index.GetValue());
			}
		}
	}
}

void UMultiplayerHitComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USystemsStatics::RemoveComponent(this, GetWorld());

	this->OnComponentBeginOverlap.RemoveAll(this);
}

void UMultiplayerHitComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (GetWorld()->GetNetMode() == NM_Client || OtherActor == GetOwner() ||
		OtherActor == GetOwner()->GetInstigator())
	{
		return;
	}

	auto* Systems = USystemsStatics::GetSystemsWorld(FName(), GetWorld());
	if (IsValid(Systems) == false)
	{
		return;
	}

	auto* Commands = Systems->Resource<UMultiplayerCommands>();
	if (IsValid(Commands) == false)
	{
		return;
	}

	Commands->RequestedActorsToKill.Enqueue(GetOwner());

	if (const auto ActorSignature = Systems->ActorSignature(OtherActor))
	{
		if (ActorSignature.GetValue().ContainsAll(this->RequiredSignature))
		{
			Commands->RequestedActorsToKill.Enqueue(OtherActor);
		}
	}
}
