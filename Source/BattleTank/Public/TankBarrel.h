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

	// Draw debug lines Barrel: projectile>hitlocation in red and projectile>aimdirection in yellow
	UPROPERTY(EditAnyWhere, Category = Debugging, meta = (DisplayName = "Debuglines"))
	bool bDrawDebugLines = false;

/// Was private before, made turret SM disappear. 
/// see also https://issues.unrealengine.com/issue/UE-63298
public:
	// Maximum Barrel Elevation Speed in degrees per second. This is used in combination with Relative Speed factor 1.0 .. -1.0 for up/down movement
	UPROPERTY(EditDefaultsOnly, Category = Movability, meta = (DisplayName = "Barrel Speed", UIMin = "5.0", UIMax = "20.0", ClampMin = "5.0", ClampMax = "20.0"))
	float MaxElevationSpeed = 10.0f;

	// Minimum Barrel Elevation in degrees. Determines lowest position.
	UPROPERTY(EditDefaultsOnly, Category = Movability, meta = (DisplayName = "Min. Elevation", UIMin = "0.0", UIMax = "20.0", ClampMin = "0.0", ClampMax = "20.0"))
	float MinimumElevation = 0.0f;

	// Maximum Barrel Elevation in degrees. Determinex highest position.
	UPROPERTY(EditDefaultsOnly, Category = Movability, meta = (DisplayName = "Max. Elevation", UIMin = "50.0", UIMax = "80.0", ClampMin = "50.0", ClampMax = "80.0"))
	float MaximumElevation = 60.0f;
};
