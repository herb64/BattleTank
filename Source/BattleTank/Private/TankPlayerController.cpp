// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

/// Get pointer to the Tank object controlled by this Player Controller
ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller: no possessed pawn..."));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Player Controller: possessed Pawn = %s"), *ControlledTank->GetName());
	}
}


// Tick manually overriden
void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller: tick - %f"), DeltaSeconds);
	AimTowardsCrosshair();
}


/// Experimental code with callbacks - does only log once after starting Play
/// No logs written when using Eject or F8...
void ATankPlayerController::OnPossess(APawn* possessedPawn) 
{
	Super::OnPossess(possessedPawn);
	//UE_LOG(LogTemp, Warning, TEXT("Just experiment... Possessing Pawn: %s"), *possessedPawn->GetName());
}

/// Experimental code with callbacks - does only log twice after stopping play
void ATankPlayerController::OnUnPossess() 
{
	Super::OnUnPossess();
	//UE_LOG(LogTemp, Warning, TEXT("Just experment... Unpossessing currently possessed pawn"));
}


void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) return;

	// We actually do control a tank, so lets find the point in the world we are currently aiming at
	// result is written into hit vector.
	FVector hit = FVector(0.0f, 0.0f, 1000.0f); // Init to strange value, useful for debugging
	if (GetSightRayHitLocation(hit))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit at %s"), *hit.ToString());
	}

}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Do a line trace from our tank position with a reach of at least the range we can shoot at - make it a variable??
	// For linetrace, we need a start point vector and a rotator for the direction which is delivered to us by DeprojectScreenPositiontoWorld
	// https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/APlayerController/DeprojectScreenPositionToWorld/index.html
	// This needs Screenx/y as input and delivers a position and a direction VECTOR (no rotator for direction)

	FVector lookDirection = FVector(1.0f);
	if (!GetLookDirection(lookDirection)) return false;

	UE_LOG(LogTemp, Warning, TEXT("Dir: %s"), *lookDirection.ToString());
	
	// next going for a line trace starting at our tank in direction of the aim point

	OutHitLocation = FVector(177.0f, 177.0f, 17.0f);
	return true;
}

/// get the current look direction
bool ATankPlayerController::GetLookDirection(FVector& CamWorldDir) const
{
	// Get Viewport information - using player controller method
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	// deproject needs float value as input for screen position - how about using FVector2D for this as in course
	float CrosshairScreenPositionX = (float)ViewportSizeX * CrosshairWidgetPositionX;
	float CrosshairScreenPositionY = (float)ViewportSizeY * CrosshairWidgetPositionY;

	//UE_LOG(LogTemp, Warning, TEXT("ViewPort: %d / %d, Crosshair %f / %f"), ViewportSizeX, ViewportSizeY, CrosshairScreenPositionX, CrosshairScreenPositionY);

	FVector CamWorldLoc;
	// This returns values in Camera terms. We need the world direction.
	return DeprojectScreenPositionToWorld(
		CrosshairScreenPositionX,
		CrosshairScreenPositionY,
		CamWorldLoc,		// not used
		CamWorldDir			// this is, what we are interested in
	);
}
