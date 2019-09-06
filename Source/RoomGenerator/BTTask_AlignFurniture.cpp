// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AlignFurniture.h"
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "BehaviorTree/BlackboardComponent.h"

namespace
{
	AActor* GetNearestActorWithTag(const UObject* WorldContextObject, FName Tag, AActor& NearTo)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithTag(WorldContextObject, Tag, Actors);
		AActor* Result = nullptr;
		if (Actors.Num())
		{
			Actors.Sort([&NearTo](AActor& A, AActor& B) {
				return NearTo.GetDistanceTo(&A) < NearTo.GetDistanceTo(&B);
				});
			Result = Actors[0];
		}
		return Result;
	}

	void RotateAwayFrom(AActor& RotateFrom, AActor& RotateObj)
	{
		RotateObj.SetActorRotation(RotateFrom.GetActorRotation() + RotateObj.GetActorRotation());
	}

	void RotateTowards(AActor& RotateTowards, AActor& RotateObj)
	{
		FVector RotateDirection = RotateTowards.GetActorLocation() - RotateObj.GetActorLocation();
		FRotator Rotator = FRotationMatrix::MakeFromYZ(RotateDirection, FVector(0.0, 0.0, 1.0)).Rotator();
		RotateObj.SetActorRotation(RotateObj.GetActorRotation() + Rotator);
	}

	FHitResult MoveTo(AActor& MoveTo, AActor& MoveObj)
	{
		FVector MoveToRotation = MoveTo.GetActorRotation().Quaternion().GetRightVector();
		FVector Move = (MoveTo.GetActorLocation() - MoveObj.GetActorLocation());
		Move = Move.ProjectOnTo(MoveToRotation);
		FVector NewLocation = MoveObj.GetTargetLocation() + Move;
		FHitResult HR;
		MoveObj.SetActorLocation(NewLocation, true, &HR);
		return HR;
	}
}


void UBTTask_AlignFurniture::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBActorAsset = GetBlackboardAsset();
	if (ensure(BBActorAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBActorAsset);
	}
	NodeName = "Align Furniture";
}

EBTNodeResult::Type UBTTask_AlignFurniture::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (IsValid(BC)) 
	{
		AActor* Furniture = (AActor*)BC->GetValueAsObject(BlackboardKey.SelectedKeyName);
		if (IsValid(Furniture))
		{
			if (Furniture->ActorHasTag(TEXT("Wallside")))
			{
				AActor* NearestWall = GetNearestActorWithTag(Furniture, TEXT("Wall"), *Furniture);
				if (IsValid(NearestWall))
				{
					RotateAwayFrom(*NearestWall, *Furniture);
					FVector InitLocation = Furniture->GetActorLocation();
					FHitResult HR = MoveTo(*NearestWall, *Furniture);
					if (!(HR.GetActor()->ActorHasTag("Wall") || HR.GetActor()->ActorHasTag("Floor")))
					{
						Furniture->SetActorLocation(InitLocation, true);
					}
				}
			}
			else if (Furniture->ActorHasTag(TEXT("Chair")))
			{
				AActor* NearestWall = GetNearestActorWithTag(Furniture, TEXT("Wall"), *Furniture);
				AActor* NearestTable = GetNearestActorWithTag(Furniture, TEXT("Table"), *Furniture);
				if (IsValid(NearestWall))
				{
					if (IsValid(NearestTable) && Furniture->GetDistanceTo(NearestTable) < Furniture->GetDistanceTo(NearestWall))
					{
						RotateTowards(*NearestTable, *Furniture);
					}
					else
					{
						RotateAwayFrom(*NearestWall, *Furniture);
					}
				}
				else if (IsValid(NearestTable))
				{
					RotateTowards(*NearestTable, *Furniture);
				}
			}
		}
		AActor* Floor = GetNearestActorWithTag(Furniture, TEXT("Floor"), *Furniture);
		if (IsValid(Floor))
		{
			FVector FurnitureLocation = Furniture->GetActorLocation();
			Furniture->SetActorLocation(FVector(FurnitureLocation.X, FurnitureLocation.Y, FurnitureLocation.Z -
				Floor->GetHorizontalDistanceTo(Furniture)), true);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
