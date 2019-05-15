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
}


void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* pPlayerController = GetWorld()->GetFirstPlayerController();
	if (!pPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("No player controller"));
		return;
	}
	ATank* pPlayerTank = Cast<ATank>(pPlayerController->GetPawn());
	if (!pPlayerTank)
	{
		UE_LOG(LogTemp, Error, TEXT("No player tank"));
		return;
	}
	ATank* pControlledTank = Cast<ATank>(GetPawn());
	if (!pControlledTank)
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled AI Tank"));
		return;
	}

	pControlledTank->AimAt(pPlayerTank->GetActorLocation());

	//pControlledTank->Fire();
}

