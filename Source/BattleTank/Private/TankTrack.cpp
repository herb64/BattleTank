// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"


void UTankTrack::SetThrottle(float Throttle)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s: Throttle set: %f"), *GetName(), Throttle);

	// Clamp forward vector, so that acceleration of tanks cannot be changed in editor
	// by setting a higher scale value on the axis binding, which is used as Throttle.
	FVector ForceApplied = GetForwardVector() * FMath::Clamp(Throttle, -1.0f, 1.0f) * MaxTrackForce;
	FVector ForceLocation = GetComponentLocation();

	// Owner is the tank. The root component is the upper component (a scene component), which
	// here is the tank static mesh component.
	// A scene component does not allow to apply any forces. Going down inheritance tree
	// Primitive Component has this functionality, so we cast this
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	UE_LOG(LogTemp, Warning, TEXT("%s: Apply force: %s"), *GetName(), *ForceApplied.ToString());

}