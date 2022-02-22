#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator/ConvertersDecl.h"

#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

template <typename IA, typename IB>
struct TIterChain
{
public:
	using Self = TIterChain<IA, IB>;
	using Item = typename IA::Item;

	TIterChain() : InnerA(), InnerB()
	{
	}

	TIterChain(IA&& IterA, IB&& IterB) : InnerA(IterA), InnerB(IterB)
	{
	}

	TOptional<Item> Next()
	{
		//// [ignore]
		if (auto Value = this->InnerA.Next())
		{
			return Value;
		}
		return this->InnerB.Next();
		//// [/ignore]
	}

	IterSizeHint SizeHint() const
	{
		//// [ignore]
		const auto A = this->InnerA.SizeHint();
		const auto B = this->InnerB.SizeHint();
		const auto Limit = A.Maximum.IsSet() && B.Maximum.IsSet()
			? A.Maximum.GetValue() + B.Maximum.GetValue()
			: TOptional<uint32>();
		return IterSizeHint{A.Minimum + B.Minimum, Limit};
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
TIterChain<IA, IB> IterChain(IA&& IterA, IB&& IterB)
{
	//// [ignore]
	return TIterChain<IA, IB>(
		MoveTempIfPossible(IterA), MoveTempIfPossible(IterB));
	//// [/ignore]
}
