#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename I, typename A>
struct TIterAdapt
{
public:
	using Self = TIterAdapt<I, A>;
	using Item = typename I::Item;

	TIterAdapt() : Inner()
	{
	}

	TIterAdapt(I&& Iter, A Processor) : Inner(Iter), Adapter(Processor)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		return this->Adapter.Next(this->Inner);
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return this->Adapter.SizeHint<I>(this->Inner);
		//// [/ignore]
	}

private:
	I Inner = {};
	A Adapter = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename I, typename A>
TIterAdapt<I, A> IterAdapt(I&& Iter, A Adapter)
{
	//// [ignore]
	return TIterAdapt<I, A>(MoveTempIfPossible(Iter), Adapter);
	//// [/ignore]
}
