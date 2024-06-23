// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");


}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();


	//Mapping Contexti sisteme kaydettik

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem
			<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		EnhancedInputComponent->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &ASCharacter::LookMouse);
		EnhancedInputComponent->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
		EnhancedInputComponent->BindAction(Input_PrimaryInteraction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
		EnhancedInputComponent->BindAction(Input_BlackHoleAttack, ETriggerEvent::Triggered, this, &ASCharacter::BlackHoleAttack);


	}

}

void ASCharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Get value from input (combined  value from WASD Keys or single Gamepad stick)
	// and convert to vector (x,y)

	const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

	// Move Forward and Back
	AddMovementInput(ControlRot.Vector(), AxisValue.Y);

	//Move Right/Left Strafe
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.X);

}

void ASCharacter::LookMouse(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get< FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ASCharacter::PrimaryAttack()
{
	if (PrimaryAttackMontage)
	{

		PlayAnimMontage(PrimaryAttackMontage);

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	}

}

void ASCharacter::PrimaryAttack_TimeElapsed()
{

	FHitResult Hit;

	FVector Start = CameraComp->GetComponentLocation();

	FVector End = Start + (CameraComp->GetComponentRotation().Vector() * 10000);

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, QueryParams);

	FVector MuzzleLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM;

	if (Hit.bBlockingHit)
	{
		FRotator SpawnRot = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, Hit.ImpactPoint);
		SpawnTM = FTransform(SpawnRot, MuzzleLocation);
	}
	else
	{
		SpawnTM = FTransform(GetControlRotation(), MuzzleLocation);

	}

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);



}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}

}

void ASCharacter::BlackHoleAttack()
{
	FTransform SpawnTM = FTransform(GetControlRotation(), GetActorLocation());

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(BlackHoleClass, SpawnTM, SpawnParams);
}
