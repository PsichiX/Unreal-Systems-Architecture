#include "Systems/Public/DynamicIterator/DynamicInspect.h"

UObject* UDynamicInspect::Next()
{
	if (IsValid(this->Inner) == false)
	{
		return nullptr;
	}
	auto* Input = this->Inner->Next();
	if (IsValid(Input))
	{
		this->Event.ExecuteIfBound(Input);
		return Input;
	}
	return nullptr;
}

int UDynamicInspect::EstimateSizeLeft() const
{
	return IsValid(this->Inner) ? this->Inner->EstimateSizeLeft() : 0;
}
