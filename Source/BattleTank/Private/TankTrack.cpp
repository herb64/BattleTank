// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Delegate.h"

#include <Runtime/Engine/Classes/Engine/Engine.h>


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
	CurrentThrottle = FMath::Clamp(CurrentThrottle + Throttle, -1.0f, 1.0f);
}

void UTankTrack::DriveTrack()
{
	FVector ForceApplied = GetForwardVector() * CurrentThrottle * MaxTrackForce;
	FVector ForceLocation = GetComponentLocation();

	// Owner is the tank. The root component is the upper component (a scene component), which
	// here is the tank static mesh component.
	// A scene component does not allow to apply any forces. Going down inheritance tree
	// Primitive Component has this functionality, so we cast this
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.0f;
}

void UTankTrack::ApplySidewaysForce()
{
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto RightSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	// TODO: in video, still using half of force: really correct? Here we go for a single track, while other in the air
	auto Acceleration = -RightSpeed / DeltaTime * GetRightVector();
	// Force = m * a
	auto Force = TankRoot->GetMass() * Acceleration;
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Cyan, FString::Printf(TEXT("RightSpeed: %f, Correction: %s"), RightSpeed, *Force.ToString()));
	TankRoot->AddForce(Force);	// Adding force at the tank location itself
}
