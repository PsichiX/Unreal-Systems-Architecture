#include "Systems/Public/DynamicIterator/DynamicFilterMap.h"

UObject* UDynamicFilterMap::Next()
{
	if (IsValid(this->Inner) == false)
	{
		return nullptr;
	}
	auto* Output = NewObject<UDynamicFilterMapOutput>(this);
	while (true)
	{
		auto* Input = this->Inner->Next();
		if (IsValid(Input) == false)
		{
			break;
		}
		Output->Value = nullptr;
		this->Event.ExecuteIfBound(Input, Output);
		if (IsValid(Output->Value))
		{
			return Output->Value;
		}
	}
	return nullptr;
}

int UDynamicFilterMap::EstimateSizeLeft() const
{
	return IsValid(this->Inner) ? this->Inner->EstimateSizeLeft() : 0;
}
