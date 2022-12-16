#include "Systems/Public/DynamicIterator/DynamicChain.h"

UObject* UDynamicChain::Next()
{
	if (IsValid(this->InnerA) == false || IsValid(this->InnerB) == false)
	{
		return nullptr;
	}
	auto* Value = this->InnerA->Next();
	if (IsValid(Value))
	{
		return Value;
	}
	return this->InnerB->Next();
}

int UDynamicChain::EstimateSizeLeft() const
{
	return IsValid(this->InnerA) && IsValid(this->InnerB)
		? this->InnerA->EstimateSizeLeft() + this->InnerB->EstimateSizeLeft()
		: 0;
}
