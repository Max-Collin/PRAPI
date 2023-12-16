// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
struct FAIStimulus;

/**
 * 
 */
UCLASS()
class PRAPI_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	public:
	AEnemyAIController();

	

	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBlackboardData> BlackboardData;

	TObjectPtr<UAIPerceptionComponent> UaiPerceptionComponent;
	TObjectPtr<UAISenseConfig_Sight> Sight;
	
	TObjectPtr<UAISenseConfig_Hearing> Hear;

	

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* SeenActor,FAIStimulus Stimulus);
	UFUNCTION()
	void OnPerceptionForgoten(AActor* SourceActor);

	


public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const {return  BlackboardComponent;}
};
