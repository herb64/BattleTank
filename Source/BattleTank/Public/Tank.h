// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"		// Must be last include

// Forward declarations
class UTankAimComponent;
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
	void AimAt(FVector hitLocation);

	void SetTankAimComponent(UTankAimComponent* AimComp);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UTankAimComponent* TankAimComponent = nullptr;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

	// Speed of Projectile. This is used to calculate the aiming direction in combination with the barrel endpoint and the desired hit location.
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (DisplayName = "Projectile Speed", UIMin = "3000.0", UIMax = "30000.0", ClampMin = "3000.0", ClampMax = "30000.0"))
	float LaunchSpeed = 10000.0f;

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
