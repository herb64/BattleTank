// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Delegate.h"


// Just switch workaround off by commenting this
#define WORKAROUND

/*
 * W O R K A R O U N D   F O R   V A N I S H I N G   S T A T I C  M E S H
 * Set static mesh within the constructor manually
 */
UTankTrack::UTankTrack()
{
#ifdef WORKAROUND
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM(TEXT("/Game/Tank/tank_fbx_Track"));
	if (SM.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Set Static Mesh %s"), *SM.Object->GetName());
		this->SetStaticMesh(SM.Object);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Could not set static mesh to component"));
	}
#endif
}


void UTankTrack::BeginPlay() 
{
	Super::BeginPlay();
	// AddDynamic is a Macro...
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

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
	//UE_LOG(LogTemp, Warning, TEXT("%s: Apply force: %s"), *GetName(), *ForceApplied.ToString());

}

void UTankTrack::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Hit Event for %s"), *GetOwner()->GetName(), *this->GetName());
}