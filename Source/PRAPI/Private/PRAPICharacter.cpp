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
	if(!EquippedThrowable)return;
	FPredictProjectilePathParams ProjectilePathParams;
	FPredictProjectilePathResult ProjectilePathResult;


	ProjectilePathParams.StartLocation = ThrowStartLocation->GetComponentLocation();
	
	float PlayerPitch ;
	FVector LaunchVelocity;
	if(GetControlRotation().Pitch>=270)
	{
		PlayerPitch = (GetControlRotation().Pitch-270)/110;
		//FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
		//LaunchVelocity = ForwardVector* ThrowSpeed *PlayerPitch +10 ;
	}
	else
	{
		PlayerPitch = (GetControlRotation().Pitch+90)/110;
		
	
	}
	PlayerPitch = FMath::Clamp(PlayerPitch,0.01,1.2);
	FVector UnitDirection =UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(),ThrowStartLocation->GetComponentLocation());
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	LaunchVelocity = UnitDirection* ThrowSpeed ;
	float PitchvelocityX =  PlayerPitch*2-FMath::Cube(PlayerPitch);
	LaunchVelocity = FVector(LaunchVelocity.X*PitchvelocityX,LaunchVelocity.Y*PitchvelocityX,FMath::Pow(LaunchVelocity.Z,PlayerPitch));
	
	
	ProjectilePathParams.LaunchVelocity = LaunchVelocity;
	ProjectilePathParams.SimFrequency = 30;
	ProjectilePathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	ProjectilePathParams.bTraceWithCollision =true;
	
	UGameplayStatics::PredictProjectilePath(this,ProjectilePathParams,ProjectilePathResult);
	UE_LOG(LogTemp,Warning,TEXT("Aim"));

	
FString String = FString::Printf(TEXT("%f"),GetControlRotation().Pitch);
	FString String2 = FString::Printf(TEXT("%f"),PlayerPitch);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1,0.1f,FColor::Red,String);
		GEngine->AddOnScreenDebugMessage(2,0.1f,FColor::Blue,String2);
	}

	//FVector(ForwardVector.X,ForwardVector.Y,ForwardVector.Z=50)
}

void APRAPICharacter::Crouch()
{
	PlayCrouchAnimations();
	if(GetCharacterMovement()->bWantsToCrouch==true)
	{
		GetCharacterMovement()->bWantsToCrouch=false;
	}
	else
	{
		
		GetCharacterMovement()->bWantsToCrouch=true;
	}
	

	
}

void APRAPICharacter::PlayCrouchAnimations()
{
	if(GetCharacterMovement()->bWantsToCrouch==true)
	{
		PlayAnimMontage(AM_Crouch,1,FName("ToStanding"));
	}
	else
	{
		PlayAnimMontage(AM_Crouch,1,FName("ToCrouch"));
	}
}

void APRAPICharacter::Throw()
{
	if(!EquippedThrowable)return;
	EquippedThrowable->GetMesh()->SetSimulatePhysics(true);
	EquippedThrowable->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EquippedThrowable->Reset_DoOnce_Hit();
	EquippedThrowable->DetachMeshFromSocket();
	
	float PlayerPitch ;
	FVector LaunchVelocity;
	if(GetControlRotation().Pitch>=270)
	{
		PlayerPitch = (GetControlRotation().Pitch-270)/110;
		//FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
		//LaunchVelocity = ForwardVector* ThrowSpeed *PlayerPitch +10 ;
	}
	else
	{
		PlayerPitch = (GetControlRotation().Pitch+90)/110;
		
	
	}
	PlayerPitch = FMath::Clamp(PlayerPitch,0.01,1.2);
	FVector UnitDirection =UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(),ThrowStartLocation->GetComponentLocation());
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	LaunchVelocity = UnitDirection* ThrowSpeed ;
	float PitchvelocityX =  PlayerPitch*2-FMath::Cube(PlayerPitch);
	LaunchVelocity = FVector(LaunchVelocity.X*PitchvelocityX,LaunchVelocity.Y*PitchvelocityX,FMath::Pow(LaunchVelocity.Z,PlayerPitch));
	
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
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started ,this,&ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed ,this,&ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(CrouchAction,ETriggerEvent::Started ,this,&APRAPICharacter::Crouch);
	}
	
}

