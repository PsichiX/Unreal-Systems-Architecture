#include "Systems/Public/DynamicIterator/DynamicSkip.h"

UObject* UDynamicSkip::Next()
{
	if (IsValid(this->Inner) == false)
	{
		return nullptr;
	}
	while (this->Num > 0)
	{
		--this->Num;
		this->Inner->Next();
	}
	return this->Inner->Next();
}

int UDynamicSkip::EstimateSizeLeft() const
{
	return IsValid(this->Inner) ? FMath::Max(this->Inner->EstimateSizeLeft() - this->Num, 0) : 0;
}
