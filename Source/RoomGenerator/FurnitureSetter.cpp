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

AActor* AFurnitureSetter::TakeFurniture()
{
	AActor* Furniture = nullptr;
	if (Beds.Num() && BedsCounter)
	{
		Furniture = Beds[FMath::RandRange(0, Beds.Num()-1)];
		--BedsCounter;
	}
	else if (WallSided.Num() && WallSidedCounter)
	{
		Furniture = WallSided[FMath::RandRange(0, WallSided.Num()-1)];
		--WallSidedCounter;
	}
	else if (Tables.Num() && TablesCounter)
	{
		Furniture = Tables[FMath::RandRange(0, Tables.Num()-1)];
		--TablesCounter;
	}
	else if (Chairs.Num() && ChairsCounter)
	{
		Furniture = Chairs[FMath::RandRange(0, Chairs.Num()-1)];
		--ChairsCounter;
	}
	return Furniture;
}

void AFurnitureSetter::MoveFurniture(AActor* Furniture, FVector Location)
{
	Furniture->SetActorLocation(Location);
}

