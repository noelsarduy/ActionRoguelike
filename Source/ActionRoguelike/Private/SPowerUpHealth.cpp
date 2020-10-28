// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpHealth.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"


ASPowerUpHealth::ASPowerUpHealth()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditCost = 10;

}

void ASPowerUpHealth::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		ASPlayerState* InstigatorState = InstigatorPawn->GetPlayerState<ASPlayerState>();

		if ((InstigatorState->GetCredits() >= CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
		{
			InstigatorState->ApplyCreditsChange(-CreditCost);
			Cooldown();
		}
	}
}

