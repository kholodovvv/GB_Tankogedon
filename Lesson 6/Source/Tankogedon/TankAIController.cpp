// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
//#include "Canon.h"

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TankPawn->ForwardMovement(1.0f);
	TankPawn->RotationRight(GetRotationValue());

	Targeting();
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	if (!TankPawn) {
		return;
	}

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector PawnLocation = TankPawn->GetActorLocation();

	MovementAccurency = TankPawn->GetMovementAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPath();
	for (FVector point : points) {
		PatrollingPoints.Add(point + PawnLocation);
	}

	CurrentPatrollingIndex = 0;

	FTimerHandle TimerSwapShooting;
	GetWorldTimerManager().SetTimer(TimerSwapShooting, this, &ATankAIController::SwapShooting, TimeSwapShooting, true, TimeSwapShooting);
}

float ATankAIController::GetRotationValue()
{
	FVector CurrentPoint = PatrollingPoints[CurrentPatrollingIndex];
	FVector PawnLocation = TankPawn->GetActorLocation();
	if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccurency) {
		CurrentPatrollingIndex++;
		if (CurrentPatrollingIndex >= PatrollingPoints.Num()) {
			CurrentPatrollingIndex = 0;
		}
	}

	FVector moveDirection = CurrentPoint - PawnLocation;
	moveDirection.Normalize();
	FVector forwardDirection = TankPawn->GetActorForwardVector();
	FVector rightDirection = TankPawn->GetActorRightVector();

	//DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
	float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));

	float RotationValue = 0.0f;
	if (forwardAngle > 2.0f) {
		RotationValue = 1.0f;
	}

	if (rightAngle > 90) {
		RotationValue = -RotationValue;
	}

	return RotationValue;
}

void ATankAIController::Targeting()
{
	if (!IsPlayerSeen() || !IsPlayerRange()) {
		return;
	}

	if (CanFire()) {
		Fire();
	}
	else {
		RotateToPlayer();
	}
}

void ATankAIController::RotateToPlayer()
{
		TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
}

bool ATankAIController::IsPlayerRange()
{
	if (!PlayerPawn) {
		return false;
	}

	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool ATankAIController::CanFire()
{
	FVector TargetingDir = TankPawn->GetTurretForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	DirToPlayer.Normalize();

	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));

	return AimAngle <= Accurency;
}

void ATankAIController::Fire()
{
	TankPawn->Fire();
}

bool ATankAIController::IsPlayerSeen()
{
	FVector PlayerPos = PlayerPawn->GetActorLocation();
	FVector EyesPos = TankPawn->GetEyesPosition();

	FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	params.AddIgnoredActor(TankPawn);
	params.bReturnPhysicalMaterial = false;
	params.bTraceComplex = true;

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, params)) {
		if (HitResult.GetActor()) {
			if (HitResult.GetActor() == PlayerPawn) {
				DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Blue, false, 0.5f, 0, 5);
			}
			DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Purple, false, 0.5f, 0, 5);
			return HitResult.GetActor() == PlayerPawn;
		}
	}

	//DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Blue, false, 0.5f, 0, 5);
	return false;
}

void ATankAIController::SwapShooting()
{
	TankPawn->SwapShooting();
}
