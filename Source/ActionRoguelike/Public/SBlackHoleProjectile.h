// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "SBlackHoleProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBlackHoleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBlackHoleProjectile();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};