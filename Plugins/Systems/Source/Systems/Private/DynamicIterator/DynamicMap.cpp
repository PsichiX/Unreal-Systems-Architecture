#include "Systems/Public/DynamicIterator/DynamicMap.h"

UObject* UDynamicMap::Next()
{
	if (IsValid(this->Inner) == false)
	{
		return nullptr;
	}
	auto* Input = this->Inner->Next();
	if (IsValid(Input))
	{
		auto* Output = NewObject<UDynamicMapOutput>(this);
		this->Event.ExecuteIfBound(Input, Output);
		if (IsValid(Output->Value))
		{
			return Input;
		}
	}
	return nullptr;
}

int UDynamicMap::EstimateSizeLeft() const
{
	return IsValid(this->Inner) ? this->Inner->EstimateSizeLeft() : 0;
}
