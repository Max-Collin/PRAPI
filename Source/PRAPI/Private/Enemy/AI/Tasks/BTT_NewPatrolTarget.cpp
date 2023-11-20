// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Tasks/BTT_NewPatrolTarget.h"

#include "Enemy.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UBTT_NewPatrolTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>( OwnerComp.GetOwner());
	AEnemy* Enemy = Cast<AEnemy>( EnemyAIController->GetPawn());
	
	if(Enemy)
	{
		Enemy->ChangePatrolTarget();
		
	}
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);

	
	return EBTNodeResult::Succeeded;
}
