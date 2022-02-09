#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T, typename I, typename F>
struct TIterFilterMap
{
public:
	using Self = TIterFilterMap<T, I, F>;
	using Item = T;
	using FunctorType = TOptional<T>(typename I::Item& Value);

	TIterFilterMap() : Inner(), Functor()
	{
	}

	TIterFilterMap(I&& Iter, F Func) : Inner(Iter), Functor(Func)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		while (auto Value = this->Inner.Next())
		{
			if (auto Mapped = this->Functor(Value.GetValue()))
			{
				return Mapped;
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

template <typename T, typename I, typename F>
TIterFilterMap<T, I, F> IterFilterMap(I&& Iter, F Functor)
{
	//// [ignore]
	return TIterFilterMap<T, I, F>(MoveTemp(Iter), Functor);
	//// [/ignore]
}
