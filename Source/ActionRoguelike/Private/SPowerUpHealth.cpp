// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpHealth.h"
#include "SAttributeComponent.h"


ASPowerUpHealth::ASPowerUpHealth()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

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
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			Cooldown();
		}
	}
}

