#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T>
struct TIterRepeat
{
public:
	using Self = TIterRepeat<T>;
	using Item = typename T;

	TIterRepeat() : Value(TOptional<T>())
	{
	}

	TIterRepeat(T Data) : Value(TOptional<T>(Data))
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		return TOptional<T>(this->Value);
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return IterSizeHint{0, TOptional<uint32>()};
		//// [/ignore]
	}

private:
	TOptional<T> Value = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

/// Iterator that yields one value infinitely.
///
/// Useful to combine it with `Take` iterator to avoid infinite loops or
/// collecting iterator values that never ends.
///
/// # Example
/// ```snippet
/// iter_repeat
/// ```
template <typename T>
TIterRepeat<T> IterRepeat(
	/// Value that iterator should yield infinitely.
	T Value)
{
	//// [ignore]
	return TIterRepeat<T>(Value);
	//// [/ignore]
}
