// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "MagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	BarrelMesh->SetSimulatePhysics(true);
	RootComponent = BarrelMesh;

	//RadialForce required for impulse and pull
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(BarrelMesh);
	RadialForce->SetAutoActivate(false);

	RadialForce->Radius = 750.0f;
	RadialForce->ImpulseStrength = 2500.0f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
}


void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//function works when on component hits thanks to ADD DYNAMIC
	BarrelMesh->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBarrel::Explode);

}

void AExplosiveBarrel::Explode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMagicProjectile::StaticClass()))
	{
		RadialForce->FireImpulse();
	}
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
