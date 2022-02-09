#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T, typename F>
struct TIterGenerate
{
public:
	using Self = TIterGenerate<T, F>;
	using Item = T;
	using FunctorType = T();

	TIterGenerate() : Functor()
	{
	}

	TIterGenerate(F Func) : Functor(Func)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		return TOptional<T>(this->Functor());
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return IterSizeHint{0, TOptional<uint32>()};
		//// [/ignore]
	}

private:
	TFunction<FunctorType> Functor = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

/// Iterator that generates values infinitely.
///
/// Useful for example for yielding random values, or anything that user would
/// want to generate procedurally. It is useful to combine it with `Take`
/// iterator to limit number of iterations in loop or for collecting values it
/// yields.
///
/// # Example
/// ```snippet
/// iter_generate
/// ```
template <typename T, typename F>
TIterGenerate<T, F> IterGenerate(
	/// Functor that should comply to signature: `T()`.
	F Functor)
{
	//// [ignore]
	return TIterGenerate<T, F>(Functor);
	//// [/ignore]
}
