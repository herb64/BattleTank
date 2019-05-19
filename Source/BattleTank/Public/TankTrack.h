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

	// Maximum force in Newton that can be applied to the Track.
	UPROPERTY(EditDefaultsOnly, Category = Driving, meta = (DisplayName = "Max Track Force", UIMin = "100000.0", UIMax = "200000000.0", ClampMin = "100000.0", ClampMax = "200000000.0"))
	float MaxTrackForce = 100000000.0f;

	UPROPERTY(EditAnywhere, Category = Driving, meta = (DisplayName = "Dummy", UIMin = "100000.0", UIMax = "2000000.0", ClampMin = "100000.0", ClampMax = "2000000.0"))
	float Dummy = 1000000.0f;

	UPROPERTY(EditAnywhere, Category = Driving, meta = (DisplayName = "Dummy", UIMin = "100000.0", UIMax = "2000000.0", ClampMin = "100000.0", ClampMax = "2000000.0"))
	float Dummy2 = 1100000.0f;
};
