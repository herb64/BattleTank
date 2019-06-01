// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

protected:
	// Self made override of BeginPlay() - virtual function in Actor class
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

//private:
	// Distance which AI tanks should keep from player tank
	UPROPERTY(EditAnywhere, Category = Driving, meta = (DisplayName = "MinDistance", UIMin = "2000.0", UIMax = "30000.0", ClampMin = "2000.0", ClampMax = "30000.0"))
	float AcceptanceRadius = 6000.0f;
};
