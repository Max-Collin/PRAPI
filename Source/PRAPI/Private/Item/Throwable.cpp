// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Throwable.h"


#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"

AThrowable::AThrowable()
{
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Perception Stimuli"));

	
	AIPerceptionStimuliSourceComponent->bAutoRegister=1;
	AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Hearing::StaticClass());

	
	
}

void AThrowable::Equip(USceneComponent* InParent,FName InSocketName)
{

	AttachMeshToSocket(InParent, InSocketName);
	ItemMesh->SetEnableGravity(false);
	ItemMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	ItemMesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	
}

void AThrowable::DetachMeshFromSocket()
{
	FDetachmentTransformRules TransformRules(EDetachmentRule::KeepWorld,true);
	ItemMesh->DetachFromComponent(TransformRules);
}


void AThrowable::AttachMeshToSocket(TObjectPtr<USceneComponent>  InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);
	ItemMesh->AttachToComponent(InParent,TransformRules,InSocketName);
}

void AThrowable::BeginPlay()
{
	Super::BeginPlay();
	ItemMesh->SetEnableGravity(true);
	ItemMesh->OnComponentHit.AddDynamic(this,&AThrowable::OnSphereHit);
	
}

void AThrowable::PlayHitSound(const FVector& ImpactPoint)
{
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void AThrowable::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	
	if(DoOnce_Hit)
	{
		UAISense_Hearing::ReportNoiseEvent(this,Hit.Location,100,this,-1);
		DoOnce_Hit = false;
		UE_LOG(LogTemp, Warning, TEXT("OnSphereHit called. HitComponent: %s"), *OtherActor->GetName());
		PlayHitSound(Hit.Location);
	}

	
}


