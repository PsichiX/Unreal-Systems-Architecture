# Unreal Systems Architecture
Systems Architecture plugin for Unreal Engine.

This plugin adds database-like interface over Unreal Engine world that allows
you to query and process game world data in easy, ergonomic and modular way.

It is all about enabling decoupling of data from logic, allowing fast development
iteration times, making it easier to develop and reason about game logic.

> **More in-depth about Systems Architecture can be found in
[Systems Architecture documentation](https://psichix.github.io/Unreal-Systems-Architecture/systems)**

This repository contains a workspace project for small demos showcasing how to
setup and use Systems Architecture in your game:

- Boids demo

    Boids are somewhat standard way to showcase database-like approach to game
    architecture in a nutshell.

    - [C++ Code](Source/Systems_Workspace/Boids/)
    - [Content](Content/Boids/)

- Life simulation demo

    Small simulation-like demo where player is a god and watches over humans and
    animals running around doing their business.

    - [C++ Code](Source/Systems_Workspace/Life/)
    - [Content](Content/Life/)

## Installation

For now until plugin gets accepted into Epic Marketplace, you should download
plugins ZIP files from [Releases page](https://github.com/PsichiX/Unreal-Systems-Architecture/releases)
and unpack them in `Plugins` folder of your game project.

## Quick showcase

- Components store data:

    ```c++
    UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
    class SYSTEMS_WORKSPACE_API UVelocityComponent : public USystemsActorComponent
    {
        GENERATED_BODY()

    public:
        UPROPERTY()
        FVector Value = FVector(0);
    };
    ```

- There are also tag components that do not store data, rather mark actors:

    ```c++
    UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
    class SYSTEMS_WORKSPACE_API UBoidComponent : public USystemsActorComponent
    {
        GENERATED_BODY()
    };
    ```

- In system functions we can query Systems World database to process batches of world data
in more functional way, using powerful lazy-iterators:

    ```c++
    void BoidsMovementSystem(USystemsWorld& Systems)
    {
        const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

        Systems.Query<UVelocityComponent, UBoidComponent>().ForEach(
            [&](auto& QueryItem)
            {
                auto* Actor = QueryItem.Get<0>();
                const auto* Velocity = QueryItem.Get<1>();
                const auto Position = Actor->GetActorLocation() + Velocity->Value * DeltaTime;

                Actor->SetActorLocation(Position);
            });
    }
    ```

> That's all what falls into common use of Systems Architecture in code,
for more in-depth explanation and examples please visit plugin documentation.

## Plugins documentation

- [Systems Architecture](https://psichix.github.io/Unreal-Systems-Architecture/systems)
- [Systems Query Debugger]() (TODO)
- [Reactive UI Systems]() (TODO)
