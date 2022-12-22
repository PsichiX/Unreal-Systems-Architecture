# Setup component

- [C++ implementation](#c)
- [Blueprint implementation](#blueprint)

# C++

- `Tutorial/Components/TutorialMovementComponent.h`:

    ```cpp
    #pragma once

    #include "CoreMinimal.h"

    #include "Systems/Public/SystemsComponent.h"

    #include "TutorialMovementComponent.generated.h"

    UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
    class TUTORIAL_API UTutorialMovementComponent : public USystemsActorComponent
    {
        GENERATED_BODY()

    public:
        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector Value = FVector(0);
    };
    ```

- `Tutorial/Components/TutorialMovementComponent.cpp`:

    ```cpp
    #include "Tutorial/Components/TutorialMovementComponent.h"
    ```

# Blueprint

1. Create new game object that inherits from [`class: USystemsActorComponent`]():

    ![Create component](../../../assets/systems_component_create.png)

    And name it `BP_TutorialMovement`.

1. Add `Value` property of `Vector` type:

    ![Add properties](../../../assets/systems_component_properties.png)

