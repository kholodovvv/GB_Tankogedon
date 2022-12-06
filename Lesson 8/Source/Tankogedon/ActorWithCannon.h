// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "ActorWithCannon.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ACanon;

UCLASS()
class TANKOGEDON_API AActorWithCannon : public APawn, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorWithCannon();
	void Fire();
	void FireSpecial();
	void SetupCannon(TSubclassOf<ACanon> newCannonClass);
	void SwapCannon();//Метод для смены пушки (Method for changing the cannon)

	virtual void TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float Value);

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
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon");
	class UArrowComponent* CanonSetupPoint;

	UPROPERTY();
	ACanon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon"); //First Cannon
	TSubclassOf<ACanon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon"); //Second Cannon
	TSubclassOf<ACanon> SecondCannonClass;

	UPROPERTY()
	TArray<TSubclassOf<ACanon>> ArrayCannonClasses; //Array for storing cannons
};
