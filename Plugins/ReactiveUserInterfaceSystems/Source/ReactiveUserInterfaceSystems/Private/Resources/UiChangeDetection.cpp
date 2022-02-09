#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"

#include "Systems/Public/SystemsWorld.h"

FUiChangeSignature::FUiChangeSignature() : Components(), Resources()
{
}

FUiChangeSignature::FUiChangeSignature(const FArchetypeSignature& InComponents,
	const TSet<uint32>& InResources)
	: Components(InComponents), Resources(InResources)
{
}

void FUiChangeSignature::ComponentRaw(const UClass* Type,
	const USystemsWorld* Systems)
{
	if (IsValid(Type) && IsValid(Systems))
	{
		const auto Index = Systems->ComponentTypeIndex(Type);
		if (Index.IsSet())
		{
			this->Components.EnableBit(Index.GetValue());
		}
	}
}

void FUiChangeSignature::ResourceRaw(const UClass* Type)
{
	if (IsValid(Type))
	{
		this->Resources.Add(Type->GetUniqueID());
	}
}

bool FUiChangeSignature::ContainsAny(const FUiChangeSignature& Other) const
{
	if (this->Components.ContainsAny(Other.Components))
	{
		return true;
	}
	for (const auto Item : Other.Resources)
	{
		if (this->Resources.Contains(Item))
		{
			return true;
		}
	}
	return false;
}

bool FUiChangeSignature::operator==(const FUiChangeSignature& Other) const
{
	return Equals(Other);
}

bool FUiChangeSignature::Equals(const FUiChangeSignature& Other) const
{
	if (this->Components.Equals(Other.Components) == false ||
		this->Resources.Num() != Other.Resources.Num())
	{
		return false;
	}
	// NOTE: Sadly Unreal do not have any optimized comparison operator for TSet
	// so here we go with brute forced one for now.
	for (const auto Item : Other.Resources)
	{
		if (this->Resources.Contains(Item) == false)
		{
			return false;
		}
	}
	return true;
}

uint32 GetTypeHash(const FUiChangeSignature& Signature)
{
	return FCrc::MemCrc32(&Signature, sizeof(FUiChangeSignature));
}

void UUiChangeDetection::BroadcastChanges(const FUiChangeSignature& Signature,
	USystemsWorld& Systems)
{
	if (this->InitializationEvents.IsBound())
	{
		this->InitializationEvents.Broadcast(Systems);
	}
	this->InitializationEvents.Clear();

	for (auto& Pair : this->Events)
	{
		if (Pair.Key.ContainsAny(Signature) && Pair.Value.IsBound())
		{
			Pair.Value.Broadcast(Systems);
		}
	}
}
