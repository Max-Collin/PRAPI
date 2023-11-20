// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Throwable.h"

#include "IDetailTreeNode.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"

AThrowable::AThrowable()
{
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Perception Stimuli"));

	
	AIPerceptionStimuliSourceComponent->bAutoRegister=1;
	AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Hearing::StaticClass());

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	SphereCollision->SetupAttachment(GetRootComponent());
	
}

void AThrowable::BeginPlay()
{
	Super::BeginPlay();

	//ItemMesh->OnComponentHit.AddDynamic(this,&AThrowable::OnSphereHit);
}

void AThrowable::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(DoOnce_Hit)
	{
		UAISense_Hearing::ReportNoiseEvent(this,Hit.Location,100,nullptr,1000);
		DoOnce_Hit = false;
		UE_LOG(LogTemp,Warning,TEXT("hit"))
	}

	
}


