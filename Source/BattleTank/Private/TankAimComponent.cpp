// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimComponent.h"
#include "TankBarrel.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// TODO: should the aim component tick??
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

// Called when the game starts
void UTankAimComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTankAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankAimComponent::AimAt(FVector hitLocation, float LaunchSpeed)
{
	if (!Barrel)
	{
		UE_LOG(LogTemp, Error, TEXT("No Barrel Component - Check BP_Tank SetBarrelReference call"));
		return;
	}

	// Using component location here, later go for socket location when creating
	// a socket for the projectile at the top of the barrel
	FVector barrelLocation = Barrel->GetComponentLocation();
	FVector startLocation = Barrel->GetSocketLocation(FName("Projectile"));

	/*UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at world hit location %s from %s"),
		*GetOwner()->GetName(), 
		*hitLocation.ToString(),
		*barrelLocation.ToString());*/

	FVector OutSuggestedVelocity = FVector(0.0f);			// Aim direction when normalized

	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutSuggestedVelocity,								// returned velocity length = LaunchSpeed
		startLocation,
		hitLocation,
		LaunchSpeed,
		false,
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace			// NEEDED, ELSE GETTING MANY NON RESULTS
	)) {
		FVector AimDirection = OutSuggestedVelocity.GetSafeNormal();
		//UE_LOG(LogTemp, Warning, TEXT("%s: Aiming direction %s"), *GetOwner()->GetName(), *AimDirection.ToString());
		MoveBarrelTowards(AimDirection);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s: No aiming direction could be determined"), *GetOwner()->GetName());
	}
}

void UTankAimComponent::MoveBarrelTowards(FVector AimDirection)
{
	Barrel->Elevate(0.7f);
}

