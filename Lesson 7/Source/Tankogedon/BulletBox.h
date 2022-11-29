// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBox.generated.h"

UCLASS()
class TANKOGEDON_API ABulletBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletBox();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	class UStaticMeshComponent* BulletBoxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	TSubclassOf<class ACanon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters");
	float Bullets = 1.0f;

	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

};
