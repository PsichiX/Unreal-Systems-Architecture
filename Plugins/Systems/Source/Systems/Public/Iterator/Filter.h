#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename I, typename F>
struct TIterFilter
{
public:
	using Self = TIterFilter<I, F>;
	using Item = typename I::Item;
	using FunctorType = bool(typename I::Item& Value);

	TIterFilter() : Inner(), Functor()
	{
	}

	TIterFilter(I&& Iter, F Func) : Inner(Iter), Functor(Func)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		while (auto Value = this->Inner.Next())
		{
			if (this->Functor(Value.GetValue()))
			{
				return Value;
			}
		}
		return TOptional<Item>();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		auto Hint = this->Inner.SizeHint();
		Hint.Minimum = 0;
		return Hint;
		//// [/ignore]
	}

private:
	I Inner = {};
	TFunction<FunctorType> Functor = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename I, typename F>
TIterFilter<I, F> IterFilter(I&& Iter, F Functor)
{
	//// [ignore]
	return TIterFilter<I, F>(MoveTempIfPossible(Iter), Functor);
	//// [/ignore]
}
