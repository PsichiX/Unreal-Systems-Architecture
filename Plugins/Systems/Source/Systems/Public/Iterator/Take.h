#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename I>
struct TIterTake
{
public:
	using Self = TIterTake<I>;
	using Item = typename I::Item;

	TIterTake() : Inner(), Num(0)
	{
	}

	TIterTake(I&& Iter, uint32 Count) : Inner(Iter), Num(Count)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		if (this->Num > 0)
		{
			if (const auto Value = this->Inner.Next())
			{
				--this->Num;
				return Value;
			}
		}
		return TOptional<Item>();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		auto Hint = this->Inner.SizeHint();
		if (Hint.Minimum > this->Num)
		{
			Hint.Minimum = this->Num;
		}
		if (Hint.Maximum.IsSet() && Hint.Maximum.GetValue() > this->Num)
		{
			Hint.Maximum.Emplace(Hint.Maximum.GetValue() - this->Num);
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
TIterTake<I> IterTake(I&& Iter, uint32 Count)
{
	//// [ignore]
	return TIterTake<I>(MoveTemp(Iter), Count);
	//// [/ignore]
}
