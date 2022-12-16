#include "Systems/Public/DynamicIterator/DynamicTake.h"

UObject* UDynamicTake::Next()
{
	if (IsValid(this->Inner) == false)
	{
		return nullptr;
	}
	if (this->Num > 0)
	{
		auto* Value = this->Inner->Next();
		if (IsValid(Value))
		{
			--this->Num;
			return Value;
		}
	}
	return nullptr;
}

int UDynamicTake::EstimateSizeLeft() const
{
	return IsValid(this->Inner) ? FMath::Min(this->Inner->EstimateSizeLeft(), this->Num) : 0;
}
