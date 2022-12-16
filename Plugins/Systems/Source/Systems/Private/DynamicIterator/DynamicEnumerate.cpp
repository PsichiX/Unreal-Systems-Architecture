#include "Systems/Public/DynamicIterator/DynamicEnumerate.h"

UObject* UDynamicEnumerate::Next()
{
	if (IsValid(this->Inner) == false)
	{
		return nullptr;
	}
	auto* Value = this->Inner->Next();
	if (IsValid(Value))
	{
		auto* Result = NewObject<UDynamicEnumerateValue>();
		Result->Index = this->Index++;
		Result->Value = Value;
		return Value;
	}
	return nullptr;
}

int UDynamicEnumerate::EstimateSizeLeft() const
{
	return IsValid(this->Inner) ? this->Inner->EstimateSizeLeft() : 0;
}
