// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPowerUp.h"

// Sets default values
ASHealthPowerUp::ASHealthPowerUp()
{
	SphereComp = CreateDefaultSubobject("SphereComp");
	RootComponent = SphereComp;
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASHealthPowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASHealthPowerUp::Activate()
{
	ActivateLogic(true);
}

void ASHealthPowerUp::Cooldown()
{
	ActivateLogic(false);

	GetWorldTimerManager().SetTimer(Respawn, this, &ASHealthPowerUp::Activate, 10.0f);
}
// Called every frame
void ASHealthPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

