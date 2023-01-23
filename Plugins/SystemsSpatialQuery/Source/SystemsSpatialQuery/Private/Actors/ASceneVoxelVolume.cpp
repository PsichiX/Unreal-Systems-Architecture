#include "SystemsSpatialQuery/Public/Actors/ASceneVoxelVolume.h"

#include "Systems/Public/Iterator.h"

void ASceneVoxelVolume::Regenerate()
{
	this->SceneVoxelRoot = FSceneVoxelNode::Make(GetBounds().GetBox(), this->MinSize);
	const auto* Level = GetWorld()->GetCurrentLevel();
	for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	{
		auto* Primitive = *It;
		if (IsValid(Primitive))
		{
			continue;
		}
		if (IterStdConst(this->PrimitiveComponentTagsFilter)
				.Any([&](const auto& Tag) { return Primitive->ComponentTags.Contains(Tag) == false; }))
		{
			continue;
		}
		const auto* Actor = Primitive->GetOwner();
		if (IsValid(Actor) == false || Actor->HasAnyFlags(RF_ClassDefaultObject) || Actor->IsInLevel(Level) == false)
		{
			continue;
		}
		if (IterStdConst(this->ActorTagsFilter)
				.Any([&](const auto& Tag) { return Actor->Tags.Contains(Tag) == false; }))
		{
			continue;
		}
		SceneVoxelRoot->Add(Primitive);
	}
	this->DataBase = this->SceneVoxelRoot->Store();
}

TOptional<FVector> ASceneVoxelVolume::FindClosestPointOnSurface(FVector Position) const
{
	if (this->SceneVoxelRoot.IsValid())
	{
		return this->SceneVoxelRoot->FindClosestPointOnSurface(Position);
	}
	return {};
}

void ASceneVoxelVolume::ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback,
	ESceneVoxelQueryOptions Options) const
{
	if (this->SceneVoxelRoot.IsValid())
	{
		this->SceneVoxelRoot->ForEachNode(Callback, Options);
	}
}

void ASceneVoxelVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (this->bAutoRegenerate)
	{
		Regenerate();
	}
}
void ASceneVoxelVolume::BeginPlay()
{
	Super::BeginPlay();

	this->SceneVoxelRoot = this->DataBase.Restore();
}