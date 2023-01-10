#include "Systems/Public/SystemsPipeline.h"

#include "Systems/Public/System.h"
#include "Systems/Public/SystemsReflection.h"
#include "Systems/Public/SystemsSubsystem.h"

void USystemsPipeline::Install(UWorld* World) const
{
	auto* Subsystem = USystemsSubsystem::Get(World);
	if (IsValid(Subsystem) == false)
	{
		return;
	}

	Subsystem->AcquireSystemsWorld(this->WorldId,
		[&](auto& Systems)
		{
			for (const auto& Pair : this->ComponentsToRegister)
			{
				if (Pair.Value.bUse == false)
				{
					continue;
				}
#if !UE_BUILD_DEVELOPMENT
				if (Pair.Value.bDevelopmentBuildOnly)
				{
					continue;
				}
#endif
				Systems.RegisterComponentRaw(Pair.Key);
			}

			for (const auto& Pair : this->TypeResourcesToInstall)
			{
				if (Pair.Value.bUse == false)
				{
					continue;
				}
#if !UE_BUILD_DEVELOPMENT
				if (Pair.Value.bDevelopmentBuildOnly)
				{
					continue;
				}
#endif
				if (Pair.Value.bUseGlobalStorage)
				{
					auto* Restored = Subsystem->Restore(Pair.Key, &Systems);
					if (IsValid(Restored))
					{
						Systems.InstallResourceRaw(Restored);
					}
					else
					{
						Systems.InstallDefaultResource(Pair.Key);
					}
				}
				else
				{
					Systems.InstallDefaultResource(Pair.Key);
				}
			}

			for (const auto& Pair : this->AssetResourcesToInstall)
			{
				if (Pair.Value.bUse == false)
				{
					continue;
				}
#if !UE_BUILD_DEVELOPMENT
				if (Pair.Value.bDevelopmentBuildOnly)
				{
					continue;
				}
#endif
				if (Pair.Value.Proxy && Pair.Value.Proxy->Type)
				{
					Systems.InstallProxyResourceRaw(Pair.Value.Proxy->Type, Pair.Key, Pair.Value.Proxy->Factory());
				}
				else
				{
					Systems.InstallResourceRaw(Pair.Key);
				}
			}

			for (const auto& Data : this->PersistentSystemsToInstall)
			{
				if (Data.bUse == false)
				{
					continue;
				}
#if !UE_BUILD_DEVELOPMENT
				if (Data.bDevelopmentBuildOnly)
				{
					continue;
				}
#endif
				auto* Instance = DuplicateObject<USystem>(Data.Template, &Systems);
				if (IsValid(Instance))
				{
					Systems.InstallSystemRaw(Instance, FInstallSystemOptions(Instance->GetFName()));
				}
			}

			for (const auto& Data : this->StartupSystemsToRun)
			{
				if (Data.bUse == false)
				{
					continue;
				}
#if !UE_BUILD_DEVELOPMENT
				if (Data.bDevelopmentBuildOnly)
				{
					continue;
				}
#endif
				auto* Instance = DuplicateObject<USystem>(Data.Template, &Systems);
				if (IsValid(Instance))
				{
					Instance->Init(Systems);
					Instance->Run(Systems);
				}
			}
		});
}

void USystemsPipeline::Uninstall(UWorld* World) const
{
	auto* Subsystem = USystemsSubsystem::Get(World);
	if (IsValid(Subsystem))
	{
		auto* Systems = Subsystem->GetSystemsWorld(this->WorldId);
		if (IsValid(Systems))
		{
			for (const auto& Pair : this->TypeResourcesToInstall)
			{
				if (Pair.Value.bUseGlobalStorage)
				{
					auto* Resource = Systems->ResourceRaw(Pair.Key);
					if (IsValid(Resource))
					{
						Subsystem->Store(Resource);
					}
				}
			}
			for (const auto& Data : this->StartupSystemsToRun)
			{
				if (Data.bUse == false)
				{
					continue;
				}
#if !UE_BUILD_DEVELOPMENT
				if (Data.bDevelopmentBuildOnly)
				{
					continue;
				}
#endif
				auto* Instance = DuplicateObject<USystem>(Data.Template, Systems);
				if (IsValid(Instance))
				{
					Instance->Init(*Systems);
					Instance->Run(*Systems);
				}
			}
		}
		Subsystem->ReleaseSystemsWorld(this->WorldId);
	}
}