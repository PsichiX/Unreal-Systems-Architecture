#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename I>
struct TIterEnumerate
{
public:
	using Self = TIterEnumerate<I>;
	using Item = TTuple<uint32, typename I::Item>;

	TIterEnumerate() : Inner()
	{
	}

	TIterEnumerate(I&& Iter) : Inner(Iter)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		const auto Value = this->Inner.Next();
		if (Value.IsSet())
		{
			return TOptional<Item>(MakeTuple(this->Index++, Value.GetValue()));
		}
		else
		{
			return TOptional<Item>();
		}
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return this->Inner.SizeHint();
		//// [/ignore]
	}

private:
	I Inner = {};
	uint32 Index = 0;

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename I>
TIterEnumerate<I> IterEnumerate(I&& Iter)
{
	//// [ignore]
	return TIterEnumerate<I>(MoveTempIfPossible(Iter));
	//// [/ignore]
}
