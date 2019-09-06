// Fill out your copyright notice in the Description page of Project Settings.


#include "FurnitureSetter.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFurnitureSetter::AFurnitureSetter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BedsCounter = FMath::RandRange(BedsMin, BedsMax);
	WallSidedCounter = FMath::RandRange(WallSidedMin, WallSidedMax);
	TablesCounter = FMath::RandRange(TablesMin, TablesMax);
	ChairsCounter = FMath::RandRange(ChairsMin, ChairsMax);
}

// Called when the game starts or when spawned
void AFurnitureSetter::BeginPlay()
{
	Super::BeginPlay();
	GetFurniture();
}

void AFurnitureSetter::GetFurniture()
{
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Bed"), Beds);
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("WallSide"), WallSided); // TODO: remove beds from wallside
	WallSided = WallSided.FilterByPredicate([](AActor* const Actor) {
		return !Actor->ActorHasTag("Bed");
	});
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Table"), Tables);
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Chair"), Chairs);
}

// Called every frame
void AFurnitureSetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFurnitureSetter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// TODO: add furniture coping instead of removing
AActor* AFurnitureSetter::TakeFurniture()
{
	AActor* Furniture = nullptr;
	TArray<AActor*>* FurnitureList = nullptr;
	if (Beds.Num() && BedsCounter)
	{
		FurnitureList = &Beds;
		--BedsCounter;
	}
	else if (WallSided.Num() && WallSidedCounter)
	{
		FurnitureList = &WallSided;
		--WallSidedCounter;
	}
	else if (Tables.Num() && TablesCounter)
	{
		FurnitureList = &Tables;
		--TablesCounter;
	}
	else if (Chairs.Num() && ChairsCounter)
	{
		FurnitureList = &Chairs;
		--ChairsCounter;
	}
	if (FurnitureList && FurnitureList->Num())
	{
		Furniture = (*FurnitureList)[FMath::RandRange(0, FurnitureList->Num() - 1)];
		FurnitureList->Remove(Furniture);
	}
	return Furniture;
}

void AFurnitureSetter::MoveFurniture(AActor* Furniture, FVector Location)
{
	Furniture->SetActorLocation(Location);
}

