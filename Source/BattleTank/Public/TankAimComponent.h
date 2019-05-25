// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TankAimComponent.generated.h"		// LAST include!

UENUM()
enum class EFiringStatus : uint8
{
	Reloading,		// cannot fire, is still loading depending on reload time (red)
	Aiming,			// can fire, but still aiming (amber)
	Locked			// can fire, target is locked (green)
};


// Forward declaration for UTankBarrel
class UTankBarrel;
class UTankTurret;
class AProjectile;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) //already spawnable
class BATTLETANK_API UTankAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Firing status read by Widget Blueprint to set color of crosshair
	UPROPERTY(BlueprintReadOnly, Category = "Firing")
	EFiringStatus FiringStatus = EFiringStatus::Reloading;

public:	
	// Called every frame
	void AimAt(FVector hitLocation);
	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	// Getter for firingstatus enum... or should we just expose the value?
	// Experiment shows, that the Getter has an execution pin
	UFUNCTION(BlueprintCallable, Category = "Firing")
	EFiringStatus GetFiringStatus();

	UFUNCTION(BlueprintCallable, Category = "Firing")
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
	// Replacing UStaticMeshComponent by our C++ class. Add forward declaration
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	//void MoveBarrelTowards(FVector AimDirection);

	// Speed of Projectile. This is used to calculate the aiming direction in combination with the barrel endpoint and the desired hit location.
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (DisplayName = "Projectile Speed", UIMin = "3000.0", UIMax = "30000.0", ClampMin = "3000.0", ClampMax = "30000.0"))
	float LaunchSpeed = 10000.0f;
	
	float lastFireTime;

	// Tick() itself is final and cannot be overridden, but we have TickComponent
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
	
	FVector AimDirection = FVector(0.0f, 0.0f, 1.0f);
};

