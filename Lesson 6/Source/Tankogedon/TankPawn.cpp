// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Canon.h"
#include "HealthComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CanonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CanonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Перемещение танка вперед, назад, влево, вправо
	ATankPawn::Movements(DeltaTime);

	//Поворот танка
	float yawRotation = RotationSpeed * targetRotationRightAxisValue * DeltaTime;
	float LerpRotateValue = FMath::Lerp(targetRotationRightAxisValue, LerpRotateValue, TurretRotationInterpolationKey);
	FRotator currentRotation = GetActorRotation();

	yawRotation += currentRotation.Yaw;

	FRotator newRotation = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotation);

	//Поворот турели танка
	if (TankController) {
		FVector mousePos = TankController->GetMousePosition();
		RotateTurretTo(mousePos);
	}
}

void ATankPawn::ForwardMovement(float Value)
{
	targetForwardAxisValue = Value;
}

void ATankPawn::RightMovement(float Value)
{
	targetRightAxisValue = Value;
}

void ATankPawn::Movements(float DeltaTime)
{
	FVector currentPosition = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();

	FVector forwardMovement = forwardVector * MoveSpeed * targetForwardAxisValue * DeltaTime;
	FVector rightMovement = rightVector * MoveSpeed * targetRightAxisValue * DeltaTime;
	FVector movePosition = currentPosition + forwardMovement + rightMovement;

	SetActorLocation(movePosition, true);
}

void ATankPawn::RotationRight(float Value)
{
	targetRotationRightAxisValue = Value;
}

void ATankPawn::Fire()
{
	if (Cannon) {
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon) {
		Cannon->FireSpecial();
	}
}

void ATankPawn::SetupCannon(TSubclassOf<ACanon> newCannonClass)
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

void ATankPawn::SwapCannon()
{
	ArrayCannonClasses.Swap(0, 1);
	SetupCannon(ArrayCannonClasses[0]);
}

void ATankPawn::AddBullets(float bullets)
{
	if (Cannon) {
		Cannon->AddAmmo(bullets);
	}
}

FVector ATankPawn::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = TurretRotation.Pitch;
	targetRotation.Roll = TurretRotation.Roll;

	FRotator newTurretRotation = FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey);

	TurretMesh->SetWorldRotation(newTurretRotation);
}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankPawn::Die()
{
	if (Cannon) {
		Cannon->Destroy();
	}

	Destroy();
}

void ATankPawn::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->GetHealth());
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());

	ArrayCannonClasses.Push(CannonClass);
	ArrayCannonClasses.Push(SecondCannonClass);

	SetupCannon(ArrayCannonClasses[0]);
}

void ATankPawn::SwapShooting()
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
