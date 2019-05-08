// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"

// Sets default values
ATankAimingComponent::ATankAimingComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankAimingComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

