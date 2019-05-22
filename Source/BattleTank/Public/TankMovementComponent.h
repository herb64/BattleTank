// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Movement)
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, Category = Movement)
	void IntendMoveBackward(float Throw);

	UFUNCTION(BlueprintCallable, Category = Movement)
	void IntendTurnRight(float Throw);

	UFUNCTION(BlueprintCallable, Category = Movement)
	void IntendTurnLeft(float Throw);

	void InitializeViaCpp();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void InitializeFromWithinBlueprint(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet);

	virtual void RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed) override;

private:
	UTankTrack *LeftTrack = nullptr;
	UTankTrack *RightTrack = nullptr;

	// Maximum Speed of the tank in km/h
	UPROPERTY(EditDefaultsOnly, Category = Movability, meta = (DisplayName = "Max Speed", UIMin = "30.0", UIMax = "90.0", ClampMin = "30.0", ClampMax = "90.0"))
	float MaxTankSpeed = 30.0f;

};
