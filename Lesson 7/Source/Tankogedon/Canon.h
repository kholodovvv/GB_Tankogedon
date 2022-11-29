// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Canon.generated.h"


UCLASS()
class TANKOGEDON_API ACanon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACanon();
	void Fire(); //Обычная стрельба снарядами
	void FireSpecial(); //Специальная стрельба (лучом)
	void FireQueue(); //Стрельба очередью
	bool IsReadyToFire(); //Готовность пушки к стрельбе
	void Reload(); //Метод перезарядки
	void ProcessAmmo(); //Обработка снарядов
	void AddAmmo(float bullets); //Добавление снарядов
	void SetCannonType(ECanonType newCannonType);
	ECanonType GetCannonType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	ECanonType CannonType = ECanonType::FireProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params");
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params");
	float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params");
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params");
	float Ammo = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params");
	int32 Queue = 3; //Количество снарядов объединённых в очередь

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects");
	class UParticleSystemComponent* ShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects");
	class UAudioComponent* AudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects");
	TSubclassOf<class UCameraShakeBase> CameraShake;

	bool bReadyToFire = true;
	FTimerHandle ReloadTimer;
	FTimerHandle IntetvalShotsTimer;
	int32 queue = Queue;
	float ammo = Ammo;
};
