#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"

#include "SystemsChangeDetection.generated.h"

class USystemsWorld;

DECLARE_MULTICAST_DELEGATE_OneParam(FSystemsWorldChangedDelegate,
	USystemsWorld&);

DECLARE_DYNAMIC_DELEGATE_OneParam(FSystemsWorldChangedEvent,
	USystemsWorld*,
	Systems);

USTRUCT(BlueprintType)
struct REACTIVESYSTEMS_API FSystemsChangeSignature
{
	GENERATED_BODY()

public:
	FSystemsChangeSignature();

	FSystemsChangeSignature(const FArchetypeSignature& Components,
		const TSet<uint32>& Resources);

	void ComponentRaw(const UClass* Type, const USystemsWorld* Systems);

	template <class T>
	void Component(const USystemsWorld* Systems)
	{
		ComponentRaw(T::StaticClass(), Systems);
	}

	void ResourceRaw(const UClass* Type);

	template <class T>
	void Resource()
	{
		ResourceRaw(T::StaticClass());
	}

	bool ContainsAny(const FSystemsChangeSignature& Other) const;

	bool operator==(const FSystemsChangeSignature& Other) const;

	bool Equals(const FSystemsChangeSignature& Other) const;

private:
	UPROPERTY()
	FArchetypeSignature Components = {};

	UPROPERTY()
	TSet<uint32> Resources = {};
};

uint32 REACTIVESYSTEMS_API GetTypeHash(
	const FSystemsChangeSignature& Signature);

UCLASS(BlueprintType)
class REACTIVESYSTEMS_API USystemsChangeDetection : public UObject
{
	GENERATED_BODY()

	friend void SystemsChangeDetectionSystem(USystemsWorld& Systems);

public:
	template <class T>
	void Subscribe(const FSystemsChangeSignature& Signature,
		T* Receiver,
		void (T::*Method)(USystemsWorld&),
		bool bForcedInitializationCall = true)
	{
		auto* Found = this->Events.Find(Signature);
		if (Found != nullptr)
		{
			Found->AddUObject(Receiver, Method);
		}
		else
		{
			auto Event = FSystemsWorldChangedDelegate();
			Event.AddUObject(Receiver, Method);
			this->Events.Add(Signature, Event);
		}
		if (bForcedInitializationCall)
		{
			this->InitializationEvents.AddUObject(Receiver, Method);
		}
	}

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Subscribe"))
	void SubscribeDynamic(const FSystemsChangeSignature& Signature,
		FSystemsWorldChangedEvent Callback,
		bool bForcedInitializationCall = true);

	template <class T>
	void Unsubscribe(const FSystemsChangeSignature& Signature, T* Receiver)
	{
		auto* Found = this->Events.Find(Signature);
		if (Found != nullptr)
		{
			Found->RemoveAll(Receiver);
		}
	}

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Unsubscribe"))
	void UnsubscribeDynamic(const FSystemsChangeSignature& Signature,
		UObject* Receiver)
	{
		Unsubscribe(Signature, Receiver);
	}

	template <class T>
	void UnsubscribeAll(T* Receiver)
	{
		for (auto& Pair : this->Events)
		{
			Pair.Value.RemoveAll(Receiver);
		}
	}

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Unsubscribe All"))
	void UnsubscribeAllDynamic(UObject* Receiver)
	{
		UnsubscribeAll(Receiver);
	}

private:
	void BroadcastChanges(const FSystemsChangeSignature& Signature,
		USystemsWorld& Systems);

	TMap<FSystemsChangeSignature, FSystemsWorldChangedDelegate> Events = {};

	FSystemsWorldChangedDelegate InitializationEvents = {};
};