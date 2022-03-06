#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Systems/Public/ActorArchetypeLocation.h"
#include "Systems/Public/ActorComponentsBucket.h"
#include "Systems/Public/ConsumedActorComponents.h"
#include "Systems/Public/Query.h"

#include "SystemsWorld.generated.h"

/// Set of options for installation of this system.
///
/// It uses builder pattern to simplify setting options.
///
/// Note that both [`struct: FInstallSystemOptions::RunBefore`]() and
/// [`struct:FInstallSystemOptions::RunAfter`]() methods are accumulative, which
/// means you can specify with them multiple systems required to run before
/// and/or after this one.
///
/// See [`class: USystemsWorld::InstallSystem`](), [`class:
/// USystemsWorld::InstallLambdaSystem`](),
/// [`class: USystemsWorld::InstallSystemRaw`]()
///
/// # Example
/// ```snippet
/// install_system_options
/// ```
USTRUCT(BlueprintType)
struct SYSTEMS_API FInstallSystemOptions
{
	GENERATED_BODY()

	friend class USystemsWorld;

public:
	FInstallSystemOptions();

	/// Create new options object.
	///
	/// # Example
	/// ```snippet
	/// install_system_options
	/// ```
	FInstallSystemOptions(
		/// Name of system being installed.
		FName Name);

	/// Add named dependency system that installed system should run before.
	///
	/// # Example
	/// ```snippet
	/// install_system_options
	/// ```
	FInstallSystemOptions& RunBefore(
		/// Name of other system.
		FName Name);

	/// Add named dependency system that installed system should run after.
	///
	/// # Example
	/// ```snippet
	/// install_system_options
	/// ```
	FInstallSystemOptions& RunAfter(
		/// Name of other system.
		FName Name);

private:
	UPROPERTY()
	FName Label = {};

	UPROPERTY()
	TSet<FName> Before = {};

	UPROPERTY()
	TSet<FName> After = {};
};

USTRUCT()
struct SYSTEMS_API FSystemData
{
	GENERATED_BODY()

	UPROPERTY()
	USystem* System = nullptr;

	UPROPERTY()
	FName Label = {};
};

/// Container that holds systems, resources and registry of components that
/// belongs to actors.
///
/// [Architecture book page](/book/architecture/world.md) explains more in depth
/// what systems world is, but in a brief: systems world is a central point user
/// interacts with using queries to resources and actor components. Think of it
/// as database - actor components are records and resources are unique
/// singleton-like data (usually config/settings or things you would call
/// managers).
UCLASS(BlueprintType)
class SYSTEMS_API USystemsWorld : public UObject
{
	GENERATED_BODY()

	friend struct FArchetype;
	template <class... T>
	friend struct TQuery;
	template <class... T>
	friend struct TTaggedQuery;
	friend class UDynamicQuery;
	friend struct FActorsIter;

public:
	using LambdaSystemType = void(ThisClass& Systems);

	/// Seal registry and initialize installed systems.
	///
	/// Method called by: [`class: USystemsSubsystem::AcquireSystemsWorld`]().
	///
	/// When user doesn't use [`class: USystemsSubsystem`]() as global systems
	/// world registry, or wants to handle systems world on their own, user
	/// should call this method after systems world setup (registering
	/// components, installation of systems and resources) and then call this
	/// method.
	///
	/// # Note
	/// > After calling this method, no further successful system or resource
	/// installation i possible, so make sure to install systems world
	/// components before sealing systems world!
	///
	/// # Example
	/// ```snippet
	/// systems_world_seal_and_initialize
	/// ```
	UFUNCTION(BlueprintCallable, Category = Systems)
	void SealAndInitialize();

	/// Register component type.
	///
	/// Prior to [`struct: FArchetypeSignature`]() being usable for queries and
	/// other architecture parts, it has to be able to identify components and
	/// for that they has to be registered by their class.
	///
	/// See [`class: UActorComponent`]()
	///
	/// # Return
	/// True if component was successfully installed (registry is not sealed and
	/// registry haven't reached its capacity).
	///
	/// # Example
	/// ```snippet
	/// systems_world_register_component_raw
	/// ```
	UFUNCTION(BlueprintCallable,
		Category = Systems,
		Meta = (DisplayName = "Register Component"))
	bool RegisterComponentRaw(
		/// Class of component that has to inherit from [`class:
		/// UActorComponent`]()
		const UClass* Type);

	/// Register component type.
	///
	/// Templated shortcut for [`class: USystemsWorld::RegisterComponentRaw`]()
	///
	/// See [`class: UActorComponent`]()
	///
	/// # Return
	/// True if component was successfully installed (registry is not sealed and
	/// registry haven't reached its capacity).
	///
	/// # Note
	/// > `T` has to be a component that inherits from [`class:
	/// UActorComponent`]()
	///
	/// # Example
	/// ```snippet
	/// systems_world_register_component
	/// ```
	template <class T>
	bool RegisterComponent()
	{
		//// [ignore]
		return RegisterComponentRaw(T::StaticClass());
		//// [/ignore]
	}

	/// Register resource object.
	///
	/// It accepts any object that inherits from `UObject`. Also systems world
	/// takes ownership over provided resource so its best to not pass any
	/// object that has its lifetime managed by other object.
	///
	/// # Return
	/// True if resource was successfully installed (registry is not sealed).
	///
	/// # Example
	/// ```snippet
	/// systems_world_install_resource_raw
	/// ```
	UFUNCTION(BlueprintCallable,
		Category = Systems,
		Meta = (DisplayName = "Install Resource"))
	bool InstallResourceRaw(
		/// Resource object to get registered and managed by this systems world.
		UObject* Resource);

	/// Register resource object.
	///
	/// See [`class: USystemsWorld::InstallResourceRaw`]()
	///
	/// # Return
	/// True if resource was successfully installed (registry is not sealed).
	///
	/// # Example
	/// ```snippet
	/// systems_world_install_default_resource
	/// ```
	UFUNCTION(BlueprintCallable, Category = Systems)
	bool InstallDefaultResource(
		/// Resource class to get constructed and registered.
		const UClass* Type);

	/// Register resource object.
	///
	/// Handy shortcut for [`class: USystemsWorld::InstallDefaultResource`]()
	///
	/// # Return
	/// True if resource was successfully installed (registry is not sealed).
	///
	/// # Example
	/// ```snippet
	/// systems_world_install_resource
	/// ```
	template <class T>
	bool InstallResource()
	{
		//// [ignore]
		return InstallDefaultResource(T::StaticClass());
		//// [/ignore]
	}

	/// Install system.
	///
	/// Usually user would want to install systems using either
	/// [`class:USystemsWorld::InstallSystem`]() or [`class:
	/// USystemsWorld::InstallLambdaSystem`]() but in case of valid reasons user
	/// can install system by its instance.
	///
	/// # Note
	/// > In case of [`struct: FInstallSystemOptions::Label`]() being empty, it
	/// will generate random label from new GUID.
	///
	/// See [`class: USystem`](), [`struct: FInstallSystemOptions`]()
	///
	/// # Return
	/// True if system was successfully installed (registry is not sealed).
	///
	/// # Example
	/// ```snippet
	/// systems_world_install_system_raw
	/// ```
	UFUNCTION(BlueprintCallable,
		Category = Systems,
		Meta = (DisplayName = "Install System"))
	bool InstallSystemRaw(
		/// Pointer to system being installed.
		USystem* System,
		/// System install options.
		FInstallSystemOptions Options);

	/// Install system.
	///
	/// See [`class: USystemsWorld::InstallSystemRaw`](), [`class: USystem`](),
	/// [`struct: FInstallSystemOptions`]()
	///
	/// # Return
	/// True if system was successfully installed (registry is not sealed).
	///
	/// # Example
	/// ```snippet
	/// systems_world_install_default_system
	/// ```
	UFUNCTION(BlueprintCallable, Category = Systems)
	bool InstallDefaultSystem(
		/// Class of system being installed.
		const UClass* Type,
		/// System install options.
		FInstallSystemOptions Options);

	/// Install system.
	///
	/// Handy shortcut for [`class: USystemsWorld::InstallDefaultSystem`]()
	///
	/// See [`class: USystem`](), [`struct: FInstallSystemOptions`]()
	///
	/// # Note
	/// > Make sure `T` is a class that inherits from [`class: USystem`]()!
	///
	/// # Return
	/// True if system was successfully installed (registry is not sealed).
	///
	/// # Example
	/// ```snippet
	/// systems_world_install_system
	/// ```
	template <class T>
	bool InstallSystem(
		/// System install options.
		FInstallSystemOptions Options)
	{
		//// [ignore]
		return InstallDefaultSystem(
			T::StaticClass(), MoveTempIfPossible(Options));
		//// [/ignore]
	}

	/// Install state-less (function or lambda) system.
	///
	/// Stateless systems are the most common ones because usually what system
	/// does it only processes the data, so creating function/lambda systems
	/// brings more ergonomics into codebase.
	///
	/// See [`struct: FInstallSystemOptions`]()
	///
	/// # Return
	/// True if system was successfully installed (registry is not sealed).
	///
	/// # Example
	/// ```snippet
	/// lambda_system
	/// ```
	///
	/// ```snippet
	/// install_lambda_system
	/// ```
	bool InstallLambdaSystem(
		/// Function or lambda being installed as system
		///
		/// # Note
		/// > `LambdaSystemType` should comply to given signature:
		/// `void(USystemsWorld&)`
		TFunction<LambdaSystemType>&& Functor,
		/// System install options.
		FInstallSystemOptions Options = FInstallSystemOptions());

	/// Tries to get pointer to resource by its class.
	///
	/// # Return
	/// Pointer to resource or `nullptr` in case resource does not exist in
	/// registry.
	///
	/// # Example
	/// ```snippet
	/// systems_world_resource_raw
	/// ```
	UFUNCTION(BlueprintCallable,
		Category = Systems,
		Meta = (DisplayName = "Get Resource"))
	UObject* ResourceRaw(
		/// Resource class.
		const UClass* Type);

	/// Tries to get pointer to resource by its type.
	///
	/// # Return
	/// Pointer to resource or `nullptr` in case resource does not exist in
	/// registry.
	///
	/// # Example
	/// ```snippet
	/// systems_world_resource
	/// ```
	template <typename T>
	T* Resource()
	{
		//// [ignore]
		return Cast<T>(ResourceRaw(T::StaticClass()));
		//// [/ignore]
	}

	/// Add actor component to registry.
	///
	/// Called in [`class: USystemsActorComponent::BeginPlay`]() and
	/// [`class:USystemsSceneComponent::BeginPlay`]() methods so user doesn't
	/// have to, but in case of user dynamically removing actor component to
	/// achieve support for behavior toggling, adding components back to
	/// registry can be achieved with this method.
	///
	/// # Note
	/// > Actor components are not registered immediatelly to avoid undefined
	/// behavior or even game crashes when performing this while iterating
	/// over systems world queries - rather they ar queued and registered
	/// after all systems complete their run on current game tick.
	///
	/// # Return
	/// True if both actor and component are valid.
	///
	/// # Example
	/// ```snippet
	/// systems_world_toggle_component
	/// ```
	UFUNCTION(BlueprintCallable, Category = Systems)
	bool AddComponent(
		/// Component to be registered.
		UActorComponent* Component);

	/// Remove actor component from registry.
	///
	/// Called in [`class: USystemsActorComponent::EndPlay`]() and
	/// [`class:USystemsSceneComponent::EndPlay`]() methods so user doesn't have
	/// to, but in case of user dynamically adding actor component to achieve
	/// support for behavior toggling, removing components from registry can be
	/// achieved with this method.
	///
	/// # Note
	/// > Actor components are not unregistered immediatelly to avoid undefined
	/// behavior or eve game crashes when performing this while iterating
	/// over systems world queries - rather the are queued and unregistered
	/// after all systems complete their run on current game tick.
	///
	/// # Return
	/// True if both actor and component are valid.
	///
	/// # Example
	/// ```snippet
	/// systems_world_toggle_component
	/// ```
	UFUNCTION(BlueprintCallable, Category = Systems)
	bool RemoveComponent(
		/// Component to be unregistered.
		UActorComponent* Component);

	/// Marks given resource as changed.
	///
	/// Useful if user wants to create reactive systems and/or UI that should
	/// only trigger when given resource changes. The reason why user has to
	/// manually mark resources as changed is for optimizations purposes, to
	/// mark deliberate changes in resources instead of markin them
	/// automatically, to avoid a lot of boilerplate of that automation, when
	/// most of the times systems and UI do not require to ask for changes.
	///
	/// Use [`class: USystemsWorld::ResourceDidChanged`]() to ask if some
	/// resource has changed.
	UFUNCTION(BlueprintCallable, Category = Systems)
	void MarkResourceChanged(UObject* Resource);

	/// Tells if given resource type did changed in last game tick.
	///
	/// See [`class: USystemsWorld::MarkResourceChanged`]().
	UFUNCTION(BlueprintCallable, Category = Systems)
	bool ResourceDidChangedRaw(const UClass* Type) const;

	/// Tells if given resource type did changed in last game tick.
	///
	/// Handy wrapper for [`class: USystemsWorld::ResourceDidChangedRaw`]().
	template <class T>
	bool ResourceDidChanged() const
	{
		//// [ignore]
		return ResourceDidChangedRaw(T::StaticClass());
		//// [/ignore]
	}

	/// Returns a set of unique type IDs of all resources that changed in last
	/// game tick.
	///
	/// Useful for more advanced usecases where user needs to ask for all
	/// changes anyway and compare them with some cached set of previously
	/// stored changes.
	const TSet<uint32>& LastChangedResources() const
	{
		//// [ignore]
		return this->CachedLastResourcesChanged;
		//// [/ignore]
	}

	/// Marks component as changed.
	///
	/// # Note
	/// > This will mark component type, not component instance, as changed. The
	/// need for component iinstance here is purely to ensure we do not mark
	/// components we do not have access to.
	UFUNCTION(BlueprintCallable, Category = Systems)
	void MarkComponentChanged(UActorComponent* Component);

	/// Returns signature of component types that changed during last game tick.
	///
	/// Useful for more usecases where user needs to cache and perform more
	/// advanced change detection between game ticks.
	UFUNCTION()
	const FArchetypeSignature& LastChangedComponents() const
	{
		//// [ignore]
		return this->CachedLastComponentsChanged;
		//// [/ignore]
	}

	/// Tells if components changed during last game tick.
	///
	/// # Note
	/// > This checks if any, not all of components marked in signature changed.
	UFUNCTION()
	bool ComponentsDidChangedRaw(const FArchetypeSignature& Signature) const;

	/// Tells if components changed during last game tick.
	///
	/// handy wrapper for [`class: USystemsWorld::ComponentsDidChangedRaw`]().
	template <class... T>
	bool ComponentsDidChanged() const
	{
		//// [ignore]
		auto Signature = FArchetypeSignature();
		const UClass* Types[] = {T::StaticClass()...};
		for (const auto* Type : Types)
		{
			if (const auto Index = ComponentTypeIndex(Type))
			{
				Signature.EnableBit(Index.GetValue());
			}
		}
		return ComponentsDidChangedRaw(Signature);
		//// [/ignore]
	}

	/// Tries to get pointer to registered actor component.
	///
	/// # Note
	/// > Because components are registered after systems run this will always
	/// return `nullptr` whe trying to get actor component just after calling
	/// [`class: USystemsWorld::AddComponent`]()
	///
	/// > Also when trying to get actor component just after calling
	/// [`class:USystemsWorld::RemoveComponent`]() will return given component
	/// instead o `nullptr` because components get unregistered after systems
	/// run.
	///
	/// # Return
	/// Pointer to component or `nullptr` in case component does not exist in
	/// registry.
	///
	/// # Example
	/// ```snippet
	/// systems_world_component_raw
	/// ```
	UFUNCTION(BlueprintCallable,
		Category = Systems,
		Meta = (DisplayName = "Get Component"))
	UActorComponent* ComponentRaw(
		/// Actor owning given component.
		AActor* Actor,
		/// Component class.
		const UClass* Type);

	/// Tries to get pointer to registered actor component.
	///
	/// Handy shortcut for [`class: USystemsWorld::ComponentRaw`]()
	///
	/// # Return
	/// Pointer to component or `nullptr` in case component does not exist in
	/// registry.
	///
	/// # Note
	/// > `T` should inherit from [`class: UActorComponent`]()
	///
	/// # Example
	/// ```snippet
	/// systems_world_component
	/// ```
	template <class T>
	T* Component(
		/// Actor owning given component.
		AActor* Actor)
	{
		//// [ignore]
		return Cast<T>(ComponentRaw(Actor, T::StaticClass()));
		//// [/ignore]
	}

	/// Gets tuple of actor components.
	///
	/// Handy wrapper for [`class: USystemsWorld::Component`]() in case of
	/// asking for more than one actor component.
	///
	/// # Note
	/// > Works similar way to [`class: USystemsWorld::Query`]() but do not put
	/// actor in its first tuple element, rather gives exactly the pointers to
	/// actor components user requests. It's worth noting that in case of
	/// component not being registered, it returns `nullptr` in tuple elements
	/// corresponding to requested actor component type.
	template <class... T>
	TTuple<T*...> Components(AActor* Actor)
	{
		//// [ignore]
		return MakeTuple(Cast<T>(ComponentRaw(Actor, T::StaticClass()))...);
		//// [/ignore]
	}

	/// Acquires lazy-iterator to query actor components.
	///
	/// More about iterators in [this architecture book
	/// page](/book/architecture/iterators.md).
	///
	/// Queries allow to yield tuples of actor and their components, and only
	/// those that comply to given query signature, so there is no iteration
	/// over any actor that do not have given component types - actors and
	/// components are registered to buckets called archetypes, and archetypes
	/// are unique as long as their signature is unique. Signature is
	/// constructed from types provided to query, as well as from types
	/// registered to systems world that belong to the same actor. Systems
	/// architecture focuses on performing queries as fast as possible and not
	/// iterating over actors that do not need to be queried was a priority.
	///
	/// See [`struct: TQuery`]()
	///
	/// # Note
	/// > Returned query iterator has always actor put as first item of item
	/// tuple and then follow requested components. So `Systems->Query<A, B,
	/// C>()` iterator will yield given tuple `TTuple<AActor*, A*, B*, C*>`
	///
	/// # Example
	/// ```snippet
	/// systems_world_query
	/// ```
	template <class... T>
	TQuery<T...> Query()
	{
		//// [ignore]
		return TQuery<T...>(this);
		//// [/ignore]
	}

	/// Acquires lazy-iterator to query actor components with additional tag
	/// components.
	///
	/// The difference between [`struct: TQuery`]() is that tagged queries
	/// allows to request existance of additional components on actor, ones that
	/// are not required for query to access - useful when user do not require
	/// any data of given components.
	///
	/// See [`struct: TTaggedQuery`]()
	///
	/// # Example
	/// ```snippet
	/// systems_world_tagged_query
	/// ```
	template <class... T>
	TTaggedQuery<T...> TaggedQuery()
	{
		//// [ignore]
		return TTaggedQuery<T...>(this);
		//// [/ignore]
	}

	/// Acquires lazy-iterator to dynamically queried actor components.
	///
	/// Handy shortcut for [`class: USystemsWorld::SpawnQuery`]().
	///
	/// See [`class: UDynamicQuery`]()
	template <class T>
	UDynamicQuery* DynamicQuery()
	{
		//// [ignore]
		return SpawnQuery(T::StaticClass());
		//// [/ignore]
	}

	/// Acquires lazy-iterator to dynamically queried actor components.
	///
	/// Because user cannot use templated types in blueprints, dynamic queries
	/// are a way to query systems world in blueprints. Also dynamic queries do
	/// not implement lazy-iterators so they are definitely not an ergonomic way
	/// to iterate over actor components and should be avoided in favor of
	/// [`class: USystems::Query`]().
	///
	/// See [`class: UDynamicQuery`]()
	UFUNCTION(BlueprintCallable,
		Category = Systems,
		Meta = (DisplayName = "Query"))
	UDynamicQuery* SpawnQuery(const UClass* BundleType);

	/// Acquires lazy-iterator over all registered actors.
	///
	/// This works similar to [`class: USystemsWorld::Query`]() but it yields
	/// only actors without their components, and **it yields all actors**. This
	/// method exists only for a last resort use cases - user should have a
	/// valid reason to query all actors and should always try to solve problem
	/// with [`class: USystemsWorld::Query`]().
	///
	/// The only use case i can think of is when user needs to for example count
	/// all registered actors, but there are other usecases which can definitely
	/// be solved with regular component queries.
	///
	/// Common usecase that would be wrong to query actors would be:
	/// ```snippet
	/// systems_world_bad_actors_query
	/// ```
	///
	/// User should instead for example add `UShiaComponent` actor component and
	/// query actors using
	/// [`class: USystemsWorld::Query`]() to iterate only on these actors that
	/// are marked as "Shia" actor using actor component tag:
	/// ```snippet
	/// systems_world_good_components_query
	/// ```
	///
	/// Another thing is that user should avoid putting any logic into the
	/// actors itself and rather create system that performs work of
	/// `AShiaActor::JustDoIt()` method. Although sometimes, mostly in case of
	/// interacting with third-party code, user is forced to call logic of actor
	/// so in this case just marking actor with component tag would be a
	/// sufficient compromise.
	FActorsIter Actors();

	/// Counts actors that contain given archetype include signature and do not
	/// contains exclude signature.
	///
	/// This is more performant way of counting actors with given set of
	/// components (although non-ergonomic for sure):
	/// ```snippet
	/// systems_world_count_raw
	/// ```
	///
	/// See [`class: USystemsWorld::Count`]() for more ergonomic use.
	///
	/// # Note
	/// > For example if requested signature is: `<A, B>` and there are actors:
	/// `1: A, B, C` and `2: A, C` then only actor `1: A, B, C` gets counted
	/// since only this one contains entirel requested signature.
	UFUNCTION()
	uint32 CountRaw(
		/// Archetype signature with minimal set of components that counted
		/// actors should contain.
		const FArchetypeSignature& IncludeSignature,
		/// Archetype signature with minimal set of components that counted
		/// actors should not contain.
		const FArchetypeSignature& ExcludeSignature) const;

	/// Counts actors that contain given archetype signature.
	///
	/// This is ergonomic shortcut for [`class: USystemsWorld::CountRaw`]() that
	/// only counts types that should be included.
	///
	/// # Note
	/// > `T` classes should inherit from [`class: UActorComponent`]()!
	///
	/// # Example
	/// ```snippet
	/// systems_world_count
	/// ```
	template <class... T>
	uint32 Count() const
	{
		//// [ignore]
		auto Signature = FArchetypeSignature();
		const UClass* Types[] = {T::StaticClass()...};
		for (const auto* Type : Types)
		{
			if (const auto Index = ComponentTypeIndex(Type))
			{
				Signature.EnableBit(Index.GetValue());
			}
		}
		return CountRaw(Signature);
		//// [/ignore]
	}

	/// Counts all registered actors.
	///
	/// Useful for debugging purposes to show number of registered actors, but
	/// any other use case would and most likely should be solved with regular
	/// queries.
	UFUNCTION()
	uint32 ActorsCount() const;

	/// Process systems world.
	///
	/// It performs:
	/// - unregistering of removed components and/or actors.
	/// - registering of added components and/or actors.
	/// - run systems logic.
	///
	/// In case of registry not being sealed at the time of calling this method,
	/// none of steps above are gonna be performed.
	///
	/// This method is called automatically for global systems world managed by
	/// [`class: USystemsSubsystem`](), but in case of user managing systems
	/// world on their own, user can do:
	/// ```snippet
	/// process_systems
	/// ```
	UFUNCTION(BlueprintCallable, Category = Systems)
	void Process();

	TOptional<FActorArchetypeLocation> FindActorArchetypeLocation(
		AActor* Actor) const;

	/// Get component registry index.
	///
	/// Useful when working directly with [`struct: FArchetypeSignature`](), but
	/// user most likely won't have any high-level usecase for that.
	///
	/// For getting component index by its class use: [`class:
	/// USystemsWorld::ComponentTypeIndex`]()
	TOptional<uint32> ComponentIndex(
		/// Component which index of we ask for.
		const UActorComponent* Component) const;

	TOptional<uint32> ComponentIdIndex(uint32 Id) const;

	/// Get component registry index.
	///
	/// Useful when working directly with [`struct: FArchetypeSignature`](), but
	/// user most likely won't have any high-level usecase for that.
	///
	/// For getting component index by component: [`class:
	/// USystemsWorld::ComponentIndex`]()
	TOptional<uint32> ComponentTypeIndex(const UClass* Type) const;

	/// Get archetype signature of given set of components.
	///
	/// Useful when working directly with [`struct: FArchetypeSignature`](), but
	/// user most likely won't have any high-level usecase for that.
	FArchetypeSignature ComponentsSignature(
		const TArrayView<UActorComponent*>& View) const;

	FArchetypeSignature ComponentsIdsSignature(
		const TArrayView<uint32>& View) const;

	TOptional<FArchetypeSignature> ActorSignature(AActor* Actor) const;

	const TArray<const UClass*>& RegisteredComponentClasses() const
	{
		//// [ignore]
		return this->ComponentClasses;
		//// [/ignore]
	}

private:
	TOptional<FConsumedActorComponents> ConsumeSwapActorComponents(
		AActor* Actor);

	TOptional<FConsumedActorComponents> ConsumeSwapActorIdComponents(uint32 Id);

	UFUNCTION()
	TArray<FArchetypeSignature> FindQueryArchetypes(
		const FArchetypeSignature& IncludeSignature,
		const FArchetypeSignature& ExcludeSignature) const;

	UFUNCTION()
	TArray<FArchetypeSignature> AllArchetypes() const;

	UFUNCTION()
	AActor* FindActorById(uint32 Id) const;

	UPROPERTY()
	bool bSealed = false;

	UPROPERTY()
	TMap<uint32, UObject*> Resources = {};

	UPROPERTY()
	uint32 ComponentTypes[SYSTEMS_COMPONENTS_COUNT] = {0};

	UPROPERTY()
	uint32 ComponentTypesCount = 0;

	// {class type uid: type index}
	UPROPERTY()
	TMap<uint32, uint32> ComponentTypesMap = {};

	UPROPERTY()
	TArray<const UClass*> ComponentClasses = {};

	UPROPERTY()
	TMap<FArchetypeSignature, FArchetype> Archetypes = {};

	UPROPERTY()
	TArray<FSystemData> Systems = {};

	UPROPERTY()
	TMap<uint32, FActorComponentsBucket> ActorComponentsToChange = {};

	UPROPERTY()
	FArchetypeSignature ComponentsBeingChanged = {};

	UPROPERTY()
	FArchetypeSignature CachedLastComponentsChanged = {};

	UPROPERTY()
	TSet<uint32> ResourcesBeingChanged = {};

	UPROPERTY()
	TSet<uint32> CachedLastResourcesChanged = {};

	UPROPERTY()
	TSet<FArchetypeSignature> CachedLastArchetypeKeys = {};

	UPROPERTY()
	bool ActorsChanged = false;
};
