// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "GameFramework/Actor.h"

#include "MagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;


UCLASS()
class ACTIONROGUELIKE_API AMagicProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

	AMagicProjectile();

};
