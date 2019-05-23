// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Tank.h"
#include "TankAimComponent.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"


/// BeginPlay manually overridden to initialize stuff
void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	ATank* ControlledTank = Cast<ATank>(GetPawn());
	if (!ensure(ControlledTank)) return;

	//UTankAimComponent* AimComponent = Cast<UTankAimComponent>(ControlledTank->GetComponentByClass(TSubclassOf<UTankAimComponent>()));
	UTankAimComponent* AimComponent = ControlledTank->FindComponentByClass<UTankAimComponent>();
	if (AimComponent) {
		ControlledTank->SetTankAimComponent(AimComponent);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("AI controller: no aiming component at beginplay"));
	}
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

	MoveToActor(pPlayerTank, AcceptanceRadius);

	pControlledTank->AimAt(pPlayerTank->GetActorLocation());

	//pControlledTank->Fire();
}

