// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDON_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class ATankPawn* TankPawn;

	UPROPERTY()
	class APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (MakeEditWidget = true)); //edit
	TArray<FVector> PatrollingPoints;

	UPROPERTY()
	float MovementAccurency = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting");
	float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting");
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting");
	float Accurency = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	float TimeSwapShooting = 5.0f;

	int32 CurrentPatrollingIndex = 0;

	float GetRotationValue();
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerRange();
	bool CanFire();
	void Fire();
	bool IsPlayerSeen();
	void Initialize();
	void SwapShooting();
};
