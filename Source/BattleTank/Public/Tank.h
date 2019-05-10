// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"		// Must be last include

// Forward declarations
class UTankAimComponent;
class UTankBarrel;
class UTankTurret;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	void AimAt(FVector hitLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UTankAimComponent* TankAimComponent = nullptr;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category=Setup)
	void SetBarrelReference(UTankBarrel* BarrelToSet);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetTurretReference(UTankTurret* TurretToSet);

private:
	// Speed of Projectile. This is used to calculate the aiming direction in combination with the barrel endpoint and the desired hit location.
	UPROPERTY(EditAnywhere, Category = Firing, meta = (DisplayName = "Projectile Speed", UIMin = "3000.0", UIMax = "30000.0", ClampMin = "3000.0", ClampMax = "30000.0"))
	float LaunchSpeed = 10000.0f;

};
