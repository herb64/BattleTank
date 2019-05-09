// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimComponent.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimComponent::SetBarrelReference(UStaticMeshComponent* BarrelToSet)
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
	if (!Barrel) return;

	// Using component location here, later go for socket location when creating
	// a socket for the projectile at the top of the barrel
	FVector barrelLocation = Barrel->GetComponentLocation();
	FVector startLocation = Barrel->GetSocketLocation(FName("Projectile"));

	/*UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at world hit location %s from %s"),
		*GetOwner()->GetName(), 
		*hitLocation.ToString(),
		*barrelLocation.ToString());*/

	FVector OutSuggestedVelocity = FVector(0.0f);
	FVector AimDirection = FVector(0.0f);
	TArray<AActor*> ActorsToIgnore;							// shouldn't we ignore ourselves?

	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutSuggestedVelocity,								// returned velocity length = LaunchSpeed
		startLocation,
		hitLocation,
		LaunchSpeed,
		false,
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace
		//ESuggestProjVelocityTraceOption::TraceFullPath,	// These parameters cause problems
		//FCollisionResponseParams::DefaultResponseParam,	// not returning suggestions quite
		//ActorsToIgnore,									// often. Not sure, which one exactly
		//false												// is responsible --- TODO
	))
	
	{
		AimDirection = OutSuggestedVelocity.GetSafeNormal();
	} 
	UE_LOG(LogTemp, Warning, TEXT("%s: Aim Direction: %s (%f)"), *GetOwner()->GetName(), *AimDirection.ToString(), OutSuggestedVelocity.Size());
}

