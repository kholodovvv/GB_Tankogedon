// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "TankFactory.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TANKOGEDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	ATankFactory();
	virtual void TakeDamage(FDamageData DamageData) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DamageTaked(float DamageValue);

	UFUNCTION()
	void Die();

	void SpawnNewTank();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* TankSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	TSubclassOf<class ATankPawn> TankSpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	float SpawnTankRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	TArray<class ATargetPoint*> TankWayPoints;

	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	const FString BuildingMeshPath = "StaticMesh'/Engine/VREditor/Teleport/TeleportDirectionMesh.TeleportDirectionMesh'";
	
	bool IsWork = true;
};
