// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller %s: no possessed pawn..."), *GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI Controller %s: possessed Pawn: %s"), *GetName(), *ControlledTank->GetName());
	}
}
