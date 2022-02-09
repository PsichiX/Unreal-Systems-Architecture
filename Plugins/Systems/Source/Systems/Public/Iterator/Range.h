#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T>
struct TIterRange
{
public:
	using Self = TIterRange<T>;
	using Item = typename T;

	TIterRange() : From(default), To(default)
	{
	}

	TIterRange(T Lower, T Upper) : From(Lower), To(Upper)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		if (this->From < this->To)
		{
			return TOptional<T>(this->From++);
		}
		else
		{
			return TOptional<T>();
		}
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		const auto Count = static_cast<uint32>(this->To - this->From);
		return IterSizeHint{Count, TOptional<uint32>(Count)};
		//// [/ignore]
	}

private:
	T From = {};
	T To = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

/// Iterator that yields range of values.
///
/// Range:
/// - inclusive lower bound.
/// - exclusive upper bound.
/// For range from 0 to 5 it means yielding values: 0, 1, 2, 3, 4.
///
/// # Note
/// > This iterator is value type agnostic, all it requires from type is for it
/// to implemen `operator++` and `operator-`;
///
/// # Example
/// ```snippet
/// iter_range
/// ```
template <typename T>
TIterRange<T> IterRange(
	/// Lower inclusive bound.
	T From,
	/// Upper exclusive bound.
	T To)
{
	//// [ignore]
	return TIterRange<T>(From, To);
	//// [/ignore]
}
