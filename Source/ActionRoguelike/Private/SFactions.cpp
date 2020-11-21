// Fill out your copyright notice in the Description page of Project Settings.


#include "SFactions.h"

USFactions::USFactions()
{

	FactionFriendlyThreshold = 100;
	FactionNeutralThreshold;
	FactionEnemyThreshold = -100;
	// ...
}


int32 USFactions::GetFactionAffinity(USFactions* Faction) const
{
	for ( TPair<USFactions*, int32> KnownFaction : FactionAffinites)
	{
		if (KnownFaction.Key->GetFactionName() == Faction->GetFactionName())
		{
			return KnownFaction.Value;
		}
	}

	return 0;
}

FName USFactions::GetFactionName() const
{
	return FactionName;
}

TArray<AActor*> USFactions::GetFactionMemebers()
{
	return FactionMemebers;
}

int32 USFactions::GetFactionFriendlyThreshold() const
{
	return FactionFriendlyThreshold;
}

int32 USFactions::GetFactionNeutralThreshold() const
{
	return FactionNeutralThreshold;
}

int32 USFactions::GetFactionEnemyThreshold() const
{
	return FactionEnemyThreshold; 
}

TMap<USFactions*, int32> USFactions::GetFactionAffinities() const
{
	return FactionAffinites;
}

void USFactions::SetFactionAffinities(USFactions* OtherFAction, int32 Delta)
{
	int32 NewAffinity = FactionAffinites.FindRef(OtherFAction);
	 
	 NewAffinity += Delta;
	 
	 FactionAffinites.Emplace(OtherFAction) = NewAffinity;
}

void USFactions::InitializeGameFactions(TSet<TSubclassOf<USFactions>> FactionsFromGameMode, TSet<USFactions*> GameFactions)
{
	//Loop through the assigned factions to see if they are in game
	for (TPair<TSubclassOf<USFactions>, int32> AssignedFaction: AssignableFactionAffinites)
	{
		//If in game add the element from the shared gamefactions pointer array to this class's arrays
		if (FactionsFromGameMode.Contains(AssignedFaction.Key))
		{
			//ccouldn't find a function to get a * pointer to GameFactions, so had to loop again
			for (USFactions* FactionPointer : GameFactions)
			{
				if (FactionPointer->GetClass() == AssignedFaction.Key->GetClass())
				{
					// Assign pointer from GameModeBase, asign value from AssignedFactions
					FactionAffinites.Add(FactionPointer, AssignedFaction.Value);
					break;
				}
			}
			
		}
	}

	for (TSubclassOf<AActor> FactionPawn : FactionMemebersAssignable)
	{
		AActor* ActorPointer = NewObject<AActor>(FactionPawn);
		
		FactionMemebers.Add(ActorPointer);
	}
}

// void USFactions::AddCharacterToFaction(TSubclassOf<AActor>  Character)
// {
// 	FactionMemebers.Add(Character);
// }
// 
// void USFactions::RemoveCharacterFromFaction(TSubclassOf<AActor>  Character)
// {
// 	FactionMemebers.Remove(Character);
// }

// TMap<TSubclassOf<USFactions>, int32> USFactions::GetDefaultFactionAffinites()
// {
// 	return DefaultFactionAffinites;
// }
