// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotationRight", this, &ATankPlayerController::RotationRight);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", EInputEvent::IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("SwapCanon", EInputEvent::IE_Pressed, this, &ATankPlayerController::SwapCanon);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MouseDirection;
	DeprojectMousePositionToWorld(MousePos, MouseDirection);
	FVector PawnPosition = TankPawn->GetActorLocation();

	MousePos.Z = PawnPosition.Z;
	FVector dir = MousePos - PawnPosition;
	dir.Normalize();

	MousePos = PawnPosition + dir * ConstMouseDirection;

	DrawDebugLine(GetWorld(), PawnPosition, MousePos, FColor::Green, false, 0.1f, 0, 5);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float Value)
{
	if (TankPawn) {
		TankPawn->ForwardMovement(Value);
	}
}

void ATankPlayerController::MoveRight(float Value)
{
	if (TankPawn) {
		TankPawn->RightMovement(Value);
	}
}

void ATankPlayerController::RotationRight(float Value)
{
	if (TankPawn) {
		TankPawn->RotationRight(Value);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn) {
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn) {
		TankPawn->FireSpecial();
	}
}

void ATankPlayerController::SwapCanon()
{
	if (TankPawn) {
		TankPawn->SwapCannon();
	}
}
