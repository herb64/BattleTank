// Copyright Herbert Mehlhose, 2019
// Note: as this is for learning purpose, there's quite some commented code inside from experiments


#include "TankMovementComponent.h"
#include "tank.h"

#include "TankTrack.h"

/// Blueprint based setting
void UTankMovementComponent::InitializeFromWithinBlueprint(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	UE_LOG(LogTemp, Warning, TEXT("%s: InitializeFromWithinBlueprint Left: %s, Right: %s"), *GetOwner()->GetName(), *LeftTrack->GetName(), *RightTrack->GetName());
}

/// E X P E R I M E N T A L   C O D E   O N L Y
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
	LeftTrack->SetThrottle(-Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnLeft(float Throw)
{
	if (!ensure(LeftTrack) || !ensure(RightTrack)) return;
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}


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