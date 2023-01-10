#include "Systems/Public/SystemsReflection.h"

static TOptional<FSystemsReflection>& GetSingleton()
{
	static TOptional<FSystemsReflection> Singleton(InPlace);
	return Singleton;
}

FSystemsReflection& FSystemsReflection::Get()
{
	return GetSingleton().GetValue();
}

void FSystemsReflection::Register(FName Name, TFunction<SystemsWorld::LambdaSystemType> Callback)
{
	FScopeLock Lock(&this->CriticalSection);
	this->Registry.Add(Name, Callback);
}

void FSystemsReflection::Unregister(FName Name)
{
	FScopeLock Lock(&this->CriticalSection);
	this->Registry.Remove(Name);
}

void FSystemsReflection::GetNames(TArray<FString>& Result) const
{
	FScopeLock Lock(&this->CriticalSection);
	IterStdConst(this->Registry)
		.Map<FString>([](const auto& Pair) { return Pair.Key.ToString(); })
		.CollectIntoArray(Result);
}

TOptional<TFunction<SystemsWorld::LambdaSystemType>> FSystemsReflection::FindByName(FName Name) const
{
	FScopeLock Lock(&this->CriticalSection);
	const auto* Found = this->Registry.Find(Name);
	if (Found != nullptr)
	{
		return *Found;
	}
	return {};
}

TArray<FString> USystemsReflection::GetOptions()
{
	TArray<FString> Result = {};
	Result.Add(FName().ToString());
	FSystemsReflection::Get().GetNames(Result);
	return Result;
}