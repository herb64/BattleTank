// Copyright Herbert Mehlhose, 2019
// Note: as this is for learning purpose, there's quite some commented code inside from experiments


#include "TankMovementComponent.h"
#include "TankTrack.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>

/// Blueprint based setting
void UTankMovementComponent::InitializeFromWithinBlueprint(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	PrimaryComponentTick.bCanEverTick = true;

	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	UE_LOG(LogTemp, Warning, TEXT("%s: Movement Component InitializeFromWithinBlueprint Left: %s, Right: %s"), *GetOwner()->GetName(), *LeftTrack->GetName(), *RightTrack->GetName());
}

void UTankMovementComponent::ToggleCorrectSidewaysMovement(void)
{
	sidewaysCorrectionActive = !sidewaysCorrectionActive;
	GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Red, FString::Printf(TEXT("Sideways Correction: %s"), (sidewaysCorrectionActive ? TEXT("ON") : TEXT("OFF"))));
	UE_LOG(LogTemp, Warning, TEXT("Sideways: "));
}

void UTankMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	// so that it ticks in BP as well, not needed in our case, but...
	
	if (sidewaysCorrectionActive) {
		// Apply corrective force to whole tank to compensate sideways movement
		auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

		auto RightSpeed = FVector::DotProduct(TankRoot->GetRightVector(), TankRoot->GetComponentVelocity());
		auto Acceleration = -RightSpeed / DeltaTime * TankRoot->GetRightVector();
		// Force = m * a
		auto Force = TankRoot->GetMass() * Acceleration;
		GEngine->AddOnScreenDebugMessage(4, 2.0f, FColor::Blue, FString::Printf(TEXT("RightSpeed: %f, Correction: %s"), RightSpeed, *Force.ToString()));
		TankRoot->AddForce(Force);	// Adding force at the tank location itself
	}
}

/// E X P E R I M E N T A L   C O D E   O N L Y  --  N O T   U S E D
void UTankMovementComponent::InitializeViaCpp()
{
	// Set Track variables
	LeftTrack = Cast<UTankTrack>(GetOwner()->GetDefaultSubobjectByName(FName("Track_L")));
	RightTrack = Cast<UTankTrack>(GetOwner()->GetDefaultSubobjectByName(FName("Track_R")));
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	UE_LOG(LogTemp, Warning, TEXT("%s: InitializeViaCpp Left: %s, Right: %s"), *GetOwner()->GetName(), *LeftTrack->GetName(), *RightTrack->GetName());

	// E X P E R I M E N T A L   C O D E   H E R E - iterating components for fun
	// This will only return the first object
	//UTankTrack* pTrack = Cast<UTankTrack>(GetOwner()->GetComponentByClass(TSubclassOf<UTankTrack>()));
	
	// This one does not work - check again
	/*TArray<UActorComponent*> Tracks = GetOwner()->GetComponentsByClass(TSubclassOf<UTankTrack>());
	for (UActorComponent* Track : Tracks)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found track: %s"), *Track->GetName());
	}*/

	// GetComponents() returning a TSet works fine listing all components
	//TSet<UActorComponent*> Tracks = GetOwner()->GetComponents();
	//for (UActorComponent* Track : Tracks) UE_LOG(LogTemp, Warning, TEXT("Found track: %s"), *Track->GetName());
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

}

void UTankMovementComponent::IntendMoveBackward(float Throw)
{
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	LeftTrack->SetThrottle(-Throw);
	RightTrack->SetThrottle(-Throw);
}

/// TODO: my tracks are exchanged  L <----> R :(

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::IntendTurnLeft(float Throw)
{
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	LeftTrack->SetThrottle(-Throw);
	RightTrack->SetThrottle(Throw);
}


/**
 * Overridden function from UNavMovementComponent. We just use this to get the Velocity direction
 * for the requested movement.
 */
void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	// NO call to Super::RequestDirectMove - we completely replace functionality!

	auto IntendedMoveDirection = MoveVelocity.GetSafeNormal();
	auto CurrentLocalDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	float ForwardThrow = FVector::DotProduct(CurrentLocalDirection, IntendedMoveDirection);

	FVector RightThrowVector = FVector::CrossProduct(CurrentLocalDirection, IntendedMoveDirection);
	float RightThrow = RightThrowVector.Z;

	IntendMoveForward(ForwardThrow);
	IntendTurnRight(RightThrow);
}