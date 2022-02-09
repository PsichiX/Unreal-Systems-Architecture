#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename I, uint32 C>
struct TIterViews
{
public:
	using Self = TIterViews<I, C>;
	using Item = TArrayView<typename I::Item>;

	TIterViews() : Inner(), Items()
	{
	}

	TIterViews(I&& Iter) : Inner(Iter), Items()
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		if (C == 0)
		{
			return TOptional<Item>();
		}
		if (this->Items.Num() < static_cast<int>(C))
		{
			while (this->Items.Num() < static_cast<int>(C))
			{
				const auto Value = this->Inner.Next();
				if (Value.IsSet() == false)
				{
					return TOptional<Item>();
				}
				this->Items.Add(Value.GetValue());
			}
			return TOptional<Item>(this->Items);
		}
		else
		{
			const auto Value = this->Inner.Next();
			if (Value.IsSet())
			{
				this->Items.RemoveAt(0);
				this->Items.Add(Value.GetValue());
				return TOptional<Item>(this->Items);
			}
			return TOptional<Item>();
		}
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		auto Hint = this->Inner.SizeHint();
		if (Hint.Minimum + 1 > C)
		{
			Hint.Minimum = Hint.Minimum + 1 - C;
		}
		if (Hint.Maximum.IsSet() && Hint.Maximum.GetValue() + 1 > C)
		{
			Hint.Maximum.Emplace(Hint.Maximum.GetValue() + 1 - C);
		}
		return Hint;
		//// [/ignore]
	}

private:
	I Inner = {};
	TArray<typename I::Item> Items = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename I, uint32 C>
TIterViews<I, C> IterViews(I&& Iter)
{
	//// [ignore]
	return TIterViews<I, C>(MoveTemp(Iter));
	//// [/ignore]
}
