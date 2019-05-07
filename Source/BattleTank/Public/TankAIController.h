// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
	public:
		ATank* GetControlledTank() const;
		// Actually, we should return a TArray... ?
		ATank* GetPlayerTank() const;

	protected:
		// Self made override of BeginPlay() - virtual function in Actor class
		virtual void BeginPlay() override;

	private:
		ATank* PlayerTank = nullptr;
};
