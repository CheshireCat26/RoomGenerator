// Fill out your copyright notice in the Description page of Project Settings.


#include "SizeDistanceEnvQueryTest.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

namespace 
{
	FORCEINLINE float CalcDistance2D(const FVector& PosA, const FVector& PosB)
	{
		return (PosB - PosA).Size2D();
	}

	FORCEINLINE float GetActorRadius(const AActor& Actor)
	{
		FVector Origin;
		FVector Extent;
		Actor.GetActorBounds(true, Origin, Extent);
		return Extent.Size2D();
	}
}

USizeDistanceEnvQueryTest::USizeDistanceEnvQueryTest(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DistanceTo = UEnvQueryContext_Querier::StaticClass();
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void USizeDistanceEnvQueryTest::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	// don't support context Item here, it doesn't make any sense
	TArray<AActor*> ActorContexts = {};
	TArray<FVector> LocationContexts;
	if (!(QueryInstance.PrepareContext(DistanceTo, ActorContexts) 
		|| QueryInstance.PrepareContext(DistanceTo, LocationContexts)))
	{
		return;
	}

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		float ItemActorR;
		if (ItemActor) 
		{
			ItemActorR = GetActorRadius(*ItemActor);
		}

		for (int32 ContextIndex = 0;
			ContextIndex < (ActorContexts.Num() ? ActorContexts.Num() : LocationContexts.Num()); ContextIndex++)
		{
			FVector ContextLocation;
			if (ActorContexts.Num()) {
				AActor* ActorContext = ActorContexts[ContextIndex];
				float ActorContextR = GetActorRadius(*ActorContext);
				ContextLocation = ActorContext->GetActorLocation();
				if (ItemActor)
				{
					MinThresholdValue = fmaxf(MinThresholdValue, ActorContextR + ItemActorR);
					MaxThresholdValue = fmaxf(MaxThresholdValue, ActorContextR + ItemActorR);
				}
				else
				{
					MinThresholdValue = fmaxf(MinThresholdValue, ActorContextR);
					MaxThresholdValue = fmaxf(MaxThresholdValue, ActorContextR);
				}
			}
			else
			{
				ContextLocation = LocationContexts[ContextIndex];
				if (ItemActor)
				{
					MinThresholdValue = fmaxf(MinThresholdValue, ItemActorR);
					MaxThresholdValue = fmaxf(MaxThresholdValue, ItemActorR);
				}
			}
			const float Distance = CalcDistance2D(ItemLocation, ContextLocation);
			It.SetScore(TestPurpose, FilterType, Distance, MinThresholdValue, MaxThresholdValue);
		}
	}
}

FText USizeDistanceEnvQueryTest::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("%s: to %s"),
		*Super::GetDescriptionTitle().ToString(),
		*UEnvQueryTypes::DescribeContext(DistanceTo).ToString()));
}

FText USizeDistanceEnvQueryTest::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}
