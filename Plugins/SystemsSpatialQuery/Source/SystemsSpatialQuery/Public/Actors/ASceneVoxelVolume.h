#pragma once

#include "CoreMinimal.h"

#include "SystemsSpatialQuery/Public/Utils/SceneVoxelTree.h"

#include "ASceneVoxelVolume.generated.h"

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API ASceneVoxelVolume : public AVolume
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Regenerate();

	TOptional<FVector> FindClosestPointOnSurface(FVector Position) const;

	void ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback,
		ESceneVoxelQueryOptions Options = ESceneVoxelQueryOptions::Any) const;

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