
#include "PhysicsComponent.h"

/*UPhysicsComponent::UPhysicsComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}*/

TArray<FVector> UPhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue)
{
	TArray<FVector> trajectory;
	FVector GravityVector(/*0.0f, 0.0f,*/0, 0, Gravity);

	for (float time = 0; time < MaxTime; time += TimeStep) {
		FVector position = StartPos + Velocity * time + GravityVector * time * time / 2;
		if (position.Z <= MinZValue) {
			break;
		}

		trajectory.Add(position);
	}

	return trajectory;
}
