// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create movement default inherited component named "Projectile Movement"
	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	if (ensure(projectileMovementComponent)) 
	{
		projectileMovementComponent->SetAutoActivate(false);
	}
}


void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectile::Launch(float Speed) const
{
	if (!ensure(projectileMovementComponent)) return;
	// our forward vector: at Projectile spawn in TankAimComponent::Fire(), we did set
	// the socketrotation, so that this now points in correct direction.
	projectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	projectileMovementComponent->Activate();

}

