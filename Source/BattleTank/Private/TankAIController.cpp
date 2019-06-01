// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankAimComponent.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}


void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO - for one player only, what about multiple players?
	APlayerController* pPlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(pPlayerController)) return;
	APawn* PlayerPawn = pPlayerController->GetPawn();
	if (!ensure(PlayerPawn)) return;

	// Movement towards player tank
	MoveToActor(PlayerPawn, AcceptanceRadius);

	// Aiming towards playertank
	UTankAimComponent* AimComponent = GetPawn()->FindComponentByClass<UTankAimComponent>();
	if (!ensure(AimComponent)) return;
	AimComponent->AimAt(PlayerPawn->GetActorLocation());
	
	// Firing at playertank - TODO needs fix!!
	//AimComponent->Fire();
}

