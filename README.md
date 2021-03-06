# Unreal Systems Architecture
Systems Architecture plugin for Unreal Engine.

## Table of contents

- [About](#about)
- [Installation](#installation)
- [Testing demos](#testing-demos)
- [Quick showcase](#quick-showcase)
- [Plugins documentation](#plugins-documentation)

## About

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

    - [C++ Code](Source/Boids/)
    - [Content](Content/Boids/)

- Life simulation demo

    Small simulation-like demo where player is a god and watches over humans and
    animals running around doing their business.

    - [C++ Code](Source/Life/)
    - [Content](Content/Life/)

- Multiplayer demo

    Example of how to combine Systems Architecture with multiplayer essentials
    (For now with no client-side predicition and server-side reconciliation).

    - [C++ Code](Source/Multiplayer/)
    - [Content](Content/Multiplayer/)

- Tutorial

    Learning project that will be later used in documentation to teach users how
    to work with Systems Architecture.

    - [C++ Code](Source/Tutorial/)
    - [Content](Content/Tutorial/)

## Installation

For now until plugin gets accepted into Epic Marketplace, you should download
plugins ZIP files from [Releases page](https://github.com/PsichiX/Unreal-Systems-Architecture/releases)
and unpack them in `Plugins` folder of your game project, enable plugins in game
`.uproject` file and recompile game project (basically follow 3rd-party plugins
installation rules).

## Testing demos

If you want to test demo mini-games, please clone this entire repo, compile game
and open default level (`/Content/Shared/Maps/GameSelectionMap.umap`).

## Quick showcase

- Components store data:

    ```c++
    UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
    class SHARED_API UVelocityComponent : public USystemsActorComponent
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
    class BOIDS_API UBoidComponent : public USystemsActorComponent
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
