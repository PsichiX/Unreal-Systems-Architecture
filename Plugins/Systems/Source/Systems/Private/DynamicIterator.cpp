#include "Systems/Public/DynamicIterator.h"

UObject* UDynamicIterator::NextBranched(EDynamicQueryBranch& Branches)
{
	auto* Result = Next();
	Branches = IsValid(Result) ? EDynamicQueryBranch::Continue : EDynamicQueryBranch::End;
	return Result;
}
