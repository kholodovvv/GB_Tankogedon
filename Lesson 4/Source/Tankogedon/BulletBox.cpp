// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBox.h"
#include "Components\StaticMeshComponent.h"
#include "Canon.h"
#include "TankPawn.h"

// Sets default values
ABulletBox::ABulletBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject< USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BulletBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletBoxMesh"));
	BulletBoxMesh->SetupAttachment(SceneComp);
	BulletBoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BulletBoxMesh->SetCollisionProfileName("OverlapAll");
	BulletBoxMesh->OnComponentBeginOverlap.AddDynamic(this, &ABulletBox::OnMeshOverlapBegin);
}

void ABulletBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
									bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);

	if (TankPawn) {
		TankPawn->AddBullets(Bullets);

		Destroy();
	}
}



