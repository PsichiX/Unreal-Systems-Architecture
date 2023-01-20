#include "Systems/Public/SystemsWorld.h"

#include "GameFramework/Actor.h"
#include "Systems/Public/ActorArchetypeLocation.h"
#include "Systems/Public/ConsumedActorComponents.h"
#include "Systems/Public/System.h"
#include "Systems/Public/SystemsComponent.h"

FInstallSystemOptions::FInstallSystemOptions() : Label(), Before(), After()
{
}

FInstallSystemOptions::FInstallSystemOptions(FName Name) : Label(Name), Before(), After()
{
}

FInstallSystemOptions& FInstallSystemOptions::RunBefore(FName Name)
{
	this->Before.Add(Name);
	return *this;
}

FInstallSystemOptions& FInstallSystemOptions::RunAfter(FName Name)
{
	this->After.Add(Name);
	return *this;
}

void USystemsWorld::SealAndInitialize()
{
	if (this->bSealed)
	{
		return;
	}

	this->bSealed = true;
	this->ComponentTypesMap.Reserve(this->ComponentTypesCount);

	for (uint32 Index = 0; Index < this->ComponentTypesCount; ++Index)
	{
		this->ComponentTypesMap.Add(this->ComponentTypes[Index], Index);
	}

	for (auto& Data : this->Systems)
	{
		Data.System->Init(*this);
	}
}

bool USystemsWorld::RegisterComponentRaw(const UClass* Type)
{
	if (this->bSealed || IsValid(Type) == false || this->ComponentTypesCount >= SYSTEMS_COMPONENTS_COUNT)
	{
		return false;
	}
	this->ComponentTypes[this->ComponentTypesCount] = Type->GetUniqueID();
	++this->ComponentTypesCount;
	this->ComponentClasses.Add(Type);
	return true;
}

bool USystemsWorld::InstallResourceRaw(UObject* Resource)
{
	if (this->bSealed || IsValid(Resource) == false)
	{
		return false;
	}
	const auto Id = Resource->GetClass()->GetUniqueID();
	this->Resources.Add(Id, Resource);
	this->ResourcesBeingChanged.Add(Id);
	return true;
}

bool USystemsWorld::InstallDefaultResource(const UClass* Type)
{
	return InstallResourceRaw(NewObject<UObject>(this, Type));
}

bool USystemsWorld::InstallProxyResourceRaw(const UClass* Type,
	UObject* Resource,
	TFunction<UObject*(UObject*)> Accessor)
{
	if (this->bSealed || IsValid(Resource) == false)
	{
		return false;
	}
	const auto Id = Type->GetUniqueID();
	this->ProxyResources.Add(Id, {Resource, Accessor});
	this->ResourcesBeingChanged.Add(Id);
	return true;
}

bool USystemsWorld::InstallSystemRaw(USystem* System, FInstallSystemOptions Options)
{
	if (this->bSealed || IsValid(System) == false ||
		this->Systems.ContainsByPredicate([&](const auto& Data) { return Data.System == System; }))
	{
		return false;
	}

	if (Options.Label.IsNone())
	{
		Options.Label = FName(FGuid::NewGuid().ToString());
	}

	const auto Before = IterStdConst(Options.Before)
							.FilterMap<int>(
								[&](const auto& Name)
								{
									const auto Index = this->Systems.IndexOfByPredicate(
										[&](const auto& Data) { return Data.Label == Name; });

									return Index >= 0 ? TOptional<int>(Index) : TOptional<int>();
								})
							.ComparedBy([](const auto A, const auto B) { return A < B; });

	const auto After = IterStdConst(Options.After)
						   .FilterMap<int>(
							   [&](const auto& Name)
							   {
								   const auto Index = this->Systems.IndexOfByPredicate(
									   [&](const auto& Data) { return Data.Label == Name; });

								   return Index >= 0 ? TOptional<int>(Index) : TOptional<int>();
							   })
						   .ComparedBy([](const auto A, const auto B) { return A > B; });

	if (Before.IsSet() && After.IsSet())
	{
		if (Before.GetValue() <= After.GetValue())
		{
			return false;
		}
		this->Systems.Insert({System, Options.Label}, After.GetValue() + 1);
	}
	if (Before.IsSet())
	{
		this->Systems.Insert({System, Options.Label}, Before.GetValue());
	}
	if (After.IsSet())
	{
		this->Systems.Insert({System, Options.Label}, After.GetValue() + 1);
	}
	else
	{
		this->Systems.Add({System, Options.Label});
	}
	return true;
}

bool USystemsWorld::InstallDefaultSystem(const UClass* Type, FInstallSystemOptions Options)
{
	return InstallSystemRaw(NewObject<USystem>(this, Type), MoveTempIfPossible(Options));
}

bool USystemsWorld::InstallLambdaSystem(TFunction<SystemsWorld::LambdaSystemType>&& Functor,
	FInstallSystemOptions Options)
{
	auto* System = NewObject<ULambdaSystem>();
	if (IsValid(System))
	{
		System->Bind(MoveTempIfPossible(Functor));
		return InstallSystemRaw(System, MoveTempIfPossible(Options));
	}
	return false;
}

UObject* USystemsWorld::ResourceRaw(const UClass* Type)
{
	if (this->bSealed == false || IsValid(Type) == false)
	{
		return nullptr;
	}
	const auto Id = Type->GetUniqueID();
	auto** Found = this->Resources.Find(Id);
	if (Found != nullptr)
	{
		return *Found;
	}
	return nullptr;
}

UObject* USystemsWorld::ProxyResourceRaw(const UClass* Type)
{
	if (this->bSealed == false || IsValid(Type) == false)
	{
		return nullptr;
	}
	const auto Id = Type->GetUniqueID();
	const auto* Found = this->ProxyResources.Find(Id);
	if (Found != nullptr && IsValid(Found->Resource))
	{
		return Found->Accessor(Found->Resource);
	}
	return nullptr;
}

bool USystemsWorld::AddComponent(UActorComponent* Component)
{
	if (IsValid(Component) == false || IsValid(Component->GetOwner()) == false)
	{
		return false;
	}
	auto* Actor = Component->GetOwner();
	if (IsValid(Actor) == false)
	{
		return false;
	}
	const auto ActorId = Actor->GetUniqueID();
	auto* Found = this->ActorComponentsToChange.Find(ActorId);
	if (Found != nullptr)
	{
		Found->Actor = Actor;
		Found->Add(Component);
	}
	else
	{
		auto Bucket = FActorComponentsBucket();
		Bucket.Actor = Actor;
		Bucket.Add(Component);
		this->ActorComponentsToChange.Add(ActorId, Bucket);
	}
	return true;
}

bool USystemsWorld::RemoveComponent(UActorComponent* Component)
{
	// NOTE: we use pointer comparison instead of IsValid because when component
	// or its actor are being destroyed, they are not valid (pending kill), but
	// memmory is still valid, at least the data what we need to identify given
	// component and its actor - this way components gets properly removed from
	// database.
	if (Component == nullptr || IsValid(Component->GetOwner()) == false)
	{
		return false;
	}
	auto* Actor = Component->GetOwner();
	if (Actor == nullptr)
	{
		return false;
	}
	const auto ActorId = Actor->GetUniqueID();
	auto* Found = this->ActorComponentsToChange.Find(ActorId);
	if (Found != nullptr)
	{
		if (IsValid(Actor))
		{
			Found->Actor = Actor;
		}
		Found->Remove(Component);
	}
	else
	{
		auto Bucket = FActorComponentsBucket();
		if (IsValid(Actor))
		{
			Bucket.Actor = Actor;
		}
		Bucket.Remove(Component);
		this->ActorComponentsToChange.Add(ActorId, Bucket);
	}
	return true;
}

void USystemsWorld::MarkResourceChanged(UObject* Resource)
{
	if (this->bSealed && IsValid(Resource))
	{
		const auto Id = Resource->GetClass()->GetUniqueID();
		if (this->Resources.Contains(Id))
		{
			this->ResourcesBeingChanged.Add(Id);
		}
	}
}

bool USystemsWorld::ResourceDidChangedRaw(const UClass* Type) const
{
	return IsValid(Type) && this->CachedLastResourcesChanged.Contains(Type->GetUniqueID());
}

void USystemsWorld::MarkComponentChanged(UActorComponent* Component)
{
	if (this->bSealed == false || IsValid(Component))
	{
		const auto Found = ComponentIndex(Component);
		if (Found.IsSet())
		{
			this->ComponentsBeingChanged.EnableBit(Found.GetValue());
		}
	}
}

bool USystemsWorld::ComponentsDidChangedRaw(const FArchetypeSignature& Signature) const
{
	return this->CachedLastComponentsChanged.ContainsAny(Signature);
}

UActorComponent* USystemsWorld::ComponentRaw(AActor* Actor, TSubclassOf<UActorComponent> Type)
{
	for (auto& Pair : this->Archetypes)
	{
		auto* Component = Pair.Value.ComponentRaw(Actor, Type);
		if (IsValid(Component))
		{
			return Component;
		}
	}
	return nullptr;
}

UDynamicQuery* USystemsWorld::SpawnQuery(const UClass* BundleType)
{
	auto* Result = NewObject<UDynamicQuery>(this, UDynamicQuery::StaticClass());
	Result->Setup(this, BundleType);
	return Result;
}

FActorsIter USystemsWorld::Actors()
{
	return FActorsIter(this);
}

uint32 USystemsWorld::CountRaw(const FArchetypeSignature& IncludeSignature,
	const FArchetypeSignature& ExcludeSignature) const
{
	auto Result = 0;
	for (const auto& Pair : this->Archetypes)
	{
		if (Pair.Key.ContainsAll(IncludeSignature) && Pair.Key.ContainsAny(ExcludeSignature) == false)
		{
			Result += Pair.Value.Actors.Num();
		}
	}
	return Result;
}

uint32 USystemsWorld::ActorsCount() const
{
	auto Result = 0;
	for (const auto& Pair : this->Archetypes)
	{
		Result += Pair.Value.Actors.Num();
	}
	return Result;
}

void USystemsWorld::Process()
{
	if (this->bSealed == false)
	{
		return;
	}

	this->CachedLastComponentsChanged = this->ComponentsBeingChanged;
	this->ComponentsBeingChanged.Clear();
	this->CachedLastResourcesChanged = this->ResourcesBeingChanged;
	this->ResourcesBeingChanged.Reset();

	// TODO: count actors to be added and reserve additional number of records
	// memory in archetype buckets before we actually add them.
	for (const auto& Pair : this->ActorComponentsToChange)
	{
		if (Pair.Value.IsEmpty())
		{
			continue;
		}
		const auto ActorId = Pair.Key;
		auto* Actor = Pair.Value.Actor;
		auto Added = Pair.Value.GetAdded();
		auto Removed = Pair.Value.GetRemoved();
		auto SignatureAdded = ComponentsSignature(Added);
		const auto SignatureRemoved = ComponentsIdsSignature(Removed);

		this->ComponentsBeingChanged = this->ComponentsBeingChanged.Include(SignatureAdded).Include(SignatureRemoved);

		if (const auto Consumed = ConsumeSwapActorIdComponents(ActorId))
		{
			SignatureAdded = Consumed.GetValue().Signature.Include(SignatureAdded).Exclude(SignatureRemoved);
			Added.Append(Consumed.GetValue().Components);
			Added.RemoveAllSwap([&](const auto* Component)
				{ return IsValid(Component) == false || Removed.Contains(Component->GetUniqueID()); },
				false);
		}

		if (Added.Num() <= 0 || IsValid(Actor) == false)
		{
			continue;
		}

		auto* ArchetypeFound = this->Archetypes.Find(SignatureAdded);
		if (ArchetypeFound == nullptr)
		{
			// TODO: preallocate with capacity.
			auto Archetype = FArchetype(SignatureAdded);
			Archetype.RebuildComponentTypesMap(*this);
			ArchetypeFound = &this->Archetypes.Add(SignatureAdded, Archetype);
		}
		const auto ComponentsOffset = ArchetypeFound->Actors.Num() * ArchetypeFound->Stride;
		ArchetypeFound->Actors.Add(Actor);
		ArchetypeFound->ActorsIds.Add(Actor->GetUniqueID());
		ArchetypeFound->Components.Reserve(ArchetypeFound->Actors.Num() * ArchetypeFound->Stride);
		for (uint32 Index = 0; Index < ArchetypeFound->Stride; ++Index)
		{
			ArchetypeFound->Components.Add(nullptr);
		}
		for (auto* Component : Added)
		{
			const auto Offset = ArchetypeFound->ComponentOffset(Component);
			if (Offset.IsSet())
			{
				ArchetypeFound->Components[ComponentsOffset + Offset.GetValue()] = Component;
			}
		}
	}

	this->ActorComponentsToChange.Reset();
	this->CachedLastArchetypeKeys.Reserve(this->Archetypes.Num());
	this->Archetypes.GetKeys(this->CachedLastArchetypeKeys);
	for (const auto& Key : this->CachedLastArchetypeKeys)
	{
		if (this->Archetypes[Key].Actors.Num() <= 0)
		{
			this->Archetypes.Remove(Key);
		}
	}

	for (auto& Data : this->Systems)
	{
		Data.System->Run(*this);
	}
}

TOptional<FActorArchetypeLocation> USystemsWorld::FindActorArchetypeLocation(AActor* Actor) const
{
	for (const auto& Pair : this->Archetypes)
	{
		const auto Index = Pair.Value.FindActorIndex(Actor);
		if (Index.IsSet())
		{
			return FActorArchetypeLocation{Pair.Key, Index.GetValue()};
		}
	}
	return TOptional<FActorArchetypeLocation>();
}

TOptional<uint32> USystemsWorld::ComponentIndex(const UActorComponent* Component) const
{
	return IsValid(Component) ? ComponentTypeIndex(Component->GetClass()) : TOptional<uint32>();
}

TOptional<uint32> USystemsWorld::ComponentIdIndex(uint32 Id) const
{
	if (this->bSealed)
	{
		const auto* Found = this->ComponentTypesMap.Find(Id);
		if (Found != nullptr)
		{
			return *Found;
		}
	}
	return TOptional<uint32>();
}

TOptional<uint32> USystemsWorld::ComponentTypeIndex(const UClass* Type) const
{
	return IsValid(Type) ? ComponentIdIndex(Type->GetUniqueID()) : TOptional<uint32>();
}

FArchetypeSignature USystemsWorld::ComponentsSignature(const TArrayView<UActorComponent*>& View) const
{
	auto Result = FArchetypeSignature();
	if (this->bSealed)
	{
		for (const auto* Component : View)
		{
			if (IsValid(Component))
			{
				const auto Found = ComponentIndex(Component);
				if (Found.IsSet())
				{
					Result.EnableBit(Found.GetValue());
				}
			}
		}
	}
	return Result;
}

FArchetypeSignature USystemsWorld::ComponentsIdsSignature(const TArrayView<uint32>& View) const
{
	auto Result = FArchetypeSignature();
	if (this->bSealed)
	{
		for (const auto UID : View)
		{
			const auto Found = ComponentIdIndex(UID);
			if (Found.IsSet())
			{
				Result.EnableBit(Found.GetValue());
			}
		}
	}
	return Result;
}

TOptional<FArchetypeSignature> USystemsWorld::ActorSignature(AActor* Actor) const
{
	for (const auto& Pair : this->Archetypes)
	{
		if (const auto Found = Pair.Value.FindActorIndex(Actor))
		{
			return TOptional<FArchetypeSignature>(Pair.Key);
		}
	}
	return TOptional<FArchetypeSignature>();
}

bool USystemsWorld::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	if (UObject::ProcessConsoleExec(Cmd, Ar, Executor))
	{
		return true;
	}
	for (const auto& ResourcePair : this->Resources)
	{
		if (IsValid(ResourcePair.Value) && ResourcePair.Value->ProcessConsoleExec(Cmd, Ar, Executor))
		{
			return true;
		}
	}
	for (const auto& System : this->Systems)
	{
		if (System.System->ProcessConsoleExec(Cmd, Ar, Executor))
		{
			return true;
		}
	}
	return false;
}

TOptional<FConsumedActorComponents> USystemsWorld::ConsumeSwapActorComponents(AActor* Actor)
{
	for (auto& Pair : this->Archetypes)
	{
		const auto Consumed = Pair.Value.ConsumeSwapActorComponents(Actor);
		if (Consumed.IsSet())
		{
			return FConsumedActorComponents{Pair.Key, Consumed.GetValue()};
		}
	}
	return TOptional<FConsumedActorComponents>();
}

TOptional<FConsumedActorComponents> USystemsWorld::ConsumeSwapActorIdComponents(uint32 Id)
{
	for (auto& Pair : this->Archetypes)
	{
		const auto Consumed = Pair.Value.ConsumeSwapActorIdComponents(Id);
		if (Consumed.IsSet())
		{
			return FConsumedActorComponents{Pair.Key, Consumed.GetValue()};
		}
	}
	return TOptional<FConsumedActorComponents>();
}

TArray<FArchetypeSignature> USystemsWorld::FindQueryArchetypes(const FArchetypeSignature& IncludeSignature,
	const FArchetypeSignature& ExcludeSignature) const
{
	auto Result = TArray<FArchetypeSignature>();
	Result.Reserve(this->Archetypes.Num());
	for (const auto& Pair : this->Archetypes)
	{
		if (Pair.Key.ContainsAll(IncludeSignature) && Pair.Key.ContainsAny(ExcludeSignature) == false)
		{
			Result.Add(Pair.Key);
		}
	}
	return Result;
}

TArray<FArchetypeSignature> USystemsWorld::AllArchetypes() const
{
	auto Result = TArray<FArchetypeSignature>();
	Result.Reserve(this->Archetypes.Num());
	for (const auto& Pair : this->Archetypes)
	{
		Result.Add(Pair.Key);
	}
	return Result;
}

AActor* USystemsWorld::FindActorById(uint32 Id) const
{
	for (const auto& Pair : this->Archetypes)
	{
		auto* Found = Pair.Value.FindActorById(Id);
		if (IsValid(Found))
		{
			return Found;
		}
	}
	return nullptr;
}