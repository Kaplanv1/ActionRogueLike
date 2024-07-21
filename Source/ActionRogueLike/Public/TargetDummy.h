// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetDummy.generated.h"

class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ATargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetDummy();

	void PostInitializeComponents();

protected:


	UPROPERTY(VisibleAnywhere)
		USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);







};
