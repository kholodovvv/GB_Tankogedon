
#include "TankFactory.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "HealthComponent.h"
#include "TankPawn.h"
#include "Engine\TargetPoint.h"
#include "Kismet\GameplayStatics.h"
#include "Engine/StaticMesh.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(BoxCollision);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TankSpawnPoint"));
	TankSpawnPoint->SetupAttachment(BuildingMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionProfileName("OverlapAll");
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATankFactory::OnMeshOverlapBegin);
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle spawnTimer;
	GetWorldTimerManager().SetTimer(spawnTimer, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank Factory Health: %f"), HealthComponent->GetHealth());
}

void ATankFactory::Die()
{
	//Destroy();
	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BuildingMeshPath);
	if (BodyMeshTemp) {
		BuildingMesh->SetStaticMesh(BodyMeshTemp);
	}

	IsWork = false;
}

void ATankFactory::SpawnNewTank()
{
	if (IsWork) {
		FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
		ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(TankSpawnClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		newTank->SetPatrollingPath(TankWayPoints);

		UGameplayStatics::FinishSpawningActor(newTank, newTank->GetTransform());
	}
}

void ATankFactory::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsWork) {
		ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);

		if (TankPawn) {
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("SecondLevel"));
		}
	}
	else {
		return;
	}
}