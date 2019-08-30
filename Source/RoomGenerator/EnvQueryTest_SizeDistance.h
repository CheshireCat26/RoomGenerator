// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_SizeDistance.generated.h"

/**
 *
 */
UCLASS()
class ROOMGENERATOR_API UEnvQueryTest_SizeDistance : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	/** context */
	UPROPERTY(EditDefaultsOnly, Category = Distance)
	TSubclassOf<UEnvQueryContext> DistanceTo;

	UEnvQueryTest_SizeDistance(const FObjectInitializer& ObjectInitializer);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
