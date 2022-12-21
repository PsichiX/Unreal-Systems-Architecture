# Pipelines

# Table of contents

- [Explanation](#explanation)
  - [Components](#components)
  - [Resources](#resources)
    - [Type resources](#type-resources)
    - [Asset resources](#asset-resources)
  - [Systems](#systems)
    - [Stateful systems](#stateful-systems)
    - [Stateless systems](#stateless-systems)
    - [Blueprint systems](#blueprint-systems)

# Explanation

See [`class: USystemsPipeline`]().

Systems pipeline is a data asset that describes configuration of a single systems world:
its components, resources and systems that should be installed for new systems world.
Given pipeline is then assigned to either [`class: USystemsGameMode`]() or
[`class: USystemsGameState`]() to automatically create and destroy dedicated systems world.

Having systems pipeline defined in a data asset simplifies systems world setup and gives
more flexibility to game designers so they can balance and test special cases and new game
features without asking programmers to take their time and make changes to the setup.

![Systems Pipeline Data Asset](/assets/systems_pipeline.png)

- __`World Id`__ - ID under which this pipeline systems world will be registered globally.
  Usually there is gonna be single systems world so it's best to leave `None` here, but in
  rare cases there might be sometimes a need to for multiple systems worlds in the game at
  once so here user can specify that.

## Components

See components [architecture book page](/book/architecture/components.md).

List of all component types that are gonna be recognized by this systems world.

![Systems Pipeline Data Asset - Components](/assets/systems_pipeline_components.png)

- __`Development Build Only`__ - Ensures this component will be registered in development
builds only (and editor builds).
- __`Use`__ - Useful for quickly disabling given component from the pipeline without actually
removing it, can be used for quick dirty tests.

## Resources

See resources [architecture book page](/book/architecture/resources.md).

### Type resources

List of type resources to install.

Type resources are usually just a typical `UObject`-based
game managers, 3rd-party library wrappers, data shared runtime data cache/storage and such.

![Systems Pipeline Data Asset - Type Resources](/assets/systems_pipeline_type_resources.png)

### Asset resources

List of asset resourecs to install.

Asset resources are purely data (either baked data sources
or systems settings) that will be used by systems.

![Systems Pipeline Data Asset - Asset Resources](/assets/systems_pipeline_asset_resources.png)

- __`Development Build Only`__ - Ensures this resource will be registered in development
builds only (and editor builds).
- __`Use`__ - Useful for quickly disabling given resource from the pipeline without actually
removing it, can be used for quick dirty tests.

## Systems

See systems [architecture book page](/book/architecture/systems.md).

Here we list all of the systems that are gonna be used by this pipeline's systems world.

> Order of systems in the list represents order of their execution!

![Systems Pipeline Data Asset - Systems](/assets/systems_pipeline_systems.png)

- __`Template`__ - Template `USystem`-based object that can be configured in-place.
- __`Development Build Only`__ - Ensures this system will be registered in development
builds only (and editor builds).
- __`Use`__ - Useful for quickly disabling given system from the pipeline without actually
removing it, can be used for quick dirty tests.

Commonly used template types:

### Stateful systems

Stateful systems are ones that can have configuration properties put in them and/or can hold
internal state (for example to cache some values between next system runs, change detection
for example). User makes them by inheriting directly from [`class: USystem`]() class.

### Stateless systems

Stateless systems are basically lambdas/functions registered into [`class: FSystemsReflection`]().

![Systems Pipeline Data Asset - Lambda Systems](/assets/systems_pipeline_lambda_systems.png)

The reason for lambda systems to be registered is that Unreal doesn't come with it's own global
function reflection so we have to put these into a dedicated registry ourselves, so we can assign
them in [`class: ULambdaSystem`]()'s `Function Name` property.

Every game or plugin module that exposes its lambda systems, has to register them in module
`StartupModule` and `ShutdownModule` methods like this:

```c++
#include "Tutorial.h"

#include "Systems/Public/SystemsReflection.h"

#include "Tutorial/Systems/Persistent/TutorialGoToSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMoveTowardsTargetSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMovementSystem.h"
#include "Tutorial/Systems/Persistent/TutorialSelectActorsSystem.h"

#define LOCTEXT_NAMESPACE "FTutorialModule"
#define SYSTEMS_NAMESPACE "Tutorial"

void FTutorialModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(TutorialGoToSystem);
	REGISTER_SYSTEM_FUNCTION(TutorialMoveTowardsTargetSystem);
	REGISTER_SYSTEM_FUNCTION(TutorialMovementSystem);
	REGISTER_SYSTEM_FUNCTION(TutorialSelectActorsSystem);
}

void FTutorialModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(TutorialGoToSystem);
	UNREGISTER_SYSTEM_FUNCTION(TutorialMoveTowardsTargetSystem);
	UNREGISTER_SYSTEM_FUNCTION(TutorialMovementSystem);
	UNREGISTER_SYSTEM_FUNCTION(TutorialSelectActorsSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_GAME_MODULE(FTutorialModule, Tutorial);
```

### Blueprint systems

Blueprint systems are systems made for quick and dirty prototyping by technical designers and
content creators, they serve purely as final reference for programmers to translate them into
native C++ systems. They exists exactly for scenarios where technical designer or content creator
has an idea for a game feature they want to quickly make and test its viability, without bothering
programmers to put their hands into every aspect of it, making feature design phase much faster
and removing every back-and-forth between programmers and idea owners.

Blueprint systems are made up of two objects:
- Query blueprint with public properties that represents what components (and optionally actor) query
  expects to iterate on.
- System blueprint with `OnRun` event that performs one or more queries on systems world.

Here is example of creating blueprint systems:

1. Create new game object that inherits from `UObject`:
    
    ![Systems Pipeline Data Asset - Create new object](/assets/systems_pipeline_create_query_object.png)
    
    And name it `BP_ShakeSystemQuery`.

1. Create public properties of actor/scene component types (and optionally an actor) your system query
    wants to iterate on:

    ![Systems Pipeline Data Asset - Create query properties](/assets/systems_pipeline_query_object.png)

1. Create new game object that inherits from [`class: UScriptableSystem`]():
    
    ![Systems Pipeline Data Asset - Create new system](/assets/systems_pipeline_create_scriptable_system.png)

    And name it `BP_ShakeSystem`.

1. Create `System World Query` node and assign `BP_ShakeSystemQuery` object class:
    
    ![Systems Pipeline Data Asset - System query A](/assets/systems_pipeline_system_query_a.png)

    With this we tell tis query iterator it will yield `BP_ShakeSystemQuery` objects with requested components
    and actor assigned to its fields.

1. Create `Lazy Iterate` node:
    
    ![Systems Pipeline Data Asset - System query B](/assets/systems_pipeline_system_query_b.png)

    Since `Query` returns a lazy iterator object (you can read about lazy iterators in
    [this architecture book page](/book/architecture/iterators.md)), this node is a `For Each` equivalent,
    to iterate over this iterator elements.

1. Apply changes to actor offset component we got from yielded query object:
    
    ![Systems Pipeline Data Asset - System query C](/assets/systems_pipeline_system_query_c.png)

