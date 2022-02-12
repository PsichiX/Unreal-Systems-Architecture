#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename I>
struct TIterSkip
{
public:
	using Self = TIterSkip<I>;
	using Item = typename I::Item;

	TIterSkip() : Inner(), Num(0)
	{
	}

	TIterSkip(I&& Iter, uint32 Count) : Inner(Iter), Num(Count)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		while (this->Num > 0)
		{
			--this->Num;
			this->Inner.Next();
		}
		return this->Inner.Next();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		auto Hint = this->Inner.SizeHint();
		if (this->Num < Hint.Minimum)
		{
			Hint.Minimum -= this->Num;
		}
		else
		{
			Hint.Minimum = 0;
		}
		if (Hint.Maximum.IsSet())
		{
			if (this->Num < Hint.Maximum.GetValue())
			{
				Hint.Maximum.Emplace(Hint.Maximum.GetValue() - this->Num);
			}
			else
			{
				Hint.Maximum.Emplace(0);
			}
		}
		return Hint;
		//// [/ignore]
	}

private:
	I Inner = {};
	uint32 Num = 0;

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename I>
TIterSkip<I> IterSkip(I&& Iter, uint32 Count)
{
	//// [ignore]
	return TIterSkip<I>(MoveTempIfPossible(Iter), Count);
	//// [/ignore]
}
