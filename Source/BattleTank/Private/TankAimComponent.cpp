// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimComponent.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTankAimComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimComponent::AimAt(FVector hitLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at world hit location %s"), *GetOwner()->GetName(), *hitLocation.ToString());
}

