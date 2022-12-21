# Setup system

- [C++ implementation](#c)
- [Blueprint implementation](#blueprint)

# C++

- `Tutorial/Systems/TutorialMovementSystem.h`:

    ```cpp
    #pragma once

    #include "CoreMinimal.h"

    class USystemsWorld;

    UFUNCTION()
    void TUTORIAL_API TutorialMovementSystem(USystemsWorld& Systems);
    ```

- `Tutorial/Systems/TutorialMovementSystem.cpp`:

    ```cpp
    #include "Tutorial/Systems/Persistent/TutorialMovementSystem.h"

    #include "Systems/Public/SystemsWorld.h"

    #include "Tutorial/Components/TutorialMovementComponent.h"

    void TutorialMovementSystem(USystemsWorld& Systems)
    {
        const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

        Systems.Query<UTutorialMovementComponent>().ForEach(
            [&](auto& QueryItem)
            {
                auto* Actor = QueryItem.Get<0>();
                auto* Movement = QueryItem.Get<1>();
                const auto Position = Actor->GetActorLocation();
                const auto Velocity = FVector(Movement->Value, 0) * DeltaTime;

                Actor->SetActorLocation(Position + Velocity);

                Movement->Value = FVector2D(0);
            });
    }
    ```

Since this is a lambda system, we need to register it to [`class: FSystemsReflection`]().
To do that you have to register it in your game module:

- `Tutorial/Tutorial.h`:

    ```cpp
    #pragma once

    #include "CoreMinimal.h"

    #include "Modules/ModuleManager.h"

    class FTutorialModule : public IModuleInterface
    {
    public:
        virtual void StartupModule() override;

        virtual void ShutdownModule() override;
    };
    ```

- `Tutorial/Tutorial.cpp`:

    ```cpp
    #include "Tutorial.h"

    #include "Systems/Public/SystemsReflection.h"

    #include "Tutorial/Systems/Persistent/TutorialMovementSystem.h"

    #define LOCTEXT_NAMESPACE "FTutorialModule"
    #define SYSTEMS_NAMESPACE "Tutorial"

    void FTutorialModule::StartupModule()
    {
        REGISTER_SYSTEM_FUNCTION(TutorialMovementSystem);
    }

    void FTutorialModule::ShutdownModule()
    {
        UNREGISTER_SYSTEM_FUNCTION(TutorialMovementSystem);
    }

    #undef LOCTEXT_NAMESPACE
    #undef SYSTEMS_NAMESPACE

    IMPLEMENT_GAME_MODULE(FTutorialModule, Tutorial);
    ```

# Blueprint

1. Create new game object that inherits from `Object`:

    ![Create query](../../../assets/systems_query_create.png)

    And name it `BP_TutorialMovementQuery`.

1. Add `BP_TutorialMovement` component and actor proeprties to tell over what systems
    world elements this query wants to iterate over.

    ![Add query properties](../../../assets/systems_query_properties.png)

    Here Actor is `Actor` and `Movement` is `BP_TutorialMovement` component.

1. Create new game object that inherits from [`class: UScriptableSystem`]():

    ![Create system](../../../assets/systems_system_create.png)

    And name it `BP_TutorialMovementSystem`.

1. Override `On Run` event:

    ![Override OnRun event](../../../assets/systems_system_run_a.png)

1. Create `Query` node and assign `BP_TutorialMovementQuery` class to it:

    ![Add query node](../../../assets/systems_system_run_b.png)

1. Create `Lazy Iterate` node:

    ![Add lazy iterate node](../../../assets/systems_system_run_c.png)

1. Perform update on yielded object:

    ![Perform update](../../../assets/systems_system_run_d.png)

    > Note that `Movement`::`Value` is multiplied by `Delta Time` provided by
    > `On Run` event.