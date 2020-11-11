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

	bIsShown = true;
	SetReplicates(true);
	
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
	bIsShown = bIsActive;
	OnRep_IsShown();
}

void ASPowerUpBase::OnRep_IsShown()
{
	SetActorEnableCollision(bIsShown);

	//Render visible or invisible
	RootComponent->SetVisibility(bIsShown, true);
}

void ASPowerUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUpBase, bIsShown);
}
