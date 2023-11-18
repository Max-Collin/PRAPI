// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "PRAPICharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class PRAPI_API APRAPICharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APRAPICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Inputs
	 */
	
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputMappingContext> IMC_PRAPI;
	UPROPERTY(EditAnywhere,Category = Input)
		TObjectPtr<UInputAction> MovementAction;
	UPROPERTY(EditAnywhere,Category = Input)
			TObjectPtr<UInputAction> LookAction;
	
	
	void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;
    	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
