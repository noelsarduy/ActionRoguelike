// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

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


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float PrevHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelata = Health - PrevHealth;
	OnHealthChange.Broadcast(nullptr, this, Health, ActualDelata);

	return true;
}

