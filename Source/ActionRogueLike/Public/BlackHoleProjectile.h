// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "BlackHoleProjectile.generated.h"

class URadialForceComponent;
/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ABlackHoleProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties

	ABlackHoleProjectile();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		URadialForceComponent* RadialForce;

protected:

	void PullAndDestroy();

	FTimerHandle TimerHandle_Destroy;

	UFUNCTION(BlueprintCallable)
	void DestroyBlackHole();

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
