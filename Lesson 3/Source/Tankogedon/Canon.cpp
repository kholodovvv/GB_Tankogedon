// Fill out your copyright notice in the Description page of Project Settings.


#include "Canon.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"

// Sets default values
ACanon::ACanon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACanon::Fire()
{
	if (!IsReadyToFire()) {
		return;
	}

	if (CannonType == ECanonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "Fire projectile");
		ProcessAmmo();
	}else if(CannonType == ECanonType::FireTrace) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire trace");
		ProcessAmmo();
	}

}

void ACanon::FireSpecial()
{
	if (!IsReadyToFire()) {
		return;
	}
	else {

		GetWorldTimerManager().SetTimer(IntetvalShotsTimer, this, &ACanon::FireQueue, 1.0f, true); //интервал между выпуском снарядов
		ProcessAmmo();
	}

}

void ACanon::FireQueue()
{
	if (queue < 1) {
		GetWorldTimerManager().ClearTimer(IntetvalShotsTimer);
		queue = Queue;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, "Fire queue"); 
		queue -= 1;
	}
}

bool ACanon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACanon::Reload()
{
	ammo = Ammo;
	UE_LOG(LogTemp, Warning, TEXT("Ammo Reload: %f"), ammo);
	bReadyToFire = true;
}

void ACanon::ProcessAmmo()
{
	ammo = ammo - FireRate;

	if (ammo >= FireRate) {
		UE_LOG(LogTemp, Warning, TEXT("Ammo: %f"), ammo);
	}
	else {
		bReadyToFire = false;
		//Автоматическая перезарядка
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, FireRate, false);
	}

}

// Called when the game starts or when spawned
void ACanon::BeginPlay()
{
	Super::BeginPlay();
	bReadyToFire = true;
}


