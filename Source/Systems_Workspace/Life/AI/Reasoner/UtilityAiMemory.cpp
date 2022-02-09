#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiMemory.h"

FUtilityAiMemoryKeyValue::FUtilityAiMemoryKeyValue() : Key(), Value()
{
}

FUtilityAiMemoryKeyValue::FUtilityAiMemoryKeyValue(const FName& InKey,
	const FUtilityAiMemoryValue& InValue)
	: Key(InKey), Value(InValue)
{
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue()
{
	this->Inner.Set<FEmptyVariantState>({});
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(bool bValue)
{
	this->Inner.Set<bool>(bValue);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(uint32 Value)
{
	this->Inner.Set<uint32>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(int32 Value)
{
	this->Inner.Set<int32>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(float Value)
{
	this->Inner.Set<float>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(const FVector& Value)
{
	this->Inner.Set<FVector>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(const FRotator& Value)
{
	this->Inner.Set<FRotator>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(const FName& Value)
{
	this->Inner.Set<FName>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(const FString& Value)
{
	this->Inner.Set<FString>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(UObject* Value)
{
	this->Inner.Set<UObject*>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(const TArray<FUtilityAiMemoryValue>& Value)
{
	this->Inner.Set<TArray<FUtilityAiMemoryValue>>(Value);
}

FUtilityAiMemoryValue::FUtilityAiMemoryValue(const TArray<FUtilityAiMemoryKeyValue>& Value)
{
	this->Inner.Set<TArray<FUtilityAiMemoryKeyValue>>(Value);
}

bool* FUtilityAiMemoryValue::AsBool()
{
	return this->Inner.TryGet<bool>();
}

uint32* FUtilityAiMemoryValue::AsUInt32()
{
	return this->Inner.TryGet<uint32>();
}

int32* FUtilityAiMemoryValue::AsInt32()
{
	return this->Inner.TryGet<int32>();
}

float* FUtilityAiMemoryValue::AsFloat()
{
	return this->Inner.TryGet<float>();
}

FVector* FUtilityAiMemoryValue::AsVector()
{
	return this->Inner.TryGet<FVector>();
}

FRotator* FUtilityAiMemoryValue::AsRotator()
{
	return this->Inner.TryGet<FRotator>();
}

FName* FUtilityAiMemoryValue::AsName()
{
	return this->Inner.TryGet<FName>();
}

FString* FUtilityAiMemoryValue::AsString()
{
	return this->Inner.TryGet<FString>();
}

UObject* FUtilityAiMemoryValue::AsObject()
{
	auto** Result = this->Inner.TryGet<UObject*>();
	return Result != nullptr ? *Result : nullptr;
}

TArray<FUtilityAiMemoryValue>* FUtilityAiMemoryValue::AsArray()
{
	return this->Inner.TryGet<TArray<FUtilityAiMemoryValue>>();
}

TArray<FUtilityAiMemoryKeyValue>* FUtilityAiMemoryValue::AsDictionary()
{
	return this->Inner.TryGet<TArray<FUtilityAiMemoryKeyValue>>();
}

void FUtilityAiMemoryValue::Clear()
{
	this->Inner.Set<FEmptyVariantState>({});
}
