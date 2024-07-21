// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(ABSTRACT)
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

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* ImpactVFX;

	UFUNCTION()
		virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	// BlueprintNativeEvent = C++ base implementation, can be expanded in Blueprints
	// BlueprintCallable to allow child classes to trigger explosions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Explode();

	virtual void PostInitializeComponents() override;

	


};
