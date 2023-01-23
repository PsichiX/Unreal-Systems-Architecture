#pragma once

#include "CoreMinimal.h"

#include "SystemsSpatialQuery/Public/Utils/SceneVoxelTree.h"

#include "ASceneVoxelVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSceneVoxelRegenerateEvent);

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API ASceneVoxelVolume : public AVolume
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Regenerate();

	const FSceneVoxelNode* Voxelizer() const;

	UPROPERTY(BlueprintAssignable)
	FOnSceneVoxelRegenerateEvent OnRegenerate = {};

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	bool bAutoRegenerate = false;

	UPROPERTY(EditAnywhere)
	double MinSize = 100.0;

	UPROPERTY(EditAnywhere)
	TSet<FName> ActorTagsFilter = {};

	UPROPERTY(EditAnywhere)
	TSet<FName> PrimitiveComponentTagsFilter = {};

	UPROPERTY(VisibleAnywhere)
	FSceneVoxelDataBase DataBase = {};

	TUniquePtr<FSceneVoxelNode> SceneVoxelRoot = {};
};