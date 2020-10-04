// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportingProjectile.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>


// Sets default values
ASTeleportingProjectile::ASTeleportingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject <USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject <UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject <UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1500.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void ASTeleportingProjectile::TimeElapsed()
{
	MovementComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), FRotator::ZeroRotator);
	BlueprintExplode();
}

void ASTeleportingProjectile::Explode()
{
	AActor* Teleporter = GetInstigator();
	Teleporter->SetActorLocation(GetActorLocation());
	Destroy();
}

void ASTeleportingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TimeElapsed();
	Explode();
}
// Called when the game starts or when spawned
void ASTeleportingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_Begin, this, &ASTeleportingProjectile::TimeElapsed, 0.2f);
	GetWorldTimerManager().SetTimer(TimerHandle_Begin, this, &ASTeleportingProjectile::Explode, 0.4f);
	
	
}

// Called every frame
void ASTeleportingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

