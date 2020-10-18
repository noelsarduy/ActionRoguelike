// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportingProjectile.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>


// Sets default values
ASTeleportingProjectile::ASTeleportingProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 8000.f;
}


void ASTeleportingProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Explode & Teleport
	GetWorldTimerManager().SetTimer(TimerHandle_Begin, this, &ASTeleportingProjectile::Explode, DetonateDelay);


}


void ASTeleportingProjectile::Explode_Implementation()
{
	//Clear Timer if on hit
	GetWorldTimerManager().ClearTimer(TimerHandle_Begin);
	//Display Explosion effect & stop EffectComp
	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX, GetActorLocation(), GetActorRotation());
	EffectComp->DeactivateSystem();
	//
	MoveComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), FRotator::ZeroRotator);
	SetActorEnableCollision(false);
	//Teleport Actor
	FTimerHandle Teleport;
	GetWorldTimerManager().SetTimer(Teleport, this, &ASTeleportingProjectile::TeleportCaster, DetonateDelay);
}

void ASTeleportingProjectile::TeleportCaster()
{
	AActor* Caster = GetInstigator();
	if (ensure(Caster))
	{
		Caster->TeleportTo(GetActorLocation(), Caster->GetActorRotation(), false, false);
	}
	//Caster->SetActorLocation(GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	Destroy();
}

// Called when the game starts or when spawned
