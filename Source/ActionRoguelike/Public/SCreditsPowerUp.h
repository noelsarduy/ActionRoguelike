// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SCreditsPowerUp.generated.h"


class UStaticMeshComponent;


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCreditsPowerUp : public ASPowerUpBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditValue;

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	ASCreditsPowerUp();

	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};