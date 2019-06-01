// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Delegate.h"

#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Engine/Public/DrawDebugHelpers.h"	// e.g. DrawDebugLine

// Experimenting with physical materials
//#include <Runtime/Engine/Classes/Materials/MaterialInterface.h>
#include <Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h>


// Just switch workaround off by commenting this
//#define WORKAROUND

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

	// Just some experiments here...
	ExperimentWithBodyAndMass();
}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp(CurrentThrottle + Throttle, -1.0f, 1.0f);
}

void UTankTrack::DriveTrack()
{
	// USceneComponent does not allow to apply any forces. Going down inheritance tree
	// UPrimitiveComponent has this functionality, so we cast to at least this, or even
	// better to UStaticMeshComponent.
	// UObject
	//   ...
	//     USceneComponent
	//       UPrimitiveComponent		<<< Provides AddForce()
	//         ...
	//           UStaticMeshComponent	<<< Root Component is a Static Mesh Component
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	FVector ForceApplied = GetForwardVector() * CurrentThrottle * MaxTrackForce;
	FVector ForceLocation = GetComponentLocation();
	ForceLocation.Z = TankRoot->GetCenterOfMass().Z;

	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

	/////////////////////////////////////
	/// Debugging and experiments go here

	DrawDebugLine(
		GetWorld(),
		ForceLocation,
		ForceLocation + ForceApplied.GetSafeNormal() * 500.0f,
		FColor::White,
		false,
		0.5f,
		0,
		4.0f
	);
}


/**
 *  Just some experimental function to play with Center of Mass, Physical Materials etc...
 *  GetBodyInstance comes with UPrimitiveComponent. 
 *  The Body Instance is a "Container for a physics representation of an object"
*/
void UTankTrack::ExperimentWithBodyAndMass()
{
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	FBodyInstance* TankBodyInstance = GetBodyInstance();
	if (ensure(TankBodyInstance)) {
		// Body objects, their names and welding
		FString bodyName = TankBodyInstance->GetBodyDebugName();
		FBodyInstance* WeldParent = TankBodyInstance->WeldParent;
		FString weldParentName = FString("No weld parent");
		if (WeldParent) weldParentName = WeldParent->GetBodyDebugName();
		UE_LOG(LogTemp, Warning, TEXT("%s/%s> Bodyname: %s, weld parent: %s"), *GetOwner()->GetName(), *GetName(), *bodyName, *weldParentName);

		// GetSimple/ComplexPhysicalMaterial(s) - only returns the default material.
		// Cause for this: it reports only, if physical material is set in Material Details, NOT if it is
		// set in physcial material override in component collision detail settings or within material instance
		UPhysicalMaterial* SimplePMat = TankBodyInstance->GetSimplePhysicalMaterial();
		float friction = 0.0f;
		float density = 0.0f;
		FString SimplePMatName = FString("No simple physical material");
		if (SimplePMat) {
			SimplePMatName = SimplePMat->GetName();
			friction = SimplePMat->Friction;
			density = SimplePMat->Density;
			UE_LOG(LogTemp, Warning, TEXT("%s/%s> Simple Physical Material: %s"), *GetOwner()->GetName(), *GetName(), *SimplePMatName);
		}

		TArray<UPhysicalMaterial*> ComplexPMatArray = TankBodyInstance->GetComplexPhysicalMaterials();
		for (UPhysicalMaterial* ComplexPMat : ComplexPMatArray) {
			UE_LOG(LogTemp, Warning, TEXT("%s/%s> Complex Physical Material: %s"), *GetOwner()->GetName(), *GetName(), *ComplexPMat->GetName());
		}

		// We could do the override here...
		// TankBodyInstance->SetPhysMaterialOverride();

		// Same result for all Center Of Mass calculations: welded to single rigid body
		FVector TankCOM = TankRoot->GetCenterOfMass();
		FVector TrackCOM = this->GetCenterOfMass();
		FVector BodyCOM = TankBodyInstance->GetCOMPosition();

		// Tank/Body Mass reports override value of tank + 2 x track mass.. shouldn't bodymass report the
		// sum for "non overridden" values only... 
		float TankMass = this->GetMass();
		float BodyMass = TankBodyInstance->GetBodyMass();
		uint8 IsMassOverridden = TankBodyInstance->bOverrideMass;
		// interesting: if not checking override, the "old" value still is reported somehow, although in editor
		// the 30000 is not shown any more. From where is that value taken? If checking again, it is shown, so the 
		// editor caches that somewhere...
		float BodyMassOverride = TankBodyInstance->GetMassOverride();

		UE_LOG(LogTemp, Warning, TEXT("%s/%s> body mass: %f, body mass override: %f(%d), tank mass: %f, PMat: %s, friction: %f, density: %f"), *GetOwner()->GetName(), *GetName(), BodyMass, BodyMassOverride, IsMassOverridden, TankMass, *SimplePMatName, friction, density);
		UE_LOG(LogTemp, Warning, TEXT("%s/%s> TrackCOM = %s, TankCOM = %s, BodyCOM = %s"), *GetOwner()->GetName(), *GetName(), *TrackCOM.ToString(), *TankCOM.ToString(), *BodyCOM.ToString());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s/%s> No Body instance"), *GetOwner()->GetName(), *GetName());
	}
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.0f;
}

/// TODO: check using impulse instead
/// https://community.gamedev.tv/t/addimpulse-instead-of-addforce/78331
/// ue4 docs state, that addforce is for more than one frame, while addImpulse is not accounting
/// for deltatime and just for one time application, e.g. explosions. So the community post makes sense to me.
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
