// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ArrowComponent.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class ACanon;

UCLASS()
class TANKOGEDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	ATankPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ForwardMovement(float Value);
	void RightMovement(float Value);
	void Movements(float DeltaTime);
	void RotationRight(float Value);
	void Fire();
	void FireSpecial();
	void SetupCannon(TSubclassOf<ACanon> newCannonClass);
	void SwapCannon();//Метод для смены пушки (Method for changing the cannon)
	void AddBullets(float bullets); //Метод для добавления снарядов (Method for adding bullets)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	UCameraComponent* Camera;

	UPROPERTY();
	ACanon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon"); //First Cannon
	TSubclassOf<ACanon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon"); //Second Cannon
	TSubclassOf<ACanon> SecondCannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon");
	class UArrowComponent* CanonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Rotation");
	float TurretRotationInterpolationKey = 0.5f;

	UPROPERTY()
	class ATankPlayerController* TankController;

	UPROPERTY()
	TArray<TSubclassOf<ACanon>> ArrayCannonClasses; //Array for storing cannons

	float targetForwardAxisValue = 0.0f;
	float targetRightAxisValue = 0.0f;

	float targetRotationRightAxisValue = 0.0f;
};
