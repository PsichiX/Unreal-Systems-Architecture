#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"

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
UCLASS(BlueprintType,
	Blueprintable,
	Abstract,
	Meta = (BlueprintSpawnableComponent))
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
	/// [`class: Self::SystemsWorlds`]().
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
/// scene componets are useful where given component should have transformation
/// relative to actor.
UCLASS(BlueprintType,
	Blueprintable,
	Abstract,
	Meta = (BlueprintSpawnableComponent))
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
	/// [`class: Self::SystemsWorlds`]().
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
