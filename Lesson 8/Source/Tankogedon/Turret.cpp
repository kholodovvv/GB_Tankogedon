// Fill out your copyright notice in the Description page of Project Settings.
#include "Turret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Canon.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealthComponent.h"
#include "TankPawn.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	RootComponent = HitCollider;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(HitCollider);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp) {
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp) {
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATurret::Die);
}

void ATurret::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATurret::Die()
{
	Destroy();
}

void ATurret::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->GetHealth());
}

FVector ATurret::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(CannonClass);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimer;
	GetWorldTimerManager().SetTimer(TargetingTimer, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);

	FTimerHandle TimerSwapShooting;
	GetWorldTimerManager().SetTimer(TimerSwapShooting, this, &ATurret::SwapShooting, TimeSwapShooting, true, TimeSwapShooting);
}

void ATurret::Destroyed()
{
	if (Cannon) {
		Cannon->Destroy();
	}
}

void ATurret::SetupCannon(TSubclassOf<ACanon> newCannonClass)
{
	if (!newCannonClass) {
		return;
	}

	if (Cannon) {
		Cannon->Destroy();
	}

	FActorSpawnParameters params;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACanon>(newCannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATurret::Targeting()
{
	if (!IsPlayerSeen() || !IsPlayerInRange()) {
		return;
	}

	if (CanFire()) {
		Fire();
	}
	else {
		RotateToPlayer();
	}
}

void ATurret::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();

	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
}

void ATurret::Fire()
{
	if (Cannon) {
		Cannon->Fire();
	}
}

bool ATurret::IsPlayerInRange()
{
	if (!PlayerPawn) {
		return false;
	}

	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	FVector targetDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();//TankPawn->GetActorLocation();
	DirToPlayer.Normalize();

	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetDir, DirToPlayer)));

	return aimAngle <= Accurency;
}

bool ATurret::IsPlayerSeen()
{
	FVector PlayerPos = PlayerPawn->GetActorLocation();
	FVector EyesPos = GetEyesPosition();

	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(this);
	params.bReturnPhysicalMaterial = false;
	params.bTraceComplex = true;

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, params)) {
		if (HitResult.GetActor()) {
			if (HitResult.GetActor() == PlayerPawn) {
				DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Blue, false, 0.5f, 0, 5);
			}
			DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Purple, false, 0.5f, 0, 5);
			return HitResult.GetActor() == PlayerPawn;
		}
	}

	return false;
}

void ATurret::SwapShooting()
{
	if (Cannon) {
		if (Cannon->GetCannonType() == ECanonType::FireProjectile) {
			Cannon->SetCannonType(ECanonType::FireTrace);
		}
		else {
			Cannon->SetCannonType(ECanonType::FireProjectile);
		}
	}
}
