#include "Systems/Public/DynamicIterator/DynamicZip.h"

UObject* UDynamicZip::Next()
{
	if (IsValid(this->InnerA) == false || IsValid(this->InnerB) == false)
	{
		return nullptr;
	}
	auto* First = this->InnerA->Next();
	auto* Second = this->InnerB->Next();
	if (IsValid(First) && IsValid(Second))
	{
		auto* Value = NewObject<UDynamicZipValue>(this);
		Value->First = First;
		Value->Second = Second;
		return Value;
	}
	return nullptr;
}

int UDynamicZip::EstimateSizeLeft() const
{
	return IsValid(this->InnerA) && IsValid(this->InnerB)
		? FMath::Min(this->InnerA->EstimateSizeLeft(), this->InnerB->EstimateSizeLeft())
		: 0;
}
