// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    RootComponent = SphereComp;



    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetupAttachment(SphereComp);

    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;
    MovementComp->ProjectileGravityScale = 0.0f;
    MovementComp->InitialSpeed = 8000;

}

void AProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Explode();
}

// _Implementation from it being marked as BlueprintNativeEvent
void AProjectileBase::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		EffectComp->DeactivateSystem();

		MovementComp->StopMovementImmediately();
		SetActorEnableCollision(false);

		Destroy();
	}
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}