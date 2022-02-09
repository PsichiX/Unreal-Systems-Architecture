#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/ArchetypeSignature.h"

#include "UiChangeDetection.generated.h"

class USystemsWorld;

DECLARE_MULTICAST_DELEGATE_OneParam(FSystemsWorldChangedDelegate,
	USystemsWorld&);

USTRUCT()
struct REACTIVEUSERINTERFACESYSTEMS_API FUiChangeSignature
{
	GENERATED_BODY()

public:
	FUiChangeSignature();

	FUiChangeSignature(const FArchetypeSignature& Components,
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

	bool ContainsAny(const FUiChangeSignature& Other) const;

	bool operator==(const FUiChangeSignature& Other) const;

	bool Equals(const FUiChangeSignature& Other) const;

private:
	UPROPERTY()
	FArchetypeSignature Components = {};

	UPROPERTY()
	TSet<uint32> Resources = {};
};

uint32 REACTIVEUSERINTERFACESYSTEMS_API GetTypeHash(
	const FUiChangeSignature& Signature);

UCLASS(BlueprintType)
class REACTIVEUSERINTERFACESYSTEMS_API UUiChangeDetection : public UObject
{
	GENERATED_BODY()

	friend void UiChangeDetectionSystem(USystemsWorld& Systems);

public:
	template <class T>
	void Subscribe(const FUiChangeSignature& Signature,
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

	template <class T>
	void Unsubscribe(const FUiChangeSignature& Signature, T* Receiver)
	{
		auto* Found = this->Events.Find(Signature);
		if (Found != nullptr)
		{
			Found->RemoveAll(Receiver);
		}
	}

	template <class T>
	void UnsubscribeAll(T* Receiver)
	{
		for (auto& Pair : this->Events)
		{
			Pair.Value.RemoveAll(Receiver);
		}
	}

private:
	void BroadcastChanges(const FUiChangeSignature& Signature,
		USystemsWorld& Systems);

	TMap<FUiChangeSignature, FSystemsWorldChangedDelegate> Events = {};

	FSystemsWorldChangedDelegate InitializationEvents = {};
};
