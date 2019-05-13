// Fill out your copyright notice in the Description page of Project Settings.

// TODO - remove that class from code. We do not subclass from Projectile Movement Component

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileMoveComponent.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API UProjectileMoveComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	
public:
	// Constructor added manually
	//UProjectileMoveComponent();
};
