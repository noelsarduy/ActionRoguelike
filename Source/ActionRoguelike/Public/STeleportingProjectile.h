// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/SphereComponent.h>
#include "STeleportingProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASTeleportingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTeleportingProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	UParticleSystemComponent* EffectComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "FX");
	UParticleSystem* ExplosionFX;


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle TimerHandle_Begin;

	void TimeElapsed();
	void Explode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
