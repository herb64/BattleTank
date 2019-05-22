// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;
/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// Self made override of BeginPlay() - virtual function in Actor class
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	float AcceptanceRadius = 3000.0f;
};
