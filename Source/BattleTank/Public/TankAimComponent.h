// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TankAimComponent.generated.h"		// LAST include!

UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Locked
};


// Forward declaration for UTankBarrel
class UTankBarrel;
class UTankTurret;


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

	UPROPERTY(BlueprintReadOnly, Category = "Firing")
	EFiringStatus FiringStatus = EFiringStatus::Locked;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AimAt(FVector hitLocation);
	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	// Getter for firingstatus enum... or should we just expose the value?
	// Experiment shows, that the Getter has an execution pin
	UFUNCTION(BlueprintCallable, Category = "Firing")
	EFiringStatus GetFiringStatus();

private:
	// Replacing UStaticMeshComponent by our C++ class. Add forward declaration
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	void MoveBarrelTowards(FVector AimDirection);

	// Speed of Projectile. This is used to calculate the aiming direction in combination with the barrel endpoint and the desired hit location.
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (DisplayName = "Projectile Speed", UIMin = "3000.0", UIMax = "30000.0", ClampMin = "3000.0", ClampMax = "30000.0"))
	float LaunchSpeed = 10000.0f;
	
};

