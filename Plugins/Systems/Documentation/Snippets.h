void main()
{
	//// [snippet: process_systems]
	auto* Systems = NewObject<USystemsWorld>(this, USystemsWorld::StaticClass());

	// [Systems world setup...]

	Systems->Process();
	//// [/snippet]

	//// [snippet: install_lambda_system]
	Systems.InstallLambdaSystem(BoidsFaceDirectionSystem, FInstallSystemOptions("BoidsFaceDirection"));
	//// [/snippet]

	//// [snippet: install_system_options]
	Systems.InstallLambdaSystem(BoidsFaceDirectionSystem,
		FInstallSystemOptions("BoidsFaceDirection")
			.RunBefore("BoidsFaceDirection")
			.RunAfter("BoidsApplyImpulse")
			.MultiplayerRunOn(ESystemMultiplayerRunOn::ServerAndClient));
	//// [/snippet]

	//// [snippet: systems_world_seal_and_initialize]
	auto* Systems = NewObject<USystemsWorld>(this, USystemsWorld::StaticClass());
	if (IsValid(Systems) == false)
	{
		Systems->InstallResource<USomeResource>();

		Systems->InstallLambdaSystem(SomeSystem, FInstallSystemOptions("Something"));

		Systems->SealAndInitialize();
	}
	//// [/snippet]

	//// [snippet: systems_world_register_component_raw]
	Systems.RegisterComponentRaw(USomeComponent::StaticClass());
	//// [/snippet]

	//// [snippet: systems_world_register_component]
	Systems.RegisterComponent<USomeComponent>();
	//// [/snippet]

	//// [snippet: systems_world_install_resource_raw]
	Systems.InstallResourceRaw(NewObject<UInventory>(Systems, UInventory::StaticClass()));
	//// [/snippet]

	//// [snippet: systems_world_install_default_resource]
	Systems.InstallDefaultResource(UInventory::StaticClass());
	//// [/snippet]

	//// [snippet: systems_world_install_resource]
	Systems.InstallResource<UInventory>();
	//// [/snippet]

	//// [snippet: systems_world_install_system_raw]
	Systems.InstallSystemRaw(
		NewObject<USomeSystem>(Systems, USomeSystem::StaticClass()), FInstallSystemOptions("Something"));
	//// [/snippet]

	//// [snippet: systems_world_install_default_system]
	Systems.InstallDefaultSystem(USomeSystem::StaticClass(), FInstallSystemOptions("Something"));
	//// [/snippet]

	//// [snippet: systems_world_install_system]
	Systems.InstallSystem<USomeSystem>(FInstallSystemOptions("Something"));
	//// [/snippet]

	//// [snippet: systems_world_resource_raw]
	auto* Inventory = Cast<UInventory>(Systems.ResourceRaw());
	if (IsValid(Inventory))
	{
		Inventory->AddItem(FItem{FItemType::Sword});
	}
	//// [/snippet]

	//// [snippet: systems_world_resource]
	Systems.Resource<UInventory>()->AddItem(FItem{FItemType::Sword});
	//// [/snippet]

	//// [snippet: systems_world_component_raw]
	Systems.ComponentRaw(Actor, UShiaComponent::StaticClass())->JustDoIt();
	//// [/snippet]

	//// [snippet: systems_world_component]
	Systems.Component<UShiaComponent>(Actor)->JustDoIt();
	//// [/snippet]

	//// [snippet: systems_world_query]
	const auto Count = static_cast<int>(Systems.Query<UBoidComponent>().Count());
	const auto Difference = Count - EXPECTED_POPULATION_NUMBER;

	if (Difference > 0)
	{
		Systems.Query<UBoidComponent>()
			.Take(Difference)
			.ForEach(
				[](auto& QueryItem)
				{
					auto* Actor = QueryItem.Get<0>();

					Actor->Destroy();
				});
	}
	//// [/snippet]

	//// [snippet: systems_world_tagged_query]
	Systems.TaggedQuery<UVelocityComponent>().With<UBoidComponent>().Iter().ForEach(
		[](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto* Velocity = QueryItem.Get<1>();
			const auto Position = Actor->GetActorLocation();

			Actor->SetActorLocation(Position + Velocity->Value * DletaTime);
		});
	//// [/snippet]

	//// [snippet: systems_world_bad_actors_query]
	Systems.Actors().ForEach(
		[](const auto* Actor)
		{
			auto* ShiaActor = Cast<AShiaActor>(Actor);
			if (IsValid(ShiaActor))
			{
				ShiaActor->JustDoit();
			}
		});
	//// [/snippet]

	//// [snippet: systems_world_good_components_query]
	Systems.Query<UShiaComponent>().ForEach(
		[](const auto* Actor)
		{
			auto* ShiaActor = Cast<AShiaActor>(Actor);
			if (IsValid(ShiaActor))
			{
				ShiaActor->JustDoit();
			}
		});
	//// [/snippet]

	//// [snippet: systems_world_count_raw]
	// Instead of this:
	const auto A = Systems.Query<UShiaComponent>().Count();

	// You can do this:
	auto Signature = FArchetypeSignature();
	if (const auto Index = Systems.ComponentTypeIndex(UShiaComponent::StaticClass()))
	{
		Signature.EnableBit(Index.GetValue());
	}

	const auto B = Systems.CountRaw(Signature);
	//// [/snippet]

	//// [snippet: systems_world_count]
	const auto Result = Systems.Count<UShiaComponent>();
	//// [/snippet]

	//// [snippet: query_next]
	auto Query = Systems.Query<UShiaComponent>();
	while (auto& QueryItem = Query.Next())
	{
		// Note that we do not check if QueryItem optional value is set.
		// `while` loop perform these checks for us, hence we use it
		// instead of standard `for` loop.
		auto* Actor = QueryItem.GetValue().Get<0>();
		auto* Shia = QueryItem.GetValue().Get<1>();

		Shia->JustDoIt(Actor);
	}
	//// [/snippet]

	//// [snippet: iter_count]
	// 10
	const uint32 Result = IterRange(0, 10).Count();
	//// [/snippet]

	//// [snippet: iter_nth]
	// 5
	const TOptional<int> Result = IterRange(0, 10).Nth(5);
	//// [/snippet]

	//// [snippet: iter_fold]
	// 45
	const int Result = IterRange(0, 10).Fold(0, [](const auto& Accum, const auto& Value) { return Accum + Value; });
	//// [/snippet]

	//// [snippet: iter_sum]
	// 45
	const int Return = IterRange(0, 10).Sum(0);
	//// [/snippet]

	//// [snippet: iter_find]
	// 5
	const TOptional<int> Result = IterRange(0, 10).Find([](const auto& Value) { return Value >= 5; });
	//// [/snippet]

	//// [snippet: iter_find_map]
	// 5.0
	const TOptional<float> Result = IterRange(0, 10).FindMap<float>(
		[](const auto& Value)
		{
			if (Value >= 5)
			{
				return TOptional<float>(static_cast<float>(Value));
			}
			else
			{
				return TOptional<float>();
			}
		});
	//// [/snippet]

	//// [snippet: iter_first]
	// 5
	const int Result = IterRange(5, 10).First();
	//// [/snippet]

	//// [snippet: iter_last]
	// 4
	const int Result = IterRange(0, 4).Last();
	//// [/snippet]

	//// [snippet: iter_while]
	auto Iter = IterRange(0, 9);
	while (const auto Value = Iter.Next())
	{
		const auto Squared = Value * Value;
		UE_LOG(LogTemp, Warning, TEXT("Squared value: %i"), Squared);
	}
	//// [/snippet]

	//// [snippet: iter_for_each]
	IterRange(0, 9).ForEach(
		[](const auto& Value)
		{
			const auto Squared = Value * Value;
			UE_LOG(LogTemp, Warning, TEXT("Squared value: %i"), Squared);
		});
	//// [/snippet]

	//// [snippet: iter_any]
	// true
	const bool Result = IterRange(0, 10).Any([](const auto& Value) { return Value > 5; });
	//// [/snippet]

	//// [snippet: iter_all]
	// false
	const bool Result = IterRange(0, 10).All([](const auto& Value) { return Value > 5; });
	//// [/snippet]

	//// [snippet: iter_compared_by]
	// 42
	const int Result = IterRange(0, 42).ComparedBy([](const auto A, const auto B) { return A > B; });
	//// [/snippet]

	//// [snippet: iter_collect_into_array]
	TArray<int> Result();
	IterRange(0, 10).CollectIntoArray(Result);
	//// [/snippet]

	//// [snippet: iter_collect_array]
	const TArray<int> Result = IterRange(0, 10).CollectArray();
	//// [/snippet]

	//// [snippet: iter_collect_into_set]
	TSet<int> Result();
	IterRange(0, 10).CollectIntoSet(Result);
	//// [/snippet]

	//// [snippet: iter_cast]
	// [0.0, 1.0, 2.0, 3.0, 4.0]
	const TArray<float> Result = IterRange(0, 5).Cast<float>().CollectArray();
	//// [/snippet]

	//// [snippet: iter_collect_set]
	const TSet<int> Result = IterRange(0, 10).CollectSet();
	//// [/snippet]

	//// [snippet: iter_chain]
	// [0, 1, 2, 3, 4, -5, -4, -3, -2, -1]
	const TArray<int> Result = IterRange(0, 5).Chain(IterRange(-5, 0)).CollectArray();
	//// [/snippet]

	//// [snippet: iter_enumerate]
	// [0, 1, 2, 3, 4]
	const TArray<int> Result =
		IterRepeat(42).Enumerate().Map<int>([](const auto& Pair) { return Pair.Get<0>(); }).Take(5).CollectArray();
	//// [/snippet]

	//// [snippet: iter_filter]
	// [0, 2, 4, 6, 8]
	const TArray<int> Result = IterRange(0, 10).Filter([](const auto& Value) { return Value % 2 == 0; }).CollectArray();
	//// [/snippet]

	//// [snippet: iter_filter_map]
	// [0.0, 2.0, 4.0, 6.0, 8.0]
	const TArray<float> I = IterRange(0, 10)
								.FilterMap<float>(
									[](const auto& Value)
									{
										if (Value % 2 == 0)
										{
											return TOptional<float>(static_cast<float>(Value));
										}
										else
										{
											return TOptional<float>();
										}
									})
								.CollectArray();
	//// [/snippet]

	//// [snippet: iter_flatten]
	// [0, 1, 2, 3, 4, 0, 1, 2, 3, 4]
	const TArray<int> P =
		IterGenerate<TIterRange<int>>([]() { return IterRange(0, 5); }).Take(2).Flatten<int>().CollectArray();
	//// [/snippet]

	//// [snippet: iter_inspect]
	// [0, 2, 4, 6, 8]
	const TArray<int> Result = IterRange(0, 10)
								   .Inspect(
									   [](const auto& Value)
									   {
										   // Prints values: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9.
										   UE_LOG(LogTemp, Warning, TEXT("Inspected item before: %i"), Value);
									   })
								   .Filter([](const auto& Value) { return Value % 2 == 0; })
								   .Inspect(
									   [](const auto& Value)
									   {
										   // Prints values: 0, 2, 4, 6, 8.
										   UE_LOG(LogTemp, Warning, TEXT("Inspected item after: %i"), Value);
									   })
								   .CollectArray();
	//// [/snippet]

	//// [snippet: iter_map]
	// [0.0, 4.0, 16.0, 36.0, 64.0]
	const TArray<float> Result = IterRange(0, 10)
									 .Filter([](const auto& Value) { return Value % 2 == 0; })
									 .Map<float>([](const auto& Value) { return static_cast<float>(Value * Value); })
									 .CollectArray();
	//// [/snippet]

	//// [snippet: iter_skip_take]
	// [3, 4, 5, 6, 7]
	const TArray<int> Result = IterRange(0, 10).Skip(3).Take(5).CollectArray();
	//// [/snippet]

	//// [snippet: iter_views]
	// [(0, 1), (1, 2), (2, 3), (3, 4)]
	const TArray<TTuple<int, int>> Result =
		IterRange(0, 5)
			.Views<2>()
			.Map<TTuple<int, int>>([](const auto& View) { return MakeTuple(View[0], View[1]); })
			.CollectArray();
	//// [/snippet]

	//// [snippet: iter_zip]
	// [(0, -5), (1, -4), (2, -3), (3, -2), (4, -1)]
	const TArray<TTuple<int, int>> Result = IterRange(0, 5).Zip(IterRange(-5, 0)).CollectArray();
	//// [/snippet]

	//// [snippet: iter_generate]
	// [?, ?, ?, ?, ?]
	const TArray<int> Result = IterGenerate<int>([]() { return FMath::Rand(); }).Take(5).CollectArray();
	//// [/snippet]

	//// [snippet: iter_once]
	// [0, 1, 2, 3, 4, -1, 5, 6, 7, 8, 9]
	const TArray<int> Result = IterRange(0, 5).Chain(IterOnce(-1)).Chain(IterRange(5, 10)).CollectArray();
	//// [/snippet]

	//// [snippet: iter_range]
	// [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	const TArray<int> Result = IterRange(0, 10).CollectArray();
	//// [/snippet]

	//// [snippet: iter_repeat]
	// [1, 1, 1, 1, 1]
	const TArray<int> Result = IterRepeat(1).Take(5).CollectArray();
	//// [/snippet]

	//// [snippet: iter_std]
	// [0, 1, 2, 3, 4]
	TArray<int> Result = IterRange(0, 5).CollectArray();
	// [0, 2, 4, 9, 16]
	IterStd(Result).ForEach([](auto& Value) { Value = Value * Value; });
	//// [/snippet]

	//// [snippet: iter_std_const]
	// [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	const TArray<int> ResultA = IterRange(0, 10).CollectArray();
	// [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	const TArray<int> ResultB = IterStd(ResultA).CollectArray();
	//// [/snippet]

	//// [snippet: shared_channel]
	auto Sender = TSenderChannel<int>();
	auto Receiver = Sender.Receiver(1);

	Sender.Send(42);
	while (const auto Value = Receiver.Receive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Received value: %i"), Value);
	}
	//// [/snippet]

	//// [snippet: dynamic_query]
	UCLASS(BlueprintType)
	class EXAMPLE_API UShiaQueryBundle
	{
		GENERATED_BODY()

	public:
		UPROPERTY(BlueprintReadOnly)
		AActor* Actor = nullptr;

		UPROPERTY(BlueprintReadOnly)
		UShiaComponent* Shia = nullptr;
	};

	auto Query = Systems.DynamicQuery<UShiaQueryBundle>();
	auto* Bundle = NewObject<UShiaQueryBundle>(this, UShiaQueryBundle::StaticClass());
	while (Query->Next(Bundle))
	{
		Bundle->Shia->JustDoIt(Bundle->Actor);
	}
	//// [/snippet]

	//// [snippet: iter_adapt]
	// [1, 3, 5, 7, 9]
	const TArray<int> Result = IterRange(0, 10).Adapt(TIterOddAdapter<int>()).CollectArray();
	//// [/snippet]

	//// [snippet: metronome]
	auto Metronome = FMetronome{};
	Metronome.Progress(10.0);
	if (Metronome.ConsumeTicked())
	{
		UE_LOG(LogTemp, Warning, TEXT("Metronome ticked!"));
	}
	//// [/snippet]
}

//// [snippet: subsystem_game_instance]
UCLASS()
class EXAMPLE_API UExampleGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	virtual void Init() override;
};

void UExampleGameInstance::Init()
{
	Super::Init();

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(FName(),
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UShiaComponent>();

				Systems.InstallResource<UShiaSettings>();

				Systems.InstallLambdaSystem(JustDoItSystem, FInstallSystemOptions("JustDoIt"));
			});
	}
}
//// [/snippet]

//// [snippet: subsystem_game_mode]
UCLASS()
class EXAMPLE_API AExampleGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

void AExampleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(FName(),
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UShiaComponent>();

				Systems.InstallResource<UShiaSettings>();

				Systems.InstallLambdaSystem(JustDoItSystem, FInstallSystemOptions("JustDoIt"));
			});
	}
}

void AExampleGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->ReleaseSystemsWorld(ThisClass::SYSTEMS_WORLD);
	}
}
//// [/snippet]

//// [snippet: system]
UCLASS()
class EXAMPLE_API ULogBirdsNumberChangeSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;

	UPROPERTY()
	uint32 LastCount = 0;
};

void ULogBirdsNumberChangeSystem::Run(USystemsWorld& Systems)
{
	Super::Run(Systems);

	if (Systems.ComponentsDidChanged<UBirdComponent>() == false)
	{
		return;
	}

	const auto Number = static_cast<int>(Systems.Query<UBirdComponent>().Count());
	const Difference = Number - this->LastCount;
	this->LastCount = Number;

	if (Difference > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Added %i birds"), Difference);
	}
	else if (Difference < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Removed %i birds"), -Difference);
	}
}
//// [/snippet]

//// [snippet: system_init]
UCLASS()
class BOIDS_API UBoidsMovementSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Init(USystemsWorld& Systems) override;

private:
	TReceiverChannel<FMovementStep> MovementStep = {};
};

void UBoidsMovementSystem::Init(USystemsWorld& Systems)
{
	Super::Init(Systems);

	auto* GameEvents = Systems.Resource<UGameEvents>();
	if (IsValid(GameEvents))
	{
		this->MovementStep = GameEvents->MovementStep.Receiver(1);
	}
}
//// [/snippet]

//// [snippet: system_run]
UCLASS()
class BOIDS_API UBoidsMovementSystem : public USystem
{
	GENERATED_BODY()

public:
	virtual void Run(USystemsWorld& Systems) override;
};

void UBoidsMovementSystem::Run(USystemsWorld& Systems)
{
	Super::Run(Systems);

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto TimeScale = BoidsSettings->TimeScale;
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds() * TimeScale;

	Systems.Query<UVelocityComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto* Velocity = QueryItem.Get<1>();
			const auto Position = Actor->GetActorLocation();

			Actor->SetActorLocation(Position + Velocity->Value * DeltaTime);
		});
}
//// [/snippet]

//// [snippet: lambda_system]
UFUNCTION()
void BoidsFaceDirectionSystem(USystemsWorld& Systems);

void BoidsFaceDirectionSystem(USystemsWorld& Systems)
{
	Systems.Query<UVelocityComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto* Velocity = QueryItem.Get<1>();

			if (Velocity->Value.IsNearlyZero() == false)
			{
				Actor->SetActorRotation(Velocity->Value.Rotation());
			}
		});
}
//// [/snippet]

//// [snippet: systems_world_toggle_component]
void ASomeActor::ToggleTagComponent(USystemsWorld& Systems, UTagComponent* Tag)
{
	this->bTagEnabled = !this->bTagEnabled;
	if (this->bTagEnabled)
	{
		Systems.AddComponent(this, Tag);
	}
	else
	{
		Systems.RemoveComponent(this, Tag);
	}
}
//// [/snippet]

//// [snippet: query_size_hint]
template <typename I>
void IterCollectIntoArray(I&& Iterator, TArray<typename I::Item>& Result)
{
	const auto SizeHint = Iterator.SizeHint();
	const auto Capacity = SizeHint.Maximum.IsSet() ? SizeHint.Maximum.GetValue() : SizeHint.Minimum;
	Result.Reserve(Result.Num() + Capacity);
	while (auto QueryItem = Iterator.Next())
	{
		Result.Add(QueryItem.GetValue());
	}
}
//// [/snippet]

//// [snippet: iter_adapter]
template <typename T>
struct TIterOddAdapter
{
public:
	template <typename I>
	TOptional<T> Next(I& Iter)
	{
		Iter.Next();
		return Iter.Next();
	}

	template <typename I>
	IterSizeHint SizeHint(const I& Iter) const
	{
		return Iter.SizeHint();
	}
};
//// [/snippet]

//// [snippet: book_getting_started]

// Actor componetns are treated as data/tags put on actors, that user reads from
// or writes to, using system queries.
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class EXAMPLE_API UPlayerComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class EXAMPLE_API UEnemyComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};

// System are usually lambdas/functions that process chunks of systems world
// data (it can be querying actor components, managing resources or just
// processing some data - the point is we focus on processing game data, no
// matter its form).
void HuntSystem(USystemsWorld& Systems)
{
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

	// Systems Architecture adds an interface over game world that allows us to
	// treat it as database that we can query, also rather than putting logic
	// into each object, we rather perform batched data processing on a set of
	// requested components.
	Systems
		.Query<UPlayerComponent>()
		// We use powerful lazy-iterators to ease our work on batched data
		// processing. this one is one of many consumer iterators and it's job
		// is to go through all yielded iterator values and make user do
		// something useful with these, just like with regular for-in loops.
		.ForEach(
			[](auto& PlayerData)
			{
				// queries always yield tuple with actor that owns components
				// requested for query, in addition to following components.
				auto* PlayerActor = PlayerData.Get<0>();
				const auto PlayerPosition = PlayerActor->GetActorLocation();

				const auto Nearest = Systems
										 .Query<UEnemyComponent>()
										 // mapping iterator is one of the most commonly used
										 // types of iterators that transform data from one form
										 // to another, usually to get exact information needed
										 // by next iterators in chain, or to just process it and
										 // collect the result.
										 .Map<TTuple<FVector, float>>(
											 [&](const auto& EnemyData)
											 {
												 const auto* EnemyActor = EnemyData.Get<0>();
												 const auto EnemyPosition = EnemyActor->GetActorLocation();
												 const auto Distance = FVector::Distance(PlayerPosition, EnemyPosition);

												 return MakeTuple(EnemyPosition, Distance);
											 })
										 // finding the nearest enemy based on distance extracted
										 // in previous iteration step.
										 .ComparedBy(
											 [](const auto& A, const auto& B)
											 {
												 const auto DistanceA = A.Get<1>();
												 const auto DistanceB = B.Get<1>();

												 return DistanceA < DistanceB;
											 });

				if (Nearest.IsSet())
				{
					const auto TargetPosition = Nearest.GetValue().Get<0>();
					const auto Direction = (TargetPosition - PlayerPosition).GetSafeNormal();
					const auto Velocity = Direction * 100.0f * DeltaTime;

					PlayerActor->SetActorLocation(PlayerPosition + Velocity);
				}
			});
}

UCLASS()
class EXAMPLE_API UExampleGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	virtual void Init() override;
};

void UExampleGameInstance::Init()
{
	Super::Init();

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(FName(),
			[&](auto& Systems)
			{
				// Due to how queries are resolved, we need to register all
				// components we might want to query.
				Systems.RegisterComponent<UPlayerComponent>();
				Systems.RegisterComponent<UEnemyComponent>();

				// Persistent systems (ones that will run on every game tick)
				// have to be registered in systems world. There are many types
				// of systems, but commonly used ones are state-less
				// lambda/function systems. System labels
				// ("Hunt") are used for resolving systems dependency graph
				// (user can provide sets of systems that given one has to run
				// before and after).
				Systems.InstallLambdaSystem(HuntSystem, FInstallSystemOptions("Hunt"));
			});
	}
}
//// [/snippet]

//// [snippet: resources]
UCLASS(BlueprintType)
class EXAMPLE_API USettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TimeScale = 1;
};

void UExampleGameInstance::Init()
{
	Super::Init();

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(FName(),
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UVelocityComponent>();

				// USettings asset set up in editor.
				Systems.InstallResourceRaw(this->Settings);

				Systems.InstallLambdaSystem(MovementSystem, FInstallSystemOptions("Movement"));
			});
	}
}

void MovementSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<USettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}
	const auto TimeScale = Settings->TimeScale;
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds() * TimeScale;

	Systems.Query<UVelocityComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto* Velocity = QueryItem.Get<1>();
			const auto Position = Actor->GetActorLocation();

			Actor->SetActorLocation(Position + Velocity->Value * DeltaTime);
		});
}
//// [/snippet]

//// [snippet: iterator_anatomy]
#pragma once

#include "CoreMinimal.h"

// Make sure to include converters declaration header first.
#include "Systems/Public/Iterator/ConvertersDecl.h"

// Then converters, macros and size hint, order doesn't matter here.
#include "Systems/Public/Iterator/Converters.h"
#include "Systems/Public/Iterator/Macros.h"
#include "Systems/Public/Iterator/SizeHint.h"

// This iterator is generalized over type of values it yields.
template <typename T>
struct TIterRepeat
{
public:
	// We need to provide type aliases for iterators injected with macros will
	// properly understand this iterato when wrapping it internally.
	using Self = TIterRepeat<T>;
	using Item = typename T;

	TIterRepeat() : Value(TOptional<T>())
	{
	}

	TIterRepeat(T Data) : Value(TOptional<T>(Data))
	{
	}

	// Provide `Next` method that returns optional value of item type.
	TOptional<Item> Next()
	{
		return TOptional<T>(this->Value);
	}

	// Provide `Sizehint` method thar returns hint about estimated range of
	// items it can yield.
	IterSizeHint SizeHint() const
	{
		return IterSizeHint{0, TOptional<uint32>()};
	}

private:
	TOptional<T> Value = {};

public:
	// To make iterators chains we call special macro that injects all other
	// built-in iterators as this one methods.
	ITER_IMPL
};

// Provide handy construction function for ergonomics, just because of C++
// having easier times with types deduction on them.
template <typename T>
TIterRepeat<T> IterRepeat(T Value)
{
	return TIterRepeat<T>(Value);
}
//// [/snippet]

//// [snippet: stateless_system]
void MovementSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<USettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}
	const auto TimeScale = Settings->TimeScale;
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds() * TimeScale;

	Systems.Query<UVelocityComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto* Velocity = QueryItem.Get<1>();
			const auto Position = Actor->GetActorLocation();

			Actor->SetActorLocation(Position + Velocity->Value * DeltaTime);
		});
}
//// [/snippet]

//// [snippet: install_stateless_system]
void UExampleGameInstance::Init()
{
	Super::Init();

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(FName(),
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UVelocityComponent>();

				Systems.InstallLambdaSystem(MovementSystem, FInstallSystemOptions("Movement"));
			});
	}
}

//// [/snippet]

//// [snippet: install_stateful_system]
void UExampleGameInstance::Init()
{
	Super::Init();

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(FName(),
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UBirdComponent>();

				Systems.InstallSystem<ULogBirdsNumberChangeSystem>(FInstallSystemOptions("LogBirdsNumberChange"));
			});
	}
}
//// [/snippet]

//// [snippet: result]
UENUM()
enum class EGuessError : uint8
{
	TooHigh,
	TooLow,
};

USTRUCT()
struct FShiaTheSecretKeeper
{
	GENERATED_BODY()

public:
	FShiaTheSecretKeeper() : Secret(), Password()
	{
	}

	FShiaTheSecretKeeper(FString InSecret, int InPassword) : Secret(InSecret), Password(InPassword)
	{
	}

	TResult<FString, EGuessError> TryGuess(int GuessPassword)
	{
		if (GuessPassword == this->Password)
		{
			return TResult<FString, EGuessError>(this->Secret);
		}
		else if (GuessPassword < this->Password)
		{
			return TResult<FString, EGuessError>(EGuessError::TooLow);
		}
		else
		{
			return TResult<FString, EGuessError>(EGuessError::TooHigh);
		}
	}

private:
	FString Secret = {};
	int Password = {};
};

int Main()
{
	const auto Shia = FShiaTheSecretKeeper("Just do it!", 42);
	const auto Result = Shia.TryGuess(42);
	if (Result.IsOk())
	{
		UE_LOG(LogTemp, Info, TEXT("Guessed! Secret: %s"), *Result.GetOk());
	}
	else
	{
		switch (Result.GetError())
		{
			case EGuessError::TooHigh:
				UE_LOG(LogTemp, Error, TEXT("Too high!"));
				break;

			case EGuessError::TooLow:
				UE_LOG(LogTemp, Error, TEXT("Too low!"));
				break;
		}
	}
}
//// [/snippet]