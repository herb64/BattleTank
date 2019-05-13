// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create movement component, give it a name
	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	if (projectileMovementComponent) 
	{
		projectileMovementComponent->SetAutoActivate(false);
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectile::Launch(float Speed) const
{
	if (!projectileMovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile has no movement component"))
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Firing at %f"), Speed);
		// our forward vector: at Projectile spawn in Tank::Fire(), we did set
		// the socketrotation, so that this now points in correct direction.
		projectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
		projectileMovementComponent->Activate();
	}
}

