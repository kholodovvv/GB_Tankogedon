// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorWithCannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Canon.h"
#include "HealthComponent.h"

// Sets default values
AActorWithCannon::AActorWithCannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CanonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CanonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &AActorWithCannon::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &AActorWithCannon::Die);
}

void AActorWithCannon::Fire()
{
	if (Cannon) {
		Cannon->Fire();
	}
}

void AActorWithCannon::FireSpecial()
{
	if (Cannon) {
		Cannon->FireSpecial();
	}
}

void AActorWithCannon::SetupCannon(TSubclassOf<ACanon> newCannonClass)
{
	if (!newCannonClass) return;

	if (Cannon) {
		Cannon->Destroy();
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACanon>(newCannonClass, spawnParams);
	Cannon->AttachToComponent(CanonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AActorWithCannon::SwapCannon()
{
	ArrayCannonClasses.Swap(0, 1);
	SetupCannon(ArrayCannonClasses[0]);
}

void AActorWithCannon::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AActorWithCannon::Die()
{
	if (Cannon) {
		Cannon->Destroy();
	}

	Destroy();
}

void AActorWithCannon::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->GetHealth());
}

void AActorWithCannon::BeginPlay()
{
	Super::BeginPlay();
	//TankController = Cast<ATankPlayerController>(GetController());

	ArrayCannonClasses.Push(CannonClass);
	ArrayCannonClasses.Push(SecondCannonClass);

	SetupCannon(ArrayCannonClasses[0]);
}

