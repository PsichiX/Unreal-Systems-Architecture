#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename IA, typename IB>
struct TIterZip
{
public:
	using Self = TIterZip<IA, IB>;
	using Item = TTuple<typename IA::Item, typename IB::Item>;

	TIterZip() : InnerA(), InnerB()
	{
	}

	TIterZip(IA&& IterA, IB&& IterB) : InnerA(IterA), InnerB(IterB)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		const auto A = this->InnerA.Next();
		const auto B = this->InnerB.Next();
		if (A.IsSet() && B.IsSet())
		{
			return TOptional<Item>(MakeTuple(A.GetValue(), B.GetValue()));
		}
		return TOptional<Item>();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		const auto A = this->InnerA.SizeHint();
		const auto B = this->InnerB.SizeHint();
		const auto Min = FMath::Min(A.Minimum, B.Minimum);
		const auto Max = A.Maximum.IsSet() && B.Maximum.IsSet()
			? FMath::Max(A.Maximum.GetValue(), B.Maximum.GetValue())
			: TOptional<uint32>();
		return IterSizeHint{Min, Max};
		//// [/ignore]
	}

private:
	IA InnerA = {};
	IB InnerB = {};

public:
	//// [inject: iter]
	ITER_IMPL
};

template <typename IA, typename IB>
TIterZip<IA, IB> IterZip(IA&& IterA, IB&& IterB)
{
	//// [ignore]
	return TIterZip<IA, IB>(
		MoveTempIfPossible(IterA), MoveTempIfPossible(IterB));
	//// [/ignore]
}
