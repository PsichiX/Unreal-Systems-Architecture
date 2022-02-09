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
	UFUNCTION(BlueprintCallable,
		Category = "Systems|World",
		Meta = (WorldContext = "World", UnsafeDuringActorConstruction = "true"))
	static USystemsWorld* GetSystemsWorld(
		/// Systems world ID.
		FName Id,
		/// World context object.
		UWorld* World);

	/// Tries to add component to specified global systems worlds.
	///
	/// Useful when creating proxy components that inherits from engine
	/// components to make them auto added to global systems worlds on
	/// `BeginPlay`.
	UFUNCTION(BlueprintCallable,
		Category = "Systems|Components",
		Meta = (WorldContext = "World", UnsafeDuringActorConstruction = "true"))
	static void AddComponent(
		/// Actor component to add to global systems world.
		UActorComponent* Component,
		/// List of global systems worlds given component should be registered
		/// into.
		const TSet<FName>& SystemsWorlds,
		/// World context object.
		UWorld* World);

	/// Tries to add component to every global systems worlds.
	///
	/// Useful when creating proxy components that inherits from engine
	/// components to make them auto added to global systems worlds on
	/// `BeginPlay`.
	UFUNCTION(BlueprintCallable,
		Category = "Systems|Components",
		Meta = (WorldContext = "World", UnsafeDuringActorConstruction = "true"))
	static void AddComponentEverywhere(
		/// Actor component to add to global systems world.
		UActorComponent* Component,
		/// World context object.
		UWorld* World);

	/// Tries to add component to global systems worlds.
	///
	/// Useful when creating proxy components that inherits from engine
	/// components to make them auto removed from global systems worlds on
	/// `EndPlay`.
	UFUNCTION(BlueprintCallable,
		Category = "Systems|Components",
		Meta = (WorldContext = "World", UnsafeDuringActorConstruction = "true"))
	static void RemoveComponent(
		/// Actor component to remove from global systems world.
		UActorComponent* Component,
		/// World context object.
		UWorld* World);

	/// Gets resource from globally registered systems world by its label.
	///
	/// This is a handy shortcut for [`class: Self::GetSystemsWorld`]() and then
	/// [`class: USystemsWorld::GetResourceRaw`]() on it.
	UFUNCTION(BlueprintCallable,
		Category = "Systems|Resources",
		Meta = (WorldContext = "World",
			UnsafeDuringActorConstruction = "true",
			DisplayName = "Get Resource"))
	static UObject* GetResourceRaw(
		/// Systems world ID.
		FName Id,
		/// Resource type user wants to get from global systems world.
		const UClass* Type,
		/// World context object.
		UWorld* World);

	/// Gets resource from globally registered systems world by its label.
	///
	/// This is a handy template shortcut for [`class: Self::GetResourceRaw`]().
	template <class T>
	static T* GetResource(
		/// Systems world ID.
		FName Id,
		/// World context object.
		UWorld* World)
	{
		//// [ignore]
		return Cast<T>(ThisClass::GetResourceRaw(Id, T::StaticClass(), World));
		//// [/ignore]
	}
};
