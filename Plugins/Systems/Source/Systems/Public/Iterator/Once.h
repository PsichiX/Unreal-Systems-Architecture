#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T>
struct TIterOnce
{
public:
	using Self = TIterOnce<T>;
	using Item = typename T;

	TIterOnce() : Value(TOptional<T>())
	{
	}

	TIterOnce(T Data) : Value(TOptional<T>(Data))
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		const auto Data = this->Value;
		this->Value.Reset();
		return Data;
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return IterSizeHint{0, TOptional<uint32>(1)};
		//// [/ignore]
	}

private:
	TOptional<T> Value = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

/// Iterator that yields one value only once.
///
/// Useful to use for yielding "leading/trailing/separator" values in chains of
/// multiple iterators.
///
/// # Example
/// ```snippet
/// iter_once
/// ```
template <typename T>
TIterOnce<T> IterOnce(
	/// Value that iterator should yield once.
	T Value)
{
	//// [ignore]
	return TIterOnce<T>(Value);
	//// [/ignore]
}
