#include "SystemsSpatialQuery/Public/Actors/ASceneVoxelVolume.h"

#include "EngineUtils.h"
#include "Systems/Public/Iterator.h"

void ASceneVoxelVolume::Regenerate()
{
	Clear();
	this->SceneVoxelRoot = FSceneVoxelNode::Make(GetBounds().GetBox(), this->MinSize);
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		auto* Actor = *It;
		if (IsValid(Actor) == false || Actor == this || Actor->IsA<AVolume>() ||
			IterStdConst(this->ActorTagsFilter)
				.Any([&](const auto& Pair) { return Actor->Tags.Contains(Pair.Key) != Pair.Value; }))
		{
			continue;
		}
		for (auto* Component : Actor->GetComponents())
		{
			auto* Primitive = Cast<UPrimitiveComponent>(Component);
			if (IsValid(Primitive) == false ||
				IterStdConst(this->PrimitiveComponentTagsFilter)
					.Any([&](const auto& Pair) { return Primitive->ComponentTags.Contains(Pair.Key) != Pair.Value; }))
			{
				continue;
			}
			SceneVoxelRoot->Add(Primitive);
#if WITH_EDITORONLY_DATA
			if (SceneVoxelRoot->Add(Primitive))
			{
				this->DebugActors.Add(Actor);
				this->DebugPrimitives.Add(Primitive);
			}
#else
			SceneVoxelRoot->Add(Primitive);
#endif
		}
	}
	this->DataBase = this->SceneVoxelRoot->Store();

	if (this->OnRegenerate.IsBound())
	{
		this->OnRegenerate.Broadcast();
	}
}

void ASceneVoxelVolume::Clear()
{
	this->SceneVoxelRoot = {};
	this->DataBase.Items.Empty();
#if WITH_EDITORONLY_DATA
	this->DebugActors.Empty();
	this->DebugPrimitives.Empty();
#endif
}

const FSceneVoxelNode* ASceneVoxelVolume::Voxelizer() const
{
	return this->SceneVoxelRoot.Get();
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