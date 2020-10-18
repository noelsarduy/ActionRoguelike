// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}


bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float PrevHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelata = Health - PrevHealth;
	OnHealthChange.Broadcast(InstigatorActor, this, Health, ActualDelata);

	return true;
}

/*bool USAttributeComponent::ApplyGradualHealthChange(AActor* InstigatorActor, float Delta, float HealingPeriod)
{
	FTimerHandle HealingTimer;
	while (Health < HealthMax)
	{
		GetWorldTimerManager().SetTimer(HealingTimer, this, &USAttributeComponent::ApplyHealthChange(InstigatorActor, Delta), HealingPeriod);
		GetWorldTimerManager().ClearTimer(HealingTimer);
	}
	
	return true;
}*/

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

