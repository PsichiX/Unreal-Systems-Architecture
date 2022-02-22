#pragma once

#include "CoreMinimal.h"

#include "Misc/TVariant.h"

template <typename T>
struct TResultOk
{
	T Value;
};

template <typename E>
struct TResultError
{
	E Error;
};

/// Result type that can wrap either `T` value or `E` error data.
///
/// Useful for return types in functions that can return either valid value or
/// some error. Given implementation tries to match `TOptional` API as much as
/// possible.
///
/// # Example
/// ```snippet
/// result
/// ```
template <typename T, typename E>
struct TResult
{
public:
	//// [ignore]
	TResult() = delete;
	//// [/ignore]

	/// Copy constructor.
	TResult(
		/// Value data to copy from.
		const T& Value)
		: Inner()
	{
		//// [ignore]
		this->Inner.Set<TResultOk<T>>({Value});
		//// [/ignore]
	}

	/// Move constructor.
	TResult(
		/// Value to consume.
		T&& Value)
		: Inner()
	{
		//// [ignore]
		this->Inner.Set<TResultOk<T>>({MoveTempIfPossible(Value)});
		//// [/ignore]
	}

	/// Copy constructor.
	TResult(
		/// Error to copy from.
		const E& Error)
		: Inner()
	{
		//// [ignore]
		this->Inner.Set<TResultError<E>>({Error});
		//// [/ignore]
	}

	/// Move constructor.
	TResult(
		/// Error to consume.
		E&& Error)
		: Inner()
	{
		//// [ignore]
		this->Inner.Set<TResultError<E>>({MoveTempIfPossible(Error)});
		//// [/ignore]
	}

	/// Copy constructor.
	TResult(
		/// Result to copy from.
		const TResult& Other)
	{
		//// [ignore]
		this->Inner = Other;
		//// [/ignore]
	}

	/// Move constructor.
	TResult(
		/// Result to consume.
		TResult&& Other)
	{
		//// [ignore]
		this->Inner = MoveTempIfPossible(Other);
		//// [/ignore]
	}

	/// Tells if result wraps value data.
	bool IsOk() const
	{
		//// [ignore]
		return this->Inner.IsType<TResultOk<T>>();
		//// [/ignore]
	}

	/// Tells if result wraps error data.
	bool IsError() const
	{
		//// [ignore]
		return this->Inner.IsType<TResultError<E>>();
		//// [/ignore]
	}

	/// Returns reference to internal value.
	///
	/// # Note
	/// > It panics if result wraps error data.
	T& GetOk()
	{
		//// [ignore]
		check(IsOk());
		return this->Inner.Get<TResultOk<T>>().Value;
		//// [/ignore]
	}

	/// Returns reference to internal value.
	///
	/// # Note
	/// > It panics if result wraps error data.
	const T& GetOk() const
	{
		//// [ignore]
		check(IsOk());
		return this->Inner.Get<TResultOk<T>>().Value;
		//// [/ignore]
	}

	/// Returns reference to internal error.
	///
	/// # Note
	/// > It panics if result wraps value data.
	E& GetError()
	{
		//// [ignore]
		check(IsError());
		return this->Inner.Get<TResultError<E>>().Error;
		//// [/ignore]
	}

	/// Returns reference to internal error.
	///
	/// # Note
	/// > It panics if result wraps value data.
	const E& GetError() const
	{
		//// [ignore]
		check(IsError());
		return this->Inner.Get<TResultError<E>>().Error;
		//// [/ignore]
	}

	/// Returns option with cloned value data.
	///
	/// In case of result wrapping error data, it returns none.
	TOptional<T> AsOk() const
	{
		//// [ignore]
		return IsOk() ? TOptional<T>(this->Inner.Get<TResultOk<T>>().Value)
					  : TOptional<T>();
		//// [/ignore]
	}

	/// Returns option with cloned error data.
	///
	/// In case of result wrapping value data, it returns none.
	TOptional<E> AsError() const
	{
		//// [ignore]
		return IsError()
			? TOptional<E>(this->Inner.Get<TResultError<E>>().Error)
			: TOptional<E>();
		//// [/ignore]
	}

	/// Replaces internal data with cloned value data.
	void SetOk(
		/// Value to copy from.
		const T& Value)
	{
		//// [ignore]
		this->Inner.Set<TResultOk<T>>({Value});
		//// [/ignore]
	}

	/// Replaces internal data with moved value data.
	void SetOk(
		/// Value to consume.
		T&& Value)
	{
		//// [ignore]
		this->Inner.Set<TResultOk<T>>({MoveTempIfPossible(Value)});
		//// [/ignore]
	}

	/// Replaces internal data with cloned error data.
	void SetError(
		/// Error to copy from.
		const E& Error)
	{
		//// [ignore]
		this->Inner.Set<TResultError<E>>({Error});
		//// [/ignore]
	}

	/// Replaces internal data with moved error data.
	void SetError(
		/// Error to consume.
		E&& Error)
	{
		//// [ignore]
		this->Inner.Set<TResultError<E>>({MoveTempIfPossible(Error)});
		//// [/ignore]
	}

	/// Copies other result.
	TResult& operator=(
		/// Result to copy from.
		const TResult& Other)
	{
		//// [ignore]
		this->Inner = Other.Inner;
		return *this;
		//// [/ignore]
	}

	/// Consumes other result.
	TResult& operator=(
		/// Result to consume.
		TResult&& Other)
	{
		//// [ignore]
		this->Inner = MoveTempIfPossible(Other.Inner);
		return *this;
		//// [/ignore]
	}

	/// Replaces internal data with cloned value data.
	TResult& operator=(
		/// Value to copy from.
		const T& Value)
	{
		//// [ignore]
		this->Inner.Set<TResultOk<T>>({Value});
		return *this;
		//// [/ignore]
	}

	/// Replaces internal data with moved value data.
	TResult& operator=(
		/// Value to consume.
		T&& Value)
	{
		//// [ignore]
		this->Inner.Set<TResultOk<T>>({MoveTempIfPossible(Value)});
		return *this;
		//// [/ignore]
	}

	/// Replaces internal data with cloned error data.
	TResult& operator=(
		/// Error to copy from.
		const E& Error)
	{
		//// [ignore]
		this->Inner.Set<TResultError<E>>({Error});
		return *this;
		//// [/ignore]
	}

	/// Replaces internal data with cloned error data.
	TResult& operator=(
		/// Error to consume.
		E&& Error)
	{
		//// [ignore]
		this->Inner.Set<TResultError<E>>({MoveTempIfPossible(Error)});
		return *this;
		//// [/ignore]
	}

	/// Tells if two results are equal.
	///
	/// Two results must both wrap either value or error, and if they do then
	/// their data is compared.
	friend bool operator==(const TResult& Lhs, const TResult& Rhs)
	{
		//// [ignore]
		if (Lhs.IsOk() && Rhs.IsOk())
		{
			return Lhs.GetOk() == Rhs.GetOk();
		}
		else if (Lhs.IsError() && Rhs.IsError())
		{
			return Lhs.GetError() == Rhs.GetError();
		}
		return false;
		//// [/ignore]
	}

	/// Tells if two results aren't equal.
	///
	/// Two results must both wrap either value or error, and if they do then
	/// their data is compared.
	friend bool operator!=(const TResult& Lhs, const TResult& Rhs)
	{
		//// [ignore]
		return !(Lhs == Rhs);
		//// [/ignore]
	}

	/// Serializes given result.
	friend FArchive& operator<<(FArchive& Ar, TResult& Result)
	{
		//// [ignore]
		Ar << Result.Inner;
		return Ar;
		//// [/ignore]
	}

	/// Cast to boolean.
	///
	/// Handy shortcut for [`struct: TResult::IsOk`]().
	/// Useful when used in if statement condition.
	explicit operator bool() const
	{
		//// [ignore]
		return IsOk();
		//// [/ignore]
	}

private:
	TVariant<TResultOk<T>, TResultError<E>> Inner = {};
};
