// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AlignFurniture.generated.h"

/**
 * 
 */
UCLASS()
class ROOMGENERATOR_API UBTTask_AlignFurniture : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	AActor* GetNearestActorWithTag(const UObject* WorldContextObject, FName Tag, AActor& NearTo);
	void RotateAwayFrom(AActor& RotateFrom, AActor& RotateObj);
	FHitResult MoveTo(AActor& MoveTo, AActor& MoveObj);
};
