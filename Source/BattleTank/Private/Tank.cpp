// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "TankBarrel.h"
#include "TankTurret.h"


// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;	// Tank does not need to tick
	UE_LOG(LogTemp, Warning, TEXT("[%s] HFCM: Constructor TANK in C++"), *GetName())
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] HFCM: BeginPlay TANK in C++"), *GetName())
	Super::BeginPlay();		// NEEDED, otherwise Blueprint BeginPlay() not called!!!
}

// Called to bind functionality to input - strange, that intellisense often complains
// about AActor not having that member function, but ATank is a Pawn! 
// seems to be somewhat timing related... ???
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
