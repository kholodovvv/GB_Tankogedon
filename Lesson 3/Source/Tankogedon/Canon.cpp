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

	if (CannonType == ECanonType::FireProjectile || Ammo >= FireRate) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "Fire projectile");
		ProcessAmmo();
	}else if(CannonType == ECanonType::FireTrace || Ammo >= FireRate) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire trace");
		ProcessAmmo();
	}

	/*bReadyToFire = false;
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, FireRate, false);*/
}

void ACanon::FireSpecial()
{
	if (!IsReadyToFire()) {
		return;
	}

	if (Ammo > 0) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Fire special");
		ProcessAmmo();
	}

	/*bReadyToFire = false;
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, FireRate, false);*/
}

bool ACanon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACanon::Reload()
{
	Ammo = 6;
	UE_LOG(LogTemp, Warning, TEXT("Ammo Reload: %f"), Ammo);
	bReadyToFire = true;
}

void ACanon::ProcessAmmo()
{
	Ammo = Ammo - FireRate;

	if (Ammo >= FireRate) {
		UE_LOG(LogTemp, Warning, TEXT("Ammo: %f"), Ammo);
	}
	else {
		bReadyToFire = false;
		//Автоматическая перезарядка
		//GetWorldTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, FireRate, false);
	}

}

// Called when the game starts or when spawned
void ACanon::BeginPlay()
{
	Super::BeginPlay();
	bReadyToFire = true;
}


