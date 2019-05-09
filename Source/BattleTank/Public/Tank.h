// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TankAimComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"		// Must be last include

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category=Setup)
	void SetBarrelReference(UStaticMeshComponent* BarrelToSet);

private:
	UPROPERTY(EditAnywhere, Category = Firing, meta = (DisplayName = "Projectile Speed"))
	float LaunchSpeed = 100000.0f;

};
