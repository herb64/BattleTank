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
	UE_LOG(LogTemp, Warning, TEXT("Player Controller: tick - %f"), DeltaSeconds);
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

	// do a line trace for intersection with world to check destination

}
