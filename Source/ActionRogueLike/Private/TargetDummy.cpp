// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDummy.h"
#include "SAttributeComponent.h"

// Sets default values
ATargetDummy::ATargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");


}



void ATargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Trigger when health is changed (damage/healing)
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATargetDummy::OnHealthChanged);
}

void ATargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{


	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
}


