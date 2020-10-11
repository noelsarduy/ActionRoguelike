// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerUpBase.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUpBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	USphereComponent* SphereComp;
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;
	FTimerHandle Respawn;
	
	void Activate();
	void Cooldown();
	void PowerUpState(bool bIsReady);

public:	

	void Interact_Implementation(APawn* InstigatorPawn) override;
	ASPowerUpBase();

};
