#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T, typename I>
struct TIterCast
{
public:
	using Self = TIterCast<T, I>;
	using Item = T;

	TIterCast() : Inner()
	{
	}

	TIterCast(I&& Iter) : Inner(Iter)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		auto Value = this->Inner.Next();
		if (Value.IsSet())
		{
			return TOptional<Item>(Item(Value.GetValue()));
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

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename T, typename I>
TIterCast<T, I> IterCast(I&& Iter)
{
	//// [ignore]
	return TIterCast<T, I>(MoveTemp(Iter));
	//// [/ignore]
}
