// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"

ABlackHoleProjectile::ABlackHoleProjectile()
{

    SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

    MovementComp->InitialSpeed = 750.0f;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;
    MovementComp->ProjectileGravityScale = 0.0f;


	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(SphereComp);

}

void ABlackHoleProjectile::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_Destroy, this, &ABlackHoleProjectile::DestroyBlackHole, 5.0f, true);

}

void ABlackHoleProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    PullAndDestroy();
}



void ABlackHoleProjectile::PullAndDestroy()
{
    RadialForce->FireImpulse();
  
}

void ABlackHoleProjectile::DestroyBlackHole()
{
    Destroy();
}



