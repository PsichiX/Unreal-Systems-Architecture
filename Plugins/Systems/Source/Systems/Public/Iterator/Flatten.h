#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T, typename I>
struct TIterFlatten
{
public:
	using Self = TIterFlatten<T, I>;
	using Item = T;
	using InnerIter = typename I::Item;

	TIterFlatten() : Inner()
	{
	}

	TIterFlatten(I&& Iter) : Inner(Iter)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		auto Iterations = 10;
		while (Iterations-- > 0)
		{
			if (this->Current.IsSet() == false)
			{
				if (const auto Value = this->Inner.Next())
				{
					this->Current = Value;
				}
				else
				{
					return TOptional<Item>();
				}
			}
			else
			{
				if (const auto Value = this->Current.GetValue().Next())
				{
					return Value;
				}
				else
				{
					this->Current.Reset();
					continue;
				}
			}
		}
		return TOptional<Item>();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return IterSizeHint{0, TOptional<uint32>()};
		//// [/ignore]
	}

private:
	I Inner = {};
	TOptional<InnerIter> Current = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename T, typename I>
TIterFlatten<T, I> IterFlatten(I&& Iter)
{
	//// [ignore]
	return TIterFlatten<T, I>(MoveTemp(Iter));
	//// [/ignore]
}
