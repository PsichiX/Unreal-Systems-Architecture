#include "Systems/Public/SystemsComponent.h"

#include "Engine/World.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

const FName PROPERTY_NAME = FName(TEXT("SystemsRegisterComponentMode"));

USystemsActorComponent::USystemsActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USystemsActorComponent::SetRegistered(bool bMode, bool bForced)
{
	if (bMode && CanBeRegisteredToSystemsWorld() == false)
	{
		bMode = false;
	}

	if (bForced || (this->bRegister != bMode))
	{
		this->bRegister = bMode;
		if (this->bRegister)
		{
			if (this->SystemsWorlds.Num() > 0)
			{
				USystemsStatics::AddComponent(this, this->SystemsWorlds, GetWorld());
			}
			else
			{
				USystemsStatics::AddComponentEverywhere(this, GetWorld());
			}
		}
		else
		{
			USystemsStatics::RemoveComponent(this, GetWorld());
		}
	}
}

void USystemsActorComponent::BeginPlay()
{
	Super::BeginPlay();

	SetRegistered(this->bRegister, true);
}

void USystemsActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SetRegistered(false, true);
}

USystemsSceneComponent::USystemsSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USystemsSceneComponent::SetRegistered(bool bMode, bool bForced)
{
	if (bMode && CanBeRegisteredToSystemsWorld() == false)
	{
		bMode = false;
	}

	if (bForced || (this->bRegister != bMode))
	{
		this->bRegister = bMode;
		if (this->bRegister)
		{
			if (this->SystemsWorlds.Num() > 0)
			{
				USystemsStatics::AddComponent(this, this->SystemsWorlds, GetWorld());
			}
			else
			{
				USystemsStatics::AddComponentEverywhere(this, GetWorld());
			}
		}
		else
		{
			USystemsStatics::RemoveComponent(this, GetWorld());
		}
	}
}

void USystemsSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	SetRegistered(this->bRegister, true);
}

void USystemsSceneComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SetRegistered(false, true);
}

void USystemsGroupComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->GetOwner()))
	{
		this->CachedComponents.Reset();
		this->GetOwner()->GetComponents(this->Type, this->CachedComponents);
	}
}