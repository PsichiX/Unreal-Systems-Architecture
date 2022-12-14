#pragma once

#include "CoreMinimal.h"

template <typename I>
void IterCollectIntoArray(I&& Iterator, TArray<typename I::Item>& Result)
{
	//// [ignore]
	const auto SizeHint = Iterator.SizeHint();
	const auto Capacity = SizeHint.Maximum.IsSet() ? SizeHint.Maximum.GetValue() : SizeHint.Minimum;
	Result.Reserve(Result.Num() + Capacity);
	while (auto QueryItem = Iterator.Next())
	{
		Result.Add(QueryItem.GetValue());
	}
	//// [/ignore]
}

template <typename I>
TArray<typename I::Item> IterCollectArray(I&& Iterator)
{
	//// [ignore]
	auto Result = TArray<I::Item>();
	IterCollectIntoArray<I>(MoveTempIfPossible(Iterator), Result);
	return Result;
	//// [/ignore]
}

template <typename I>
void IterCollectIntoSet(I&& Iterator, TSet<typename I::Item>& Result)
{
	//// [ignore]
	const auto SizeHint = Iterator.SizeHint();
	const auto Capacity = SizeHint.Maximum.IsSet() ? SizeHint.Maximum.GetValue() : SizeHint.Minimum;
	Result.Reserve(Result.Num() + Capacity);
	while (auto QueryItem = Iterator.Next())
	{
		Result.Add(QueryItem.GetValue());
	}
	//// [/ignore]
}

template <typename I>
TSet<typename I::Item> IterCollectSet(I&& Iterator)
{
	//// [ignore]
	auto Result = TSet<I::Item>();
	IterCollectIntoSet<I>(MoveTempIfPossible(Iterator), Result);
	return Result;
	//// [/ignore]
}

template <typename K, typename V, typename I>
void IterCollectIntoMap(I&& Iterator, TMap<K, V>& Result)
{
	//// [ignore]
	const auto SizeHint = Iterator.SizeHint();
	const auto Capacity = SizeHint.Maximum.IsSet() ? SizeHint.Maximum.GetValue() : SizeHint.Minimum;
	Result.Reserve(Result.Num() + Capacity);
	while (auto QueryItem = Iterator.Next())
	{
		const auto& Value = QueryItem.GetValue();
		Result.Add(Value.Get<0>(), Value.Get<1>());
	}
	//// [/ignore]
}

template <typename K, typename V, typename I>
TMap<K, V> IterCollectMap(I&& Iterator)
{
	//// [ignore]
	auto Result = TMap<K, V>();
	IterCollectIntoMap<K, V, I>(MoveTempIfPossible(Iterator), Result);
	return Result;
	//// [/ignore]
}
