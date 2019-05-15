// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
//UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = ("Collision"))
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Rotate(float RelativeSpeed);

/// Was private before, made turret SM disappear. 
/// see also https://issues.unrealengine.com/issue/UE-63298
public:
	// Maximum Turret Rotation Speed in degrees per second. This is used in combination with Relative Speed factor 1.0 .. -1.0 for left/right rotation
	UPROPERTY(EditDefaultsOnly, Category = Movability, meta = (DisplayName = "Rotate Speed", UIMin = "5.0", UIMax = "20.0", ClampMin = "5.0", ClampMax = "20.0"))
	float MaxRotationSpeed = 10.0f;

	// Maximum Turret Rotation in degrees in left/right direction
	UPROPERTY(EditDefaultsOnly, Category = Movability, meta = (DisplayName = "Max Angle", UIMin = "40.0", UIMax = "120.0", ClampMin = "40.0", ClampMax = "120.0"))
	float MaxRotationAngle = 20.0f;
};
