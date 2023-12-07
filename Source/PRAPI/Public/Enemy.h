// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PRAPI_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	UFUNCTION(BlueprintCallable)
	void ChangePatrolTarget();

	UFUNCTION(BlueprintCallable)
	void playAlertSound();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool PlayOnce=true;

protected:
	void BeginPlay() override;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrolTargets;


private:
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* AlertSound;

public:
	FORCEINLINE AActor* GetPatrolTarget() const {return PatrolTarget;}
};
