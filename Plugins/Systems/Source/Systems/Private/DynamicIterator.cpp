#include "Systems/Public/DynamicIterator.h"

#include "Systems/Public/DynamicIterator/DynamicChain.h"
#include "Systems/Public/DynamicIterator/DynamicEnumerate.h"
#include "Systems/Public/DynamicIterator/DynamicFilter.h"
#include "Systems/Public/DynamicIterator/DynamicFilterMap.h"
#include "Systems/Public/DynamicIterator/DynamicInspect.h"
#include "Systems/Public/DynamicIterator/DynamicMap.h"
#include "Systems/Public/DynamicIterator/DynamicSkip.h"
#include "Systems/Public/DynamicIterator/DynamicTake.h"
#include "Systems/Public/DynamicIterator/DynamicZip.h"

UObject* UDynamicIterator::NextBranched(EDynamicIteratorNextBranch& Branches)
{
	auto* Result = Next();
	Branches = IsValid(Result) ? EDynamicIteratorNextBranch::Continue : EDynamicIteratorNextBranch::End;
	return Result;
}

void UDynamicIterator::CollectArray(TSubclassOf<UObject> Type, TArray<UObject*>& Result)
{
	Result.Empty();
	Result.Reserve(EstimateSizeLeft());
	while (true)
	{
		auto* Item = Next();
		if (IsValid(Item) == false)
		{
			return;
		}
		if (Item->GetClass() == Type.Get())
		{
			Result.Add(Item);
		}
	}
}

void UDynamicIterator::CollectSet(TSubclassOf<UObject> Type, TSet<UObject*>& Result)
{
	Result.Empty();
	Result.Reserve(EstimateSizeLeft());
	while (true)
	{
		auto* Item = Next();
		if (IsValid(Item) == false)
		{
			return;
		}
		if (Item->GetClass() == Type.Get())
		{
			Result.Add(Item);
		}
	}
}

int UDynamicIterator::Count()
{
	auto Result = 0;
	while (IsValid(Next()))
	{
		++Result;
	}
	return Result;
}

void UDynamicIterator::Nth(TSubclassOf<UObject> Type,
	int Index,
	UObject*& Result,
	EDynamicIteratorStatusBranch& Branches)
{
	Result = nullptr;
	Branches = EDynamicIteratorStatusBranch::Failure;
	while (Index > 0)
	{
		--Index;
		Next();
	}
	auto* Item = Next();
	if (IsValid(Item) && Item->GetClass() == Type.Get())
	{
		Result = Item;
		Branches = EDynamicIteratorStatusBranch::Success;
	}
}

// void UDynamicIterator::Fold(TSubclassOf<UObject> Type,
// 	UObject* StartValue,
// 	FDynamicFoldEvent OnFold,
// 	UObject*& Result,
// 	EDynamicIteratorStatusBranch& Branches)
// {
// }

void UDynamicIterator::Find(TSubclassOf<UObject> Type,
	FDynamicFindEvent OnFind,
	UObject*& Result,
	EDynamicIteratorStatusBranch& Branches)
{
	Result = nullptr;
	Branches = EDynamicIteratorStatusBranch::Failure;
	auto* Output = NewObject<UDynamicFindOutput>(this);
	while (true)
	{
		auto* Input = Next();
		if (IsValid(Input) == false)
		{
			return;
		}
		Output->Value = false;
		OnFind.ExecuteIfBound(Input, Output);
		if (Output->Value && Input->GetClass() == Type.Get())
		{
			Result = Input;
			Branches = EDynamicIteratorStatusBranch::Success;
			return;
		}
	}
}

void UDynamicIterator::FindMap(TSubclassOf<UObject> Type,
	FDynamicFindMapEvent OnFindMap,
	UObject*& Result,
	EDynamicIteratorStatusBranch& Branches)
{
	Result = nullptr;
	Branches = EDynamicIteratorStatusBranch::Failure;
	auto* Output = NewObject<UDynamicFindMapOutput>(this);
	while (true)
	{
		auto* Input = Next();
		if (IsValid(Input) == false)
		{
			return;
		}
		Output->Value = nullptr;
		OnFindMap.ExecuteIfBound(Input, Output);
		if (IsValid(Output->Value) && Output->Value->GetClass() == Type.Get())
		{
			Result = Output->Value;
			Branches = EDynamicIteratorStatusBranch::Success;
			return;
		}
	}
}

void UDynamicIterator::First(TSubclassOf<UObject> Type, UObject*& Result, EDynamicIteratorStatusBranch& Branches)
{
	Result = nullptr;
	Branches = EDynamicIteratorStatusBranch::Failure;
	auto* Item = Next();
	if (IsValid(Item) && Item->GetClass() == Type.Get())
	{
		Result = Item;
		Branches = EDynamicIteratorStatusBranch::Success;
	}
}

void UDynamicIterator::Last(TSubclassOf<UObject> Type, UObject*& Result, EDynamicIteratorStatusBranch& Branches)
{
	Result = nullptr;
	Branches = EDynamicIteratorStatusBranch::Failure;
	while (true)
	{
		auto* Item = Next();
		if (IsValid(Result) == false)
		{
			return;
		}
		if (Item->GetClass() == Type.Get())
		{
			Result = Item;
			Branches = EDynamicIteratorStatusBranch::Success;
		}
	}
}

void UDynamicIterator::ForEach(FDynamicForEachEvent OnForEach)
{
	auto* Input = Next();
	if (IsValid(Input))
	{
		OnForEach.ExecuteIfBound(Input);
	}
}

bool UDynamicIterator::Any(FDynamicAnyEvent OnAny)
{
	auto* Output = NewObject<UDynamicAnyOutput>(this);
	while (true)
	{
		auto* Input = Next();
		if (IsValid(Input) == false)
		{
			return false;
		}
		Output->Value = false;
		OnAny.ExecuteIfBound(Input, Output);
		if (Output->Value)
		{
			return true;
		}
	}
	return false;
}

bool UDynamicIterator::All(FDynamicAllEvent OnAll)
{
	auto* Output = NewObject<UDynamicAllOutput>(this);
	while (true)
	{
		auto* Input = Next();
		if (IsValid(Input) == false)
		{
			return false;
		}
		Output->Value = false;
		OnAll.ExecuteIfBound(Input, Output);
		if (Output->Value == false)
		{
			return false;
		}
	}
	return true;
}

void UDynamicIterator::ComparedBy(TSubclassOf<UObject> Type,
	FDynamicComparedByEvent OnComparedBy,
	UObject*& Result,
	EDynamicIteratorStatusBranch& Branches)
{
	Result = nullptr;
	Branches = EDynamicIteratorStatusBranch::Failure;
	auto* Output = NewObject<UDynamicComparedByOutput>(this);
	while (true)
	{
		auto* Input = Next();
		if (IsValid(Input) == false)
		{
			return;
		}
		if (IsValid(Result))
		{
			Output->Value = false;
			OnComparedBy.ExecuteIfBound(Input, Result, Output);
			if (Output->Value)
			{
				Result = Input;
				Branches = EDynamicIteratorStatusBranch::Success;
			}
		}
		else
		{
			Result = Input;
			Branches = EDynamicIteratorStatusBranch::Success;
		}
	}
}

UDynamicChain* UDynamicIterator::Chain(UDynamicIterator* Other)
{
	auto* Result = NewObject<UDynamicChain>(this);
	Result->InnerA = this;
	Result->InnerB = Other;
	return Result;
}

UDynamicFilter* UDynamicIterator::Filter(FDynamicFilterEvent OnFilter)
{
	auto* Result = NewObject<UDynamicFilter>(this);
	Result->Inner = this;
	Result->Event = OnFilter;
	return Result;
}

UDynamicMap* UDynamicIterator::Map(FDynamicMapEvent OnMap)
{
	auto* Result = NewObject<UDynamicMap>(this);
	Result->Inner = this;
	Result->Event = OnMap;
	return Result;
}

UDynamicFilterMap* UDynamicIterator::FilterMap(FDynamicFilterMapEvent OnFilterMap)
{
	auto* Result = NewObject<UDynamicFilterMap>(this);
	Result->Inner = this;
	Result->Event = OnFilterMap;
	return Result;
}

UDynamicInspect* UDynamicIterator::Inspect(FDynamicInspectEvent OnInspect)
{
	auto* Result = NewObject<UDynamicInspect>(this);
	Result->Inner = this;
	Result->Event = OnInspect;
	return Result;
}

UDynamicSkip* UDynamicIterator::Skip(int Num)
{
	auto* Result = NewObject<UDynamicSkip>(this);
	Result->Inner = this;
	Result->Num = Num;
	return Result;
}

UDynamicTake* UDynamicIterator::Take(int Num)
{
	auto* Result = NewObject<UDynamicTake>(this);
	Result->Inner = this;
	Result->Num = Num;
	return Result;
}

UDynamicEnumerate* UDynamicIterator::Enumerate()
{
	auto* Result = NewObject<UDynamicEnumerate>(this);
	Result->Inner = this;
	return Result;
}

UDynamicZip* UDynamicIterator::Zip(UDynamicIterator* Other)
{
	auto* Result = NewObject<UDynamicZip>(this);
	Result->InnerA = this;
	Result->InnerB = Other;
	return Result;
}
