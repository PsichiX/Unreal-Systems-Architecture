# Subsystem

# Table of contents

- [Explanation](#explanation)
- [Examples](#examples)
    - [Game instance](#game-instance)
    - [Game mode](#game-mode)

# Explanation

For the sake of convenience Systems Architecture provides handy wrapper over
[`class: USystemsWorld`]() global instances as a [`class: USystemsSubsystem`]()
that is basically a registry of systems worlds accessible from any place in the code.

All user needs to do is to register and setup systems world using
[`class: USystemsSubsystem::AcquireSystemsWorld`]() when certain game phase starts
(either game, level, menu, etc) before adding/removing/querying actor components, and
unregister systems world using [`class: USystemsSubsystem::ReleaseSystemsWorld`]()
when given game phase ends.

# Examples

The most common game phases where systems might exists are game instance and game mode.
- You register systems world in game instance when there are systems are possible
to run during entire game lifetime. This is the easiest and safest option.
- You register systems world in game mode when there are systems that are possible
to run during given game level lifetime. This is useful if we for example have completely
separate pipelines set for each level or game mode.

## Game instance

Since game instance has a lifetime of entire game run, we only care here about
registering systems world to subsystem on `UGameInstance::Init`.

```snippet
subsystem_game_instance
```

## Game mode

When it comes to game mode, we should register systems world on `AGameModeBase::InitGame`
(it runs before any actor `BeginPlay` - if we register it on `BeginPlay`, then some
actors placed on level might try to register to yet non-existing systems world) and
unregister it on `AGameModeBase::EndPlay`.

```snippet
subsystem_game_mode
```
