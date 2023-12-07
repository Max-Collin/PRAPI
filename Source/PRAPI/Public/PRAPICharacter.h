// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/PickUpInterface.h"

#include "PRAPICharacter.generated.h"

class AThrowable;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class PRAPI_API APRAPICharacter : public ABaseCharacter , public IPickUpInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APRAPICharacter();
	
	UFUNCTION(BlueprintCallable)
	void Throw();

	virtual void SetOverlappingItem(AItem* Item) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Inputs
	 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	void PlayThrowAction();
	void Aim();
	void Crouch();
	
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputMappingContext> IMC_PRAPI;
		
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputAction> MovementAction;
		
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputAction> LookAction;
		
	UPROPERTY(EditAnywhere,Category = Input)
        TObjectPtr<UInputAction> JumpAction;
        
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputAction> EquipAction;
		
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputAction> ThrowAction;
		
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputAction> AimAction;
		
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputAction> CrouchAction;
	
	/**
	 * Animations 
	 */

	void PlayCrouchAnimations();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AM_Throw;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AM_Crouch;

	
	
	UPROPERTY(EditAnywhere)
	float ThrowSpeed=800;
private:

	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChildActorComponent> ThrowStartLocation;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AThrowable> EquippedThrowable;
    	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
