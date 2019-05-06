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

	UE_LOG(LogTemp, Warning, TEXT("TankPlayerController - beginplay..."));
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("No possessed pawn..."));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s"), *ControlledTank->GetName());
	}
}

/// Experimental code with callbacks - does only log once after starting Play
/// No logs written when using Eject or F8...
void ATankPlayerController::OnPossess(APawn* possessedPawn) 
{
	Super::OnPossess(possessedPawn);
	UE_LOG(LogTemp, Warning, TEXT("Possessing Pawn: %s"), *possessedPawn->GetName());
}

/// Experimental code with callbacks - does only log twice after stopping play
void ATankPlayerController::OnUnPossess() 
{
	Super::OnUnPossess();
	UE_LOG(LogTemp, Warning, TEXT("Unpossessing currently possessed pawn"));
}