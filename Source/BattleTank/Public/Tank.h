// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"		// Must be last include

// Forward declarations
class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

	/// Instead of using AActor* pointer, use TSubclassOf for type safety to make Editor only offer valid choices to the Artist
	/// https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/TSubclassOf
	// The Projectile Class to be used (BTW: Choice limited by TSubclassOf)
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (DisplayName = "Projectile Class"))
	TSubclassOf<AProjectile> ProjectileBlueprint;

	// Reload time, determines maximum fire rate
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (DisplayName = "Reload time"))
	float ReloadTime = 4.0f;
	
private:
	// Local Barrel reference for spawning Projectiles in Fire()
	UTankBarrel* Barrel = nullptr;

	float lastFireTime;
};
