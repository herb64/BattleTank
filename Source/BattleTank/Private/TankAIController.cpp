// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Tank.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"


/// BeginPlay manually overridden to initialize stuff
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

	PlayerTank = GetPlayerTank();
}


void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimTowardsPlayerTank();
}


void ATankAIController::AimTowardsPlayerTank()
{
	if (!GetControlledTank() || !GetPlayerTank()) return;

	GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
}


/// Get pointer to our own controlled Tank
ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

/// Get pointer to the Player Tank controlled by the first player controller
/// NOT handling multiple 
ATank * ATankAIController::GetPlayerTank() const
{
	// We might use the number to decide, if we need an iterator..
	//UE_LOG(LogTemp, Warning, TEXT("AI Controller: %d player controllers present."), GetWorld()->GetNumPlayerControllers());
	APlayerController* pPC = GetWorld()->GetFirstPlayerController();
	if (pPC)
	{
		ATank* pPT = Cast<ATank>((pPC->GetPawn()));
		//UE_LOG(LogTemp, Warning, TEXT("AI Controller %s: found Enemy Player Tank %s to fight against."), *GetName(), *pPT->GetName());
		return pPT;
	}
	else {
		//UE_LOG(LogTemp, Error, TEXT("AI Contoller %s: could not detect any enemy Player Tank to fight against."), *GetName());
		return nullptr;
	}
}
