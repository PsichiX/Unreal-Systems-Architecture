#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T, typename I, typename F>
struct TIterMap
{
public:
	using Self = TIterMap<T, I, F>;
	using Item = T;
	using FunctorType = T(typename I::Item& Value);

	TIterMap() : Inner(), Functor()
	{
	}

	TIterMap(I&& Iter, F Func) : Inner(Iter), Functor(Func)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		auto Value = this->Inner.Next();
		if (Value.IsSet())
		{
			return TOptional<Item>(this->Functor(Value.GetValue()));
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
	TFunction<FunctorType> Functor = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename T, typename I, typename F>
TIterMap<T, I, F> IterMap(I&& Iter, F Functor)
{
	//// [ignore]
	return TIterMap<T, I, F>(MoveTempIfPossible(Iter), Functor);
	//// [/ignore]
}
