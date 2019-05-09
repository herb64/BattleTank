// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "TankAimComponent.h"


// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;	// Tank does not need to tick
	TankAimComponent = CreateDefaultSubobject <UTankAimComponent>(FName("Aim Component"));
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}


// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// This is a Blueprint Callable - Called in BP_Tank Event Graph BeginPlay()
void ATank::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	TankAimComponent->SetBarrelReference(BarrelToSet);
}


void ATank::AimAt(FVector hitLocation)
{
	// no need to protect pointers as added in construction ???
	TankAimComponent->AimAt(hitLocation, LaunchSpeed);
}