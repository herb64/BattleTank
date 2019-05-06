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
};
