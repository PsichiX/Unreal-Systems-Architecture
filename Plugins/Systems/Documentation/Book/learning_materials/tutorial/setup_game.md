# Setup game

1. Create new data asset that inherits from [`class: USystemsPipeline`]():

    ![Create data asset](../../../assets/systems_pipeline_create_data_asset.png)

    And name it `DA_TutorialPipeline`.

1. Add components:

    - C++

        ![Add C++ components](../../../assets/game_components_cpp.png)

    - Blueprint

        ![Add Blueprint components](../../../assets/game_components_blueprint.png)

1. Add persistent systems:

    - C++

        ![Add C++ systems](../../../assets/game_systems_cpp.png)

    - Blueprint

        ![Add Blueprint systems](../../../assets/game_systems_blueprint.png)

1. Create new game object that inherits from [`class: USystemsGameMode`]():

    ![Create game mode](../../../assets/systems_pipeline_game_mode_a.png)

    And name it `BP_TutorialGameMode`.

1. Assign `BP_TutorialPipeline` to `Systems Pipeline` property:

    ![Create game mode](../../../assets/systems_pipeline_game_mode_b.png)

1. Assign `BP_TutorialGameMode` to level you want this pipeline to run on:

    ![Create game mode](../../../assets/game_level_mode.png)

1. Put some of `BP_Entity` actors on level, run game in `PIE` and see systems performing!
