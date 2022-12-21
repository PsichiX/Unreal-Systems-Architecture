# Setup actor

1. Create new game object that inherits from [`class: ASystemsActor`]() or `Actor`
    (if you don't need automatic built-in engine components registration based on
    `Systems` tag) - same for pawns ([`class: ASystemsPawn`]() or `Pawn`):

    ![Create actor](../../../assets/systems_actor_create.png)

    And name it `BP_Entity`.

1. Add `BP_TutorialMovement` component:

    ![Add components](../../../assets/systems_actor_components_a.png)

    ![Add components](../../../assets/systems_actor_components_b.png)

1. Assign random unit vector to `TutorialMovement`::`Value` on `BeginPlay` event:

    ![Add components](../../../assets/systems_actor_components_c.png)

    To initially pick a random direction for this actor further movement.