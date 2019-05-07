// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "tank.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()


	public:
		ATank* GetControlledTank() const;
		void OnPossess(APawn*);
		void OnUnPossess();

	protected:
		// Self made override of BeginPlay() - virtual function in Actor class
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaSeconds) override;

	private:
		// makes the tank pointing the barrel towards the destination where crosshair
		// intersects the world.
		void AimTowardsCrosshair();
		bool GetSightRayHitLocation(FVector& OutHitLocation) const;

		bool GetLookDirection(FVector&) const;

		// Crosshair postion for aiming, relative to screen position in X direction
		UPROPERTY(EditAnywhere, meta = (DisplayName = "Crosshair X"))
		float CrosshairWidgetPositionX = 0.5f;
		// Crosshair postion for aiming, relative to screen position in Y direction, starting at top
		UPROPERTY(EditAnywhere, meta = (DisplayName = "Crosshair Y"))
		float CrosshairWidgetPositionY = 0.333333f;
};
