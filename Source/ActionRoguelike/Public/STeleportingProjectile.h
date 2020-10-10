// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportingProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASTeleportingProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_Begin;
	// Called when the game starts or when spawned
	virtual void Explode_Implementation() override;
	
	//void TimeElapsed();
	void TeleportCaster();
	virtual void BeginPlay() override;

public:	
	ASTeleportingProjectile();

};
