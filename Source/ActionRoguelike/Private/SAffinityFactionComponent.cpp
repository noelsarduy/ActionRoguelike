// Fill out your copyright notice in the Description page of Project Settings.


#include "SAffinityFactionComponent.h"
#include "SFactions.h"
#include <Templates/SubclassOf.h>
#include <Engine/World.h>

USAffinityFactionComponent::USAffinityFactionComponent()
{
	IndividualFriendlyThreshold = 10;
	IndividualEnemyThreshold = -10;
	PrimaryComponentTick.bCanEverTick = true;
}




USAffinityFactionComponent* USAffinityFactionComponent::GetFactionComponents(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAffinityFactionComponent>(FromActor->GetComponentByClass(USAffinityFactionComponent::StaticClass()));
	}

	return nullptr;
}

USFactions* USAffinityFactionComponent::GetFaction() const
{
	return OwnerFaction;
}

void USAffinityFactionComponent::InitializeFaction(USFactions* OwningFaction)
{
	OwnerFaction = OwningFaction;
}

int32 USAffinityFactionComponent::GetAggro(AActor* Instigator) const
{
	if (Aggresors.Contains(Instigator))
	{
		return Aggresors.FindRef(Instigator);
	}
	return 0;
}

void USAffinityFactionComponent::AddAggresion(AActor* Instigator, int32 AggroLevel)
{
	if (Aggresors.Contains(Instigator))
	{
		SetAggro(Instigator, AggroLevel);

		return;
	}
	
	Aggresors.Add(Instigator, AggroLevel);
}

void USAffinityFactionComponent::RemoveAggresor(AActor* Instigator)
{
	Aggresors.FindAndRemoveChecked(Instigator);
}	

void USAffinityFactionComponent::SetAggro(AActor* Instigator, int32 AggroAmount)
{
	int32 NewAggro = Aggresors.FindRef(Instigator);

	NewAggro += AggroAmount;

	Aggresors.Emplace(Instigator) = NewAggro;
}

bool USAffinityFactionComponent::IsEnemy_Implementation(AActor* Instigator)
{
	USAffinityFactionComponent* FactionComponent = GetFactionComponents(Instigator);
	if (FactionComponent)
	{
		USFactions* OtherFaction = FactionComponent->GetFaction();
		if (OtherFaction != GetFaction())
		{
			if (GetFaction()->GetFactionAffinity(OtherFaction) <= GetFaction()->GetFactionEnemyThreshold())
			{
				return true;
			}
		}
	}
	return false;
}

bool USAffinityFactionComponent::IsFriendly_Implementation(AActor* Instigator)
{
	USAffinityFactionComponent* FactionComponent = GetFactionComponents(Instigator);
	if (FactionComponent)
	{
		USFactions* OtherFaction = FactionComponent->GetFaction();
		if (OtherFaction != GetFaction())
		{
			if (GetFaction()->GetFactionAffinity(OtherFaction) >= GetFaction()->GetFactionFriendlyThreshold())
			{
				return true;
			}
		}
	}
	return false;
}

bool USAffinityFactionComponent::IsNeutral_Implementation(AActor* Instigator)
{
	USAffinityFactionComponent* FactionComponent = GetFactionComponents(Instigator);
	if (FactionComponent)
	{
		USFactions* OtherFaction = FactionComponent->GetFaction();
		if (OtherFaction != GetFaction())
		{
			if (GetFaction()->GetFactionAffinity(OtherFaction) < GetFaction()->GetFactionFriendlyThreshold() && GetFaction()->GetFactionAffinity(OtherFaction) > GetFaction()->GetFactionEnemyThreshold())
			{
				return true;
			}
		}
	}
	return false;
}

void USAffinityFactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USAffinityFactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

