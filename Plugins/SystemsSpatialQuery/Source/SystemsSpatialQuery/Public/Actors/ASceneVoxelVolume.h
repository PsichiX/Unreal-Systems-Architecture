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
	UFUNCTION(CallInEditor, Category = "Scene Voxel Volume")
	void Regenerate();

	UFUNCTION(CallInEditor, Category = "Scene Voxel Volume")
	void Clear();

	const FSceneVoxelNode* Voxelizer() const;

	UPROPERTY(BlueprintAssignable, Category = "Scene Voxel Volume")
	FOnSceneVoxelRegenerateEvent OnRegenerate = {};

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

private:
#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const override
	{
		return true;
	}

	virtual void Tick(float DeltaSeconds) override;
#endif

	UPROPERTY(EditAnywhere, Category = "Scene Voxel Volume")
	bool bAutoRegenerate = false;

	UPROPERTY(EditAnywhere, Category = "Scene Voxel Volume")
	double MinSize = 100.0;

	UPROPERTY(EditAnywhere, Category = "Scene Voxel Volume")
	TMap<FName, bool> ActorTagsFilter = {};

	UPROPERTY(EditAnywhere, Category = "Scene Voxel Volume")
	TMap<FName, bool> PrimitiveComponentTagsFilter = {};

	UPROPERTY(VisibleAnywhere, Category = "Scene Voxel Volume|Debug")
	FSceneVoxelDataBase DataBase = {};

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Scene Voxel Volume|Debug")
	TSet<TObjectPtr<AActor>> DebugActors = {};

	UPROPERTY(VisibleAnywhere, Category = "Scene Voxel Volume|Debug")
	TSet<TObjectPtr<UPrimitiveComponent>> DebugPrimitives = {};

	UPROPERTY(EditAnywhere, Category = "Scene Voxel Volume|Debug")
	bool bDrawDebugCells = false;
#endif

	TUniquePtr<FSceneVoxelNode> SceneVoxelRoot = {};
};