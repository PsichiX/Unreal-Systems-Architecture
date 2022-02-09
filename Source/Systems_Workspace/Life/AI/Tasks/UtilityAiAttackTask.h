#pragma once

#include "CoreMinimal.h"

#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiTask.h"

#include "UtilityAiAttackTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SYSTEMS_WORKSPACE_API UUtilityAiAttackTask : public UUtilityAiTask
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName TargetMemoryProperty = FName(TEXT("NearestActor"));

private:
	virtual bool OnEnter(AActor* Actor,
		USystemsWorld& Systems,
		FUtilityAiMemory& Memory,
		bool bChanged) override;
};
