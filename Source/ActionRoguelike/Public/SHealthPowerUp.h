// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHealthPowerUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPowerUp();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	USphereComponent* SphereComp;
	virtual void BeginPlay() override;
	FTimerHandle Respawn;

	UFUNCTION()
	void Activate();
	void Cooldown();

	void ActivateLogic(bool bIsReady);

public:	
	void Interact(APawn* InstigatorPawn);
	virtual void Tick(float DeltaTime) override;

};
