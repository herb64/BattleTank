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
	UTankTurret();
	void Rotate(float RelativeSpeed);

	float previousRotate = 0.0f;

/// Was private before, made turret SM disappear. 
/// see also https://issues.unrealengine.com/issue/UE-63298
/// 28.05.2019 - make private again
private:
	// Maximum Turret Rotation Speed in degrees per second. This is used in combination with Relative Speed factor 1.0 .. -1.0 for left/right rotation
	UPROPERTY(EditDefaultsOnly, Category = Movability, meta = (DisplayName = "Rotate Speed", UIMin = "5.0", UIMax = "40.0", ClampMin = "5.0", ClampMax = "40.0"))
	float MaxRotationSpeed = 20.0f;
};
