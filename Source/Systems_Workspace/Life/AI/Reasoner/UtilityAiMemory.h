#pragma once

#include "CoreMinimal.h"

#include "Misc/TVariant.h"

#include "UtilityAiMemory.generated.h"

struct FUtilityAiMemoryValue;
struct FUtilityAiMemoryKeyValue;

using FUtilityAiMemoryValueInner = TVariant<FEmptyVariantState,
	bool,
	uint32,
	int32,
	float,
	FVector,
	FRotator,
	FName,
	FString,
	UObject*,
	TArray<FUtilityAiMemoryValue>,
	TArray<FUtilityAiMemoryKeyValue>>;

USTRUCT()
struct SYSTEMS_WORKSPACE_API FUtilityAiMemoryValue
{
	GENERATED_BODY()

public:
	FUtilityAiMemoryValue();

	FUtilityAiMemoryValue(bool bValue);

	FUtilityAiMemoryValue(uint32 Value);

	FUtilityAiMemoryValue(int32 Value);

	FUtilityAiMemoryValue(float Value);

	FUtilityAiMemoryValue(const FVector& Value);

	FUtilityAiMemoryValue(const FRotator& Value);

	FUtilityAiMemoryValue(const FName& Value);

	FUtilityAiMemoryValue(const FString& Value);

	FUtilityAiMemoryValue(UObject* Value);

	FUtilityAiMemoryValue(const TArray<FUtilityAiMemoryValue>& Value);

	FUtilityAiMemoryValue(const TArray<FUtilityAiMemoryKeyValue>& Value);

	bool* AsBool();

	uint32* AsUInt32();

	int32* AsInt32();

	float* AsFloat();

	FVector* AsVector();

	FRotator* AsRotator();

	FName* AsName();

	FString* AsString();

	UObject* AsObject();

	template <class T>
	T* CastObject()
	{
		return Cast<T>(AsObject());
	}

	TArray<FUtilityAiMemoryValue>* AsArray();

	TArray<FUtilityAiMemoryKeyValue>* AsDictionary();

	void Clear();

private:
	FUtilityAiMemoryValueInner Inner = {};
};

USTRUCT()
struct SYSTEMS_WORKSPACE_API FUtilityAiMemoryKeyValue
{
	GENERATED_BODY()

public:
	FUtilityAiMemoryKeyValue();

	FUtilityAiMemoryKeyValue(const FName& InKey, const FUtilityAiMemoryValue& InValue);

	UPROPERTY()
	FName Key = FName();

	UPROPERTY()
	FUtilityAiMemoryValue Value = {};
};

USTRUCT()
struct SYSTEMS_WORKSPACE_API FUtilityAiMemory
{
	GENERATED_BODY()

public:
	FUtilityAiMemoryValue& Access(const FName& Name)
	{
		return this->Values.FindOrAdd(Name);
	}

private:
	TMap<FName, FUtilityAiMemoryValue> Values = {};
};
