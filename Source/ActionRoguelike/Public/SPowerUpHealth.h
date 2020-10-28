// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SPowerUpHealth.generated.h"

class UMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUpHealth : public ASPowerUpBase
{
	GENERATED_BODY()

	protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditCost;
	
	public:
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	ASPowerUpHealth();

	
};
