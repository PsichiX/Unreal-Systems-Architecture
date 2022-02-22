#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T>
struct TIterOddAdapter
{
public:
	template <typename I>
	TOptional<T> Next(I& Iter)
	{
		Iter.Next();
		return Iter.Next();
	}

	template <typename I>
	IterSizeHint SizeHint(const I& Iter) const
	{
		return Iter.SizeHint();
	}
};

void TestIterators();
