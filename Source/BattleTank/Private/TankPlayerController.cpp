// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankAimComponent.h"

// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"	// e.g. DrawDebugLine
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>



void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//UTankAimComponent* AimComponent = Cast<UTankAimComponent>(ControlledTank->GetComponentByClass(TSubclassOf<UTankAimComponent>()));
	UTankAimComponent* AimComponent = GetPawn()->FindComponentByClass<UTankAimComponent>();
	if (AimComponent) {
		FoundAimComponent(AimComponent);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Player controller: no aiming component at beginplay"));
	}
}


void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimTowardsCrosshair();
}


void ATankPlayerController::AimTowardsCrosshair()
{
	// in case we died and depossessed the pawn, we need to check
	UTankAimComponent* AimComponent = GetPawn()->FindComponentByClass<UTankAimComponent>();

	if (!ensure(AimComponent)) return;

	FVector OutHit = FVector(0.0f, 0.0f, 1.0f);
	if (!GetSightRayHitLocation(OutHit)) return;
	
	AimComponent->AimAt(OutHit);
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Do a line trace from our tank position with a reach of at least the range we can shoot at - make it a variable??
	// For linetrace, we need a start point vector and a rotator for the direction which is delivered to us by DeprojectScreenPositiontoWorld
	// https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/APlayerController/DeprojectScreenPositionToWorld/index.html
	// This needs Screenx/y as input and delivers a position and a direction VECTOR (no rotator for direction)

	// 1. Get Look direction vector
	FVector lookDirection = FVector(0.0f, 0.0f, 1.0f);
	if (!GetLookDirection(lookDirection)) return false;
	
	// 2. Get hit location for this look direction vector
	FVector hitLocation = FVector(0.0f, 0.0f, 1.0f);
	if (!GetLookVectorHitLocation(lookDirection, hitLocation)) return false;
	OutHitLocation = hitLocation;
	return true;
}


bool ATankPlayerController::GetLookDirection(FVector& CamWorldDir) const
{
	// Get Viewport information - using player controller method
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	// deproject needs float value as input for screen position - how about using FVector2D for this as in course
	float CrosshairScreenPositionX = (float)ViewportSizeX * CrosshairWidgetPositionX;
	float CrosshairScreenPositionY = (float)ViewportSizeY * CrosshairWidgetPositionY;

	FVector CamWorldLoc;
	// This returns values in Camera terms. We need the world direction.
	return DeprojectScreenPositionToWorld(
		CrosshairScreenPositionX,
		CrosshairScreenPositionY,
		CamWorldLoc,		// not used
		CamWorldDir			// this is, what we are interested in
	);
}


bool ATankPlayerController::GetLookVectorHitLocation(FVector lookDirection, FVector& OutHitLocation) const
{
	FHitResult hitResult;

	FVector vStart = PlayerCameraManager->GetCameraLocation();
	FVector vEnd = vStart + lookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(
		hitResult,
		vStart,
		vEnd,
		ECollisionChannel::ECC_Visibility							// hit anything visible
		//FCollisionQueryParams(ECollisionChannel::ECC_Visibility),
		//FCollisionResponseParams(ECollisionResponse::ECR_Overlap)
		))
	{
		OutHitLocation = hitResult.Location;
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitResult.GetActor()->GetName());
		// https://wiki.unrealengine.com/Logs,_Printing_Messages_To_Yourself_During_Runtime
		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Orange, FString::Printf(TEXT("Hit: %s"), *hitResult.GetActor()->GetName()));
		return true;
	}

	OutHitLocation = FVector(0.0f, 0.0f, 1.0f);
	return false;
}


