// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

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

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	 UCameraComponent* CameraComp;


	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* Input_LookMouse;




	virtual void BeginPlay() override;

	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
