// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FurnitureSetter.generated.h"

UCLASS()
class ROOMGENERATOR_API AFurnitureSetter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFurnitureSetter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void GetFurniture();

	TArray<AActor*> Beds;
	TArray<AActor*> WallSided;
	TArray<AActor*> Tables;
	TArray<AActor*> Chairs;

	int32 BedsCounter;
	int32 WallSidedCounter;
	int32 TablesCounter;
	int32 ChairsCounter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	int32 BedsMin = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 BedsMax = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 WallSidedMin = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 WallSidedMax = 3;
	UPROPERTY(EditDefaultsOnly)
	int32 TablesMin = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 TablesMax = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 ChairsMin = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 ChairsMax = 4;

	UFUNCTION(BlueprintCallable)
	virtual AActor* TakeFurniture();

	UFUNCTION(BlueprintCallable)
	virtual void MoveFurniture(AActor* Furniture, FVector Location);
};
