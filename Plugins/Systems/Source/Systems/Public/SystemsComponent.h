#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Iterator/Std.h"

#include "SystemsComponent.generated.h"

class USystemsWorld;

/// Base class for systems actor components.
///
/// [Architecture book page](/book/architecture/components.md) explains more in
/// depth actor components relation with systems world but in a brief: they are
/// data stored in game world actors. You should inherit from this class any new
/// component class that should get automatically registered to and unregistered
/// from globally accessible systems worlds in [`class: USystemsSubsystem`]()
/// and then can be queried using [`class: USystemsWorld::Query`](). You can of
/// course not inherit components from this class and manually register or
/// unregister this component at will to any systems world if you want to make
/// it available from custom or just non-globally accessible systems world, this
/// class is useful only for automation of registering/unregistering components.
UCLASS(BlueprintType, Blueprintable, Abstract, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_API USystemsActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USystemsActorComponent();

	/// Tells if this component is registered to subsystem's systems world.
	bool IsRegistered() const
	{
		//// [ignore]
		return this->bRegister;
		//// [/ignore]
	}

	/// Registers this component to subsystem's systems worlds listed in
	/// [`class: $Self$::SystemsWorlds`]().
	void SetRegistered(
		/// If true, component gets registered, otherwise it gets unregistered.
		bool bMode,
		/// Forces registration change.
		///
		/// # Note
		/// > Be careful, when `bMode` is true, this will not unregister this
		/// component from systems worlds where this component is already
		/// registered - this is useful mainly when get called in
		/// `UActorComponent::BeginPlay` to enforce initial registration when
		/// internal `bRegister` gets set to true in editor.
		bool bForced = false);

	/// List of subsystem's systems worlds where this component should be
	/// registered.
	///
	/// If list is empty, it means this component will be added to every
	/// registered systems world.
	UPROPERTY(EditAnywhere, Category = "Systems|Component|Actor")
	TSet<FName> SystemsWorlds = {};

	/// Tells if given component can be automatically registered into systems
	/// worlds.
	///
	/// Useful to override when component has to meet certain conditions to get
	/// considered for automatic registration, conditions such as for example
	/// being owned by server game instance, or actor being a pawn.
	///
	/// By default this method always returns `true`.
	virtual bool CanBeRegisteredToSystemsWorld() const
	{
		//// [ignore]
		return true;
		//// [/ignore]
	}

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, Category = "Systems|Component|Actor")
	bool bRegister = true;
};

/// Base class for systems scene components.
///
///	This is scene component equivalent to [`class: USystemsActorComponent`]() -
/// scene components are useful where given component should have transformation
/// relative to actor.
UCLASS(BlueprintType, Blueprintable, Abstract, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_API USystemsSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USystemsSceneComponent();

	/// Tells if this component is registered to subsystem's systems world.
	bool IsRegistered() const
	{
		//// [ignore]
		return this->bRegister;
		//// [/ignore]
	}

	/// Registers this component to subsystem's systems worlds listed in
	/// [`class: $Self$::SystemsWorlds`]().
	void SetRegistered(
		/// If true, component gets registered, otherwise it gets unregistered.
		bool bMode,
		/// Forces registration change.
		///
		/// # Note
		/// > Be careful, when `bMode` is true, this will not unregister this
		/// component from systems worlds where this component is already
		/// registered - this is useful mainly when get called in
		/// `UActorComponent::BeginPlay` to enforce initial registration when
		/// internal `bRegister` gets set to true in editor.
		bool bForced = false);

	/// List of subsystem's systems worlds where this component should be
	/// registered.
	///
	/// If list is empty, it means this component will be added to every
	/// registered systems world.
	UPROPERTY(EditAnywhere, Category = "Systems|Component|Scene")
	TSet<FName> SystemsWorlds = {};

	/// Tells if given component can be automatically registered into systems
	/// worlds.
	///
	/// Useful to override when component has to meet certain conditions to get
	/// considered for automatic registration, conditions such as for example
	/// being owned by server game instance, or actor being a pawn.
	///
	/// By default this method always returns `true`.
	virtual bool CanBeRegisteredToSystemsWorld() const
	{
		//// [ignore]
		return true;
		//// [/ignore]
	}

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, Category = "Systems|Component|Scene")
	bool bRegister = true;
};

/// Component that groups actor components of certain type.
///
/// The point of this component is to allow Systems World to have a single point of
/// iteration on more than one component of given type, which is not possible to do
/// by default in Systems Architecture, because queries are single-typed.
///
/// This is useful mostly for cases where actor might have multiple components of
/// same type which we would like to iterate on.
///
/// # Note
/// > Since queries require concrete type for each component, it's best to create
/// classes per each components group type and make them inherit this class, so
/// you register that specialized group type into Systems World and use same
/// specialized group type in component queries.
UCLASS()
class SYSTEMS_API USystemsGroupComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	/// Gives iterator that yields grouped components that casts to `T`.
	template <typename T>
	auto Iter()
	{
		//// [ignore]
		return IterStd(this->CachedComponents)
			.Map<T*>([](auto* Component) { return Cast<T>(Component); })
			.Filter([](const auto* Component) { return IsValid(Component); });
		//// [/ignore]
	}

	/// Gives iterator that yields grouped components that has `Tag` tag and casts to `T`.
	template <typename T>
	auto TagIter(FName Tag)
	{
		//// [ignore]
		return Iter<T>().Filter([Tag](const auto* Component) { return Component->ComponentHasTag(Tag); });
		//// [/ignore]
	}

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UActorComponent> Type = {};

	UPROPERTY()
	TArray<UActorComponent*> CachedComponents = {};
};