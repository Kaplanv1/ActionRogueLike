// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"



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

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	

	


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
		EnhancedInputComponent->BindAction(Input_DashProjectile, ETriggerEvent::Triggered, this, &ASCharacter::DashProjectile);


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
	if (ensure(PrimaryAttackMontage))
	{

		PlayAnimMontage(PrimaryAttackMontage);

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	}

}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
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
	if(BlackHoleAttackMontage)
	{
		PlayAnimMontage(BlackHoleAttackMontage);

		GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.24f);

	}
	
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{

	SpawnProjectile(BlackHoleClass);
}

void ASCharacter::DashProjectile()
{
	if (DashMontage)
	{
		PlayAnimMontage(DashMontage);	

		GetWorldTimerManager().SetTimer(TimerHandle_DashProjectile, this, &ASCharacter::DashProjectile_TimeElapsed, 0.2f);

	}

}

void ASCharacter::DashProjectile_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}


void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FHitResult Hit;
		FVector TraceStart = CameraComp->GetComponentLocation();
		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetInstigator());

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		FRotator ProjRotation;
		// true if we got to a blocking hit (Alternative: SweepSingleByChannel with ECC_WorldDynamic)
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Adjust location to end up at crosshair look-at
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		}
		else
		{
			// Fall-back since we failed to find any blocking hit
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}


		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}