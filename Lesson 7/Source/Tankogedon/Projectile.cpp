// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components\StaticMeshComponent.h"
#include "DamageTaker.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SceneComp);
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
}

void AProjectile::Start()
{
	GetWorldTimerManager().SetTimer(MovementTimer, this, &AProjectile::Move, MoveRate, true, 0.0f);
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);


}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* owner = GetOwner(); //ACannon
	AActor* OwnerByOwner = owner != nullptr ? owner->GetOwner() : nullptr; //ATurret or ATankPawn
	
	if (OtherActor != owner || OtherActor != OwnerByOwner) {
		IDamageTaker* DamageActor = Cast<IDamageTaker>(OtherActor);

		if (DamageActor) {
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = owner;
			DamageData.DamageMaker = this;

			DamageActor->TakeDamage(DamageData);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s"), *OtherActor->GetName());
			OtherActor->Destroy();
		}

		Destroy();
	}
	
}


