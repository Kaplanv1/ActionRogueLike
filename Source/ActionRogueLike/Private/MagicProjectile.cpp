// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMagicProjectile::AMagicProjectile() 
{

	MovementComp->InitialSpeed = 2000.0f;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;
    MovementComp->ProjectileGravityScale = 0.0f;

}


