// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = ("Collision"))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Elevate(float RelativeSpeed);

private:
	// Maximum Barrel Eleveation Speed in degrees per second. This is used in combination with Relative Speed factor 1.0 .. -1.0 for up/down movement
	UPROPERTY(EditAnywhere, Category = Movability, meta = (DisplayName = "Max. Speed", UIMin = "5.0", UIMax = "20.0", ClampMin = "5.0", ClampMax = "20.0"))
	float MaxElevationSpeed = 10.0f;

	// Minimum Barrel Elevation in degrees. Determines lowest position.
	UPROPERTY(EditAnywhere, Category = Movability, meta = (DisplayName = "Min. Elevation", UIMin = "0.0", UIMax = "20.0", ClampMin = "0.0", ClampMax = "20.0"))
	float MinimumElevation = 20.0f;

	// Maximum Barrel Elevation in degrees. Determinex highest position.
	UPROPERTY(EditAnywhere, Category = Movability, meta = (DisplayName = "Max. Elevation", UIMin = "50.0", UIMax = "80.0", ClampMin = "50.0", ClampMax = "80.0"))
	float MaximumElevation = 60.0f;
};
