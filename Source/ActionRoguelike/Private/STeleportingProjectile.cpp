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
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject <UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject <UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 10000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void ASTeleportingProjectile::TimeElapsed()
{
	//Clear Timer if on hit
	GetWorldTimerManager().ClearTimer(TimerHandle_Begin);
	//Display Explosion effect & stop EffectComp
	UGameplayStatics::SpawnEmitterAtLocation(this,ExplosionFX, GetActorLocation(), GetActorRotation());
	EffectComp->DeactivateSystem();
	//
	MovementComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), FRotator::ZeroRotator);
	SetActorEnableCollision(false);
	//Teleport Actor
	FTimerHandle Teleport;
	GetWorldTimerManager().SetTimer(Teleport, this, &ASTeleportingProjectile::Explode, 0.2f);
}

void ASTeleportingProjectile::Explode()
{
	AActor* Instigator = GetInstigator();
	Instigator->SetActorLocation(GetActorLocation());
	Destroy();
}

void ASTeleportingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}
// Called when the game starts or when spawned
void ASTeleportingProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Ignore Caster
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	//Explode & Teleport
	GetWorldTimerManager().SetTimer(TimerHandle_Begin, this, &ASTeleportingProjectile::TimeElapsed, 0.2f);


}

// Called every frame
void ASTeleportingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
