// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"
#include <Components/SphereComponent.h>

// Sets default values
ASPowerUpBase::ASPowerUpBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;

}

// Called when the game starts or when spawned
void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

void ASPowerUpBase::Activate()
{
	PowerUpState(true);
}

void ASPowerUpBase::Cooldown()
{
	PowerUpState(false);

	GetWorldTimerManager().SetTimer(Respawn, this, &ASPowerUpBase::Activate, RespawnTime);
}

void ASPowerUpBase::PowerUpState(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);

	//Render visible or invisible
	RootComponent->SetVisibility(bIsActive, true);
}

