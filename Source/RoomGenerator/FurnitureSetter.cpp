// Fill out your copyright notice in the Description page of Project Settings.


#include "FurnitureSetter.h"
#include "Engine\LevelStreamingDynamic.h"

// Sets default values
AFurnitureSetter::AFurnitureSetter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ULevelStreamingDynamic* StreamingLevel = NewObject<ULevelStreamingDynamic>((UObject*)GetTransientPackage(), ULevelStreamingDynamic::StaticClass());

	// Add delegates
	StreamingLevel->OnLevelLoaded.AddDynamic(this, &AFurnitureSetter::OnLevelLoaded);

	StreamingLevel->bShouldBlockOnLoad = false;
	StreamingLevel->bInitiallyLoaded = true;
	StreamingLevel->bInitiallyVisible = true;

	GetWorld()->AddStreamingLevel(StreamingLevel);
}

// Called when the game starts or when spawned
void AFurnitureSetter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFurnitureSetter::OnLevelLoaded()
{
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

