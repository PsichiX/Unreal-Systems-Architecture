#include "Systems/Public/DynamicIterator/DynamicFilter.h"

UObject* UDynamicFilter::Next()
{
	if (IsValid(this->Inner) == false)
	{
		return nullptr;
	}
	auto* Output = NewObject<UDynamicFilterOutput>(this);
	while (true)
	{
		auto* Input = this->Inner->Next();
		if (IsValid(Input) == false)
		{
			break;
		}
		Output->Value = false;
		this->Event.ExecuteIfBound(Input, Output);
		if (Output->Value)
		{
			return Input;
		}
	}
	return nullptr;
}

int UDynamicFilter::EstimateSizeLeft() const
{
	return IsValid(this->Inner) ? this->Inner->EstimateSizeLeft() : 0;
}
