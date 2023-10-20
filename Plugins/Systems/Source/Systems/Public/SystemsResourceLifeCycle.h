#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "SystemsResourceLifeCycle.generated.h"

class SYSTEMS_API USystemsWorld;

UINTERFACE()
class USystemsResourceLifeCycle : public UInterface
{
	GENERATED_BODY()
};

class SYSTEMS_API ISystemsResourceLifeCycle
{
	GENERATED_BODY()

public:
	virtual void OnInit(USystemsWorld& Systems)
	{
	}

	virtual void OnCleanup(USystemsWorld& Systems)
	{
	}
};
