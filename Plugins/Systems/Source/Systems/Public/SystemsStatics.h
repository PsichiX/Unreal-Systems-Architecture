#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "SystemsStatics.generated.h"

class USystemsWorld;

/// Library of useful boilerplate-wrapping functions related to systems
/// components.
UCLASS()
class SYSTEMS_API USystemsStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// Gets globally registered systems world by its label.
	///
	/// Useful in places where we need to perform systems qorld query, but there
	/// is no systems world provided into the scope.
	UFUNCTION(BlueprintPure,
		Category = "Systems|World",
		Meta = (WorldContext = "WorldContext", UnsafeDuringActorConstruction = "true"))
	static USystemsWorld* GetSystemsWorld(
		/// Systems world ID.
		FName Id,
		/// World context object.
		UObject* WorldContext);

	/// Gets actor component from globally registered systems world by its label.
	///
	/// This is a handy shortcut for [`class: Self::GetSystemsWorld`]() and then
	/// [`class: USystemsWorld::ComponentRaw`]() on it.
	UFUNCTION(BlueprintPure,
		Category = "Systems|Components",
		Meta = (WorldContext = "WorldContext",
			UnsafeDuringActorConstruction = "true",
			DisplayName = "Get Component",
			DeterminesOutputType = "Type"))
	static UActorComponent* GetComponentRaw(
		/// Systems world ID.
		FName Id,
		/// Actor to search in.
		AActor* Actor,
		/// Type of component to search for.
		const TSubclassOf<UActorComponent>& Type,
		/// World context object.
		UObject* WorldContext);

	/// Tries to add component to specified global systems worlds.
	///
	/// Useful when creating proxy components that inherits from engine
	/// components to make them auto added to global systems worlds on
	/// `BeginPlay`.
	UFUNCTION(BlueprintCallable,
		Category = "Systems|Components",
		Meta = (WorldContext = "WorldContext", UnsafeDuringActorConstruction = "true"))
	static void AddComponent(
		/// Actor component to add to global systems world.
		UActorComponent* Component,
		/// List of global systems worlds given component should be registered
		/// into.
		const TSet<FName>& SystemsWorlds,
		/// World context object.
		UObject* WorldContext);

	/// Tries to add component to every global systems worlds.
	///
	/// Useful when creating proxy components that inherits from engine
	/// components to make them auto added to global systems worlds on
	/// `BeginPlay`.
	UFUNCTION(BlueprintCallable,
		Category = "Systems|Components",
		Meta = (WorldContext = "WorldContext", UnsafeDuringActorConstruction = "true"))
	static void AddComponentEverywhere(
		/// Actor component to add to global systems world.
		UActorComponent* Component,
		/// World context object.
		UObject* WorldContext);

	/// Tries to add component to global systems worlds.
	///
	/// Useful when creating proxy components that inherits from engine
	/// components to make them auto removed from global systems worlds on
	/// `EndPlay`.
	UFUNCTION(BlueprintCallable,
		Category = "Systems|Components",
		Meta = (WorldContext = "WorldContext", UnsafeDuringActorConstruction = "true"))
	static void RemoveComponent(
		/// Actor component to remove from global systems world.
		UActorComponent* Component,
		/// World context object.
		UObject* WorldContext);

	/// Gets resource from globally registered systems world by its label.
	///
	/// This is a handy shortcut for [`class: Self::GetSystemsWorld`]() and then
	/// [`class: USystemsWorld::ResourceRaw`]() on it.
	UFUNCTION(BlueprintPure,
		Category = "Systems|Resources",
		Meta = (WorldContext = "WorldContext",
			UnsafeDuringActorConstruction = "true",
			DisplayName = "Get Resource",
			DeterminesOutputType = "Type"))
	static UObject* GetResourceRaw(
		/// Systems world ID.
		FName Id,
		/// Resource type user wants to get from global systems world.
		const UClass* Type,
		/// World context object.
		UObject* WorldContext);

	/// Gets resource from globally registered systems world by its label.
	///
	/// This is a handy template shortcut for [`class: Self::ResourceRaw`]().
	template <class T>
	static T* GetResource(
		/// Systems world ID.
		FName Id,
		/// World context object.
		UObject* WorldContext)
	{
		//// [ignore]
		if (IsValid(WorldContext) == false)
		{
			return nullptr;
		}
		return Cast<T>(ThisClass::GetResourceRaw(Id, T::StaticClass(), WorldContext->GetWorld()));
		//// [/ignore]
	}
};
