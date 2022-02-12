#pragma once

#include "CoreMinimal.h"

#include "Misc/TVariant.h"

template <typename T>
struct TResultOk
{
	T Value;
};

template <typename T>
struct TResultError
{
	T Error;
};

// TODO: document.
template <typename T, typename E>
struct TResult
{
public:
	TResult() = delete;

	TResult(const T& Value) : Inner()
	{
		this->Inner.Set<TResultOk<T>>({Value});
	}

	TResult(T&& Value) : Inner()
	{
		this->Inner.Set<TResultOk<T>>({MoveTempIfPossible(Value)});
	}

	TResult(const E& Error) : Inner()
	{
		this->Inner.Set<TResultError<E>>({Error});
	}

	TResult(E&& Error) : Inner()
	{
		this->Inner.Set<TResultError<E>>({MoveTempIfPossible(Error)});
	}

	TResult(const TResult& Other)
	{
		this->Inner = Other;
	}

	TResult(TResult&& Other)
	{
		this->Inner = MoveTempIfPossible(Other);
	}

	bool IsOk() const
	{
		return this->Inner.IsType<TResultOk<T>>();
	}

	bool IsError() const
	{
		return this->Inner.IsType<TResultError<E>>();
	}

	T& GetOk()
	{
		check(IsOk());
		return this->Inner.Get<TResultOk<T>>().Value;
	}

	const T& GetOk() const
	{
		check(IsOk());
		return this->Inner.Get<TResultOk<T>>().Value;
	}

	E& GetError()
	{
		check(IsError());
		return this->Inner.Get<TResultError<E>>().Error;
	}

	const E& GetError() const
	{
		check(IsError());
		return this->Inner.Get<TResultError<E>>().Error;
	}

	TOptional<T> Ok() const
	{
		return IsOk() ? TOptional<T>(this->Inner.Get<TResultOk<T>>().Value)
					  : TOptional<T>();
	}

	TOptional<E> Error() const
	{
		return IsError()
			? TOptional<E>(this->Inner.Get<TResultError<E>>().Error)
			: TOptional<E>();
	}

	void SetOk(const T& Value)
	{
		this->Inner.Set<TResultOk<T>>({Value});
	}

	void SetOk(T&& Value)
	{
		this->Inner.Set<TResultOk<T>>({MoveTempIfPossible(Value)});
	}

	void SetError(const E& Error)
	{
		this->Inner.Set<TResultError<E>>({Error});
	}

	void SetError(E&& Error)
	{
		this->Inner.Set<TResultError<E>>({MoveTempIfPossible(Error)});
	}

	TResult& operator=(const TResult& Other)
	{
		this->Inner = Other.Inner;
		return *this;
	}

	TResult& operator=(TResult&& Other)
	{
		this->Inner = MoveTempIfPossible(Other.Inner);
		return *this;
	}

	TResult& operator=(const T& Value)
	{
		this->Inner.Set<TResultOk<T>>({Value});
		return *this;
	}

	TResult& operator=(T&& Value)
	{
		this->Inner.Set<TResultOk<T>>({MoveTempIfPossible(Value)});
		return *this;
	}

	TResult& operator=(const E& Error)
	{
		this->Inner.Set<TResultError<E>>({Error});
		return *this;
	}

	TResult& operator=(E&& Error)
	{
		this->Inner.Set<TResultError<E>>({MoveTempIfPossible(Error)});
		return *this;
	}

	friend bool operator==(const TResult& Lhs, const TResult& Rhs)
	{
		if (Lhs.IsOk() && Rhs.IsOk())
		{
			return Lhs.GetOk() == Rhs.GetOk();
		}
		else if (Lhs.IsError() && Rhs.IsError())
		{
			return Lhs.GetError() == Rhs.GetError();
		}
		return false;
	}

	friend bool operator!=(const TResult& Lhs, const TResult& Rhs)
	{
		return !(Lhs == Rhs);
	}

	friend FArchive& operator<<(FArchive& Ar, TResult& Result)
	{
		Ar << Result.Inner;
		return Ar;
	}

private:
	TVariant<TResultOk<T>, TResultError<E>> Inner = {};
};
