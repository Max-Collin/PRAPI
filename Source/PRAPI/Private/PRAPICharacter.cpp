// Fill out your copyright notice in the Description page of Project Settings.


#include "PRAPICharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/Throwable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APRAPICharacter::APRAPICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate=FRotator(0.f,360.,0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	ThrowStartLocation = CreateDefaultSubobject<UChildActorComponent>(TEXT("ThrowStartLocation"));
	ThrowStartLocation->SetupAttachment(GetRootComponent());

}

void APRAPICharacter::SetOverlappingItem(AItem* Item)
{
	IPickUpInterface::SetOverlappingItem(Item);
	OverlappingItem= Item;
}

// Called when the game starts or when spawned
void APRAPICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_PRAPI,0);
		}
	}
	
}

void APRAPICharacter::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection,MovementVector.Y);
	
	const FVector RightDirection =FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection,MovementVector.X);
}

void APRAPICharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void APRAPICharacter::Equip()
{
	
	if(OverlappingItem)
	{
		AThrowable* OverlappingThrowable = Cast<AThrowable>(OverlappingItem);
		if(OverlappingThrowable)
		{
			OverlappingThrowable->Equip(GetMesh(),FName("RightHandSocket"));
			EquippedThrowable = OverlappingThrowable;
		}
		
		
		
		
		
	}

	
}

void APRAPICharacter::PlayThrowAction()
{
	if(EquippedThrowable&&AM_Throw)
	{
		PlayAnimMontage(AM_Throw);
	}
	
	
}

void APRAPICharacter::Aim()
{
	FPredictProjectilePathParams ProjectilePathParams;
	FPredictProjectilePathResult ProjectilePathResult;


	ProjectilePathParams.StartLocation = ThrowStartLocation->GetComponentLocation();
	FVector UnitDirection =UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(),ThrowStartLocation->GetComponentLocation());
	
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	FVector LaunchVelocity = UnitDirection* ThrowSpeed;
	ProjectilePathParams.LaunchVelocity = LaunchVelocity;
	ProjectilePathParams.SimFrequency = 15;
	ProjectilePathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	ProjectilePathParams.bTraceWithCollision =true;
	
	UGameplayStatics::PredictProjectilePath(this,ProjectilePathParams,ProjectilePathResult);
	UE_LOG(LogTemp,Warning,TEXT("Aim"));

	//FVector(ForwardVector.X,ForwardVector.Y,ForwardVector.Z=50)
}

void APRAPICharacter::Throw()
{
		EquippedThrowable->Reset_DoOnce_Hit();
		EquippedThrowable->DetachMeshFromSocket();
		EquippedThrowable->GetMesh()->SetEnableGravity(true);
		const FVector UnitDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(),EquippedThrowable->GetActorLocation());
		FVector LaunchVelocity = UnitDirection* ThrowSpeed;
		EquippedThrowable->GetMesh()->SetPhysicsLinearVelocity(LaunchVelocity);
		EquippedThrowable->GetMesh()->SetEnableGravity(true);
		EquippedThrowable = nullptr;
	
	


	
}


// Called every frame
void APRAPICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APRAPICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction,ETriggerEvent::Triggered,this,&APRAPICharacter::Move);

		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&APRAPICharacter::Look);
		EnhancedInputComponent->BindAction(EquipAction,ETriggerEvent::Started,this,&APRAPICharacter::Equip);
		EnhancedInputComponent->BindAction(ThrowAction,ETriggerEvent::Started,this,&APRAPICharacter::PlayThrowAction);
		EnhancedInputComponent->BindAction(AimAction,ETriggerEvent::Triggered,this,&APRAPICharacter::Aim);
	}
	
}

