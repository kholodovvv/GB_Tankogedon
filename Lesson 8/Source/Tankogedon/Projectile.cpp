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
	
	if(VolumetricExplosion)
		Explode();
	
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
			//UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s"), *OtherActor->GetName());
			//OtherActor->Destroy();
			UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (mesh) {
				if (mesh->IsSimulatingPhysics()) {
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
				}
			}
			else {
				OtherActor->Destroy();
			}
		}

		//Destroy();
	}
	
}

void AProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;
	bool sweepResult = GetWorld()->SweepMultiByChannel(AttackHit, startPos, endPos, Rotation, ECollisionChannel::ECC_Visibility, Shape, params);

	GetWorld()->DebugDrawTraceTag = "Explode Trace";

	if (sweepResult) {
		for (FHitResult hitResult : AttackHit)
		{
			AActor* otherActor = hitResult.GetActor();
			if (!otherActor) continue;

			//IDamageTaker* damageTakerActor = Cast<IDamageTaker>(otherActor);

			if (!HitResult(otherActor))
			/*{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;
				damageTakerActor->TakeDamage(damageData);
			}
			else*/ {
				/*UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(otherActor->GetRootComponent());


				if (mesh) {
					if (mesh->IsSimulatingPhysics())
					{
						FVector forceVector = otherActor->GetActorLocation() - GetActorLocation();
						forceVector.Normalize();
						//mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
						mesh->AddForce(forceVector * PushForce, NAME_None, true);
					}
				}*/
				SupplementForce(otherActor);
			}

		}
	}
}

bool AProjectile::HitResult(AActor* OtherActor)
{
	IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);

	if (damageTakerActor)
	{
		FDamageData damageData;
		damageData.DamageValue = Damage;
		damageData.Instigator = GetOwner();
		damageData.DamageMaker = this;
		damageTakerActor->TakeDamage(damageData);

		return true;
	}
	else {
		return false;
	}
}

void AProjectile::SupplementForce(AActor* OtherActor)
{
	UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());

	if (mesh) {
		if (mesh->IsSimulatingPhysics())
		{
			FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
			forceVector.Normalize();
			mesh->AddForce(forceVector * PushForce, NAME_None, true);
		}
	}
}
