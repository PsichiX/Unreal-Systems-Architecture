#include "Shared/Components/VelocityComponent.h"
#include "SystemsUnitTests/Public/SystemsUnitTest.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Systems/Persistent/BoidsLimitVelocitySystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBoidsLimitVelocityTest,
	"Boids.LimitVelocity",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext |
		EAutomationTestFlags::EngineFilter)

bool FBoidsLimitVelocityTest::RunTest(const FString& Parameters)
{
	return SystemsUnitTest(
		[&](auto* World)
		{
			// Setup database
			auto* Settings = AcquireMockResource<UBoidsSettings>(World,
				[](auto& Settings)
				{
					Settings.MinVelocity = 10;
					Settings.MaxVelocity = 20;
				});
			UTEST_TRUE("Settings are valid", IsValid(Settings));

			auto* Systems = AcquireMockWorld(World,
				[&](auto& Systems)
				{
					Systems.RegisterComponent<UBoidComponent>();
					Systems.RegisterComponent<UVelocityComponent>();

					Systems.InstallResourceRaw(Settings);

					Systems.InstallLambdaSystem(
						BoidsLimitVelocitySystem, FInstallSystemOptions("BoidsLimitVelocity"));
				});
			UTEST_TRUE("Systems world is valid", IsValid(Systems));

			// Prepare mock data.
			IterGenerate<FVector>([]() { return FMath::VRand(); })
				.Take(100)
				.ForEach(
					[&](const auto& Direction)
					{
						FMockActorBuilder::Make<AActor>(World)
							.AddComponent<UBoidComponent>()
							.AddComponentWithSetup<UVelocityComponent>(
								[&](auto* Velocity) { Velocity->Value = Direction; });
					});

			// Run systems.
			Systems->Process();

			// Compare results with expectations.
			const auto Status = Systems->Query<UVelocityComponent>().All(
				[&](const auto& QueryItem)
				{
					const auto* Velocity = QueryItem.Get<1>();
					const auto Length = Velocity->Value.Size();

					return Length >= Settings->MinVelocity && Length <= Settings->MaxVelocity;
				});
			UTEST_TRUE("Velocity is limited", Status);

			return true;
		});
}