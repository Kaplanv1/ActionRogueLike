// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
struct FInputActionInstance;
struct FInputActionValue;
class USpringArmComponent;
class  UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();


protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* PrimaryAttackMontage;

	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> BlackHoleClass;;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* BlackHoleAttackMontage;

	FTimerHandle TimerHandle_BlackHoleAttack;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> DashProjectileClass;;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* DashMontage;

	FTimerHandle TimerHandle_DashProjectile;

protected:

	//Components

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	 UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
		USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
		USAttributeComponent* AttributeComp;

	//Inputs

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_LookMouse;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_PrimaryInteraction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_BlackHoleAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_DashProjectile;




	virtual void BeginPlay() override;

	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);
	void PrimaryAttack();
	void PrimaryInteract();
	void PrimaryAttack_TimeElapsed();
	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();
	void DashProjectile();
	void DashProjectile_TimeElapsed();
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
