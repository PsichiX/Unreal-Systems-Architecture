#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename T>
struct TIterStd
{
public:
	using Self = TIterStd<T>;
	using Item = typename T::ElementType;

	TIterStd() : Inner(), Num(0)
	{
	}

	TIterStd(T& Container)
		//// [ignore]
		: Inner(Container.CreateIterator()), Num(static_cast<uint32>(Container.Num()))
	//// [/ignore]
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		if (this->Inner)
		{
			const auto Value = *this->Inner;
			++this->Inner;
			if (this->Num > 0)
			{
				--this->Num;
			}
			return TOptional<Item>(Value);
		}
		return TOptional<Item>();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return IterSizeHint{this->Num, TOptional<uint32>(this->Num)};
		//// [/ignore]
	}

private:
	typename T::TIterator Inner = {};
	uint32 Num = 0;

public:
	ITER_IMPL
};

/// Iterator that yields mutable values from standard Unreal Engine collections
/// (such as arrays, sets and maps).
///
/// # Example
/// ```snippet
/// iter_std
/// ```
template <typename T>
TIterStd<T> IterStd(
	/// Reference to Unreal Engine collection.
	T& Container)
{
	//// [ignore]
	return TIterStd<T>(Container);
	//// [/ignore]
}

template <typename T>
struct TIterStdConst
{
public:
	using Self = TIterStdConst<T>;
	using Item = const typename T::ElementType;

	TIterStdConst() : Inner(), Num(0)
	{
	}

	TIterStdConst(const T& Container)
		//// [ignore]
		: Inner(Container.CreateConstIterator()), Num(static_cast<uint32>(Container.Num()))
	//// [/ignore]
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		if (this->Inner)
		{
			const auto Value = *this->Inner;
			++this->Inner;
			if (this->Num > 0)
			{
				--this->Num;
			}
			return TOptional<Item>(Value);
		}
		return TOptional<Item>();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		return IterSizeHint{this->Num, TOptional<uint32>(this->Num)};
		//// [/ignore]
	}

private:
	typename T::TConstIterator Inner = {};
	uint32 Num = 0;

public:
	//// [inject: iter]
	ITER_IMPL
};

/// Iterator that yields immutable values from standard Unreal Engine
/// collections (such as arrays, sets and maps).
///
/// # Example
/// ```snippet
/// iter_std_const
/// ```
template <typename T>
TIterStdConst<T> IterStdConst(
	/// Reference to Unreal Engine collection.
	const T& Container)
{
	//// [ignore]
	return TIterStdConst<T>(Container);
	//// [/ignore]
}
