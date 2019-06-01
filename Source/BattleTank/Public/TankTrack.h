// Oh, just a test, if comment change also triggers loss of static mesh

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * Tank Track is used to apply forces to the tank... todo add more description
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	

public:

	// Add in the constructor to implement manual mesh add as workaround
	UTankTrack();

	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	void DriveTrack();

protected:
	virtual void BeginPlay() override;

private:
	/** To be called when tracks hit something - use for ground contact test */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ApplySidewaysForce();
	void ExperimentWithBodyAndMass();

	// Maximum force in Newton that can be applied to the Track.
	UPROPERTY(EditDefaultsOnly, Category = Driving, meta = (DisplayName = "Max Track Force", UIMin = "100000.0", UIMax = "200000000.0", ClampMin = "100000.0", ClampMax = "200000000.0"))
		float MaxTrackForce = 40000000.0f;

	float CurrentThrottle = 0.0f;
};
