// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Tasks/BTT_GetPatrolTarget.h"

#include "Enemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_GetPatrolTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>( OwnerComp.GetOwner());
	AEnemy* Enemy = Cast<AEnemy>( EnemyAIController->GetPawn());
	if(EnemyAIController&&Enemy)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsVector("PatrolTarget",Enemy->GetPatrolTarget()->GetActorLocation());
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Failed;
}
