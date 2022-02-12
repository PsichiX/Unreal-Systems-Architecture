#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename I, typename F>
struct TIterInspect
{
public:
	using Self = TIterInspect<I, F>;
	using Item = typename I::Item;
	using FunctorType = void(typename I::Item& Value);

	TIterInspect() : Inner(), Functor()
	{
	}

	TIterInspect(I&& Iter, F Func) : Inner(Iter), Functor(Func)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		if (auto Value = this->Inner.Next())
		{
			this->Functor(Value.GetValue());
			return Value;
		}
		return TOptional<Item>();
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

template <typename I, typename F>
TIterInspect<I, F> IterInspect(I&& Iter, F Functor)
{
	//// [ignore]
	return TIterInspect<I, F>(MoveTempIfPossible(Iter), Functor);
	//// [/ignore]
}
