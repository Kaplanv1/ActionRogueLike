// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USphereComponent* SphereComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UProjectileMovementComponent* MovementComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UParticleSystemComponent* EffectComp;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
