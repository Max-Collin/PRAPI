// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"


AEnemyAIController::AEnemyAIController()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component")));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	Sight=CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Sight->SightRadius=500.f;
	Sight->LoseSightRadius=500.f;
	Sight->PeripheralVisionAngleDegrees = 60.f;
	Sight->DetectionByAffiliation.bDetectEnemies=true;
	Sight->DetectionByAffiliation.bDetectNeutrals=true;
	Sight->DetectionByAffiliation.bDetectFriendlies=true;

	Hear = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	Hear->HearingRange=1000.f;
	Hear->DetectionByAffiliation.bDetectEnemies=true;
	Hear->DetectionByAffiliation.bDetectNeutrals=true;
	Hear->DetectionByAffiliation.bDetectFriendlies=true;

	AAIController::GetPerceptionComponent()->ConfigureSense(*Sight);
	AAIController::GetPerceptionComponent()->ConfigureSense(*Hear);
	AAIController::GetPerceptionComponent()->SetDominantSense(*Sight->GetSenseImplementation());
	AAIController::GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemyAIController::OnTargetPerceptionUpdate);

	

	
}

void AEnemyAIController::OnTargetPerceptionUpdate(AActor* PerceviedActor,FAIStimulus Stimulus)
{
	
	if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		UE_LOG(LogTemp,Warning,TEXT("actor seen"))
		if(BlackboardComponent)
		{
			BlackboardComponent->SetValueAsObject("ChaseTarget",PerceviedActor);
		}
		
	}
	else if(Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		UE_LOG(LogTemp,Warning,TEXT("actor heard"))
		BlackboardComponent->SetValueAsVector("InvestigationTarget",Stimulus.StimulusLocation);
		
	}
	
	
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if(BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
	if(BlackboardComponent && BlackboardData)
	{
		InitializeBlackboard(*BlackboardComponent,*BlackboardData);
	}
	
	
	
	
}
