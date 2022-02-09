#pragma once

#include "CoreMinimal.h"

template <typename I>
uint32 IterCount(I&& Iterator)
{
	//// [ignore]
	auto Result = 0;
	while (const auto Value = Iterator.Next())
	{
		++Result;
	}
	return Result;
	//// [/ignore]
}

template <typename I>
TOptional<typename I::Item> IterNth(I&& Iterator, uint32 Index)
{
	//// [ignore]
	while (Index > 0)
	{
		--Index;
		Iterator.Next();
	}
	return Iterator.Next();
	//// [/ignore]
}

template <typename T, typename I, typename F>
T IterFold(I&& Iterator, T Start, F Functor)
{
	//// [ignore]
	while (auto Value = Iterator.Next())
	{
		Start = Functor(Start, Value.GetValue());
	}
	return Start;
	//// [/ignore]
}

template <typename I>
typename I::Item IterSum(I&& Iterator, typename I::Item InitialValue)
{
	//// [ignore]
	auto Result = InitialValue;
	while (const auto Value = Iterator.Next())
	{
		Result += Value.GetValue();
	}
	return Result;
	//// [/ignore]
}

template <typename I, typename F>
TOptional<typename I::Item> IterFind(I&& Iterator, F Functor)
{
	//// [ignore]
	while (auto Value = Iterator.Next())
	{
		if (Functor(Value.GetValue()))
		{
			return Value;
		}
	}
	return TOptional<I::Item>();
	//// [/ignore]
}

template <typename T, typename I, typename F>
TOptional<T> IterFindMap(I&& Iterator, F Functor)
{
	//// [ignore]
	while (auto Value = Iterator.Next())
	{
		if (auto Result = Functor(Value.GetValue()))
		{
			return Result;
		}
	}
	return TOptional<T>();
	//// [/ignore]
}

template <typename I>
TOptional<typename I::Item> IterFirst(I&& Iterator)
{
	//// [ignore]
	return Iterator.Next();
	//// [/ignore]
}

template <typename I>
TOptional<typename I::Item> IterLast(I&& Iterator)
{
	//// [ignore]
	auto Last = TOptional<typename I::Item>();
	while (const auto Value = Iterator.Next())
	{
		Last = Value;
	}
	return Last;
	//// [/ignore]
}

template <typename I, typename F>
void IterForEach(I&& Iterator, F Functor)
{
	//// [ignore]
	while (auto Value = Iterator.Next())
	{
		Functor(Value.GetValue());
	}
	//// [/ignore]
}

template <typename I, typename F>
bool IterAny(I&& Iterator, F Functor)
{
	//// [ignore]
	while (auto Value = Iterator.Next())
	{
		if (Functor(Value.GetValue()))
		{
			return true;
		}
	}
	return false;
	//// [/ignore]
}

template <typename I, typename F>
bool IterAll(I&& Iterator, F Functor)
{
	//// [ignore]
	while (auto Value = Iterator.Next())
	{
		if (Functor(Value.GetValue()) == false)
		{
			return false;
		}
	}
	return true;
	//// [/ignore]
}

template <typename I, typename F>
TOptional<typename I::Item> IterComparedBy(I&& Iterator, F Functor)
{
	//// [ignore]
	auto Result = TOptional<typename I::Item>();
	while (auto Value = Iterator.Next())
	{
		if (Result.IsSet())
		{
			if (Functor(Value.GetValue(), Result.GetValue()))
			{
				Result = Value;
			}
		}
		else
		{
			Result = Value;
		}
	}
	return Result;
	//// [/ignore]
}
