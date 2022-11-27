// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject< USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(SceneComp);
	WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallMesh->SetCollisionProfileName("OverlapAll");
}


