// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	APhysicsProjectile();

	virtual void Start() override;


protected:
	virtual void Move() override;
	//virtual void Explode() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	class UPhysicsComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	class UParticleSystemComponent* TrailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float MoveAccurency = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float TrajectoryMaxTime = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float TrajectoryTimeStep = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float TrajectorySpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	bool bShowTrajectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	FVector MoveVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	TArray<FVector> CurrentTrajectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	int32 TrajectoryCurrentIndex;

	float ExplodeRadius = 4.0f;
};
