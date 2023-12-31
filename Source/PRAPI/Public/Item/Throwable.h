// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Throwable.generated.h"

class USphereComponent;
class UAIPerceptionStimuliSourceComponent;
/**
 * 
 */
UCLASS()
class PRAPI_API AThrowable : public AItem
{
	GENERATED_BODY()

	
public:
	AThrowable();
	void Equip(USceneComponent* InParent,FName InSocketName);
	void DetachMeshFromSocket();
protected:
	virtual void BeginPlay() override;

	UPROPERTY( VisibleAnywhere, Category=Components, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent>  AIPerceptionStimuliSourceComponent;

	void PlayHitSound(const FVector& ImpactPoint);
	
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
private:
	void AttachMeshToSocket(TObjectPtr<USceneComponent>  InParent, FName InSocketName);
	void ResetDoOnce();
	
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	bool DoOnce_Hit = false;
	
public:
	FORCEINLINE void Reset_DoOnce_Hit()  { DoOnce_Hit =true;}
	
	
};
