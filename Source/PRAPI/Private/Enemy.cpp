// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
}

void AEnemy::ChangePatrolTarget()
{
	if(PatrolTargets.Num()>0)
	{
		int RandomPatrolTarget = FMath::RandRange(0,PatrolTargets.Num()-1);
		PatrolTarget = PatrolTargets[RandomPatrolTarget];
	}
	
}

void AEnemy::playAlertSound()
{
	if(AlertSound&&PlayOnce)
	{
		UGameplayStatics::PlaySoundAtLocation(GetOwner(),AlertSound,GetOwner()->GetActorLocation());

		PlayOnce = false;
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	ChangePatrolTarget();
}
