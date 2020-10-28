// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditsPowerUp.h"
#include "SPlayerState.h"
#include "SPowerUpBase.h"
#include "Components/SphereComponent.h"

ASCreditsPowerUp::ASCreditsPowerUp()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	MeshComp->SetupAttachment(RootComponent);

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASCreditsPowerUp::OnActorOverlap);

	CreditValue = 100;
}

void ASCreditsPowerUp::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Creditor = Cast<APawn>(OtherActor);
	if (Creditor) 
	{
		ASPlayerState* InstigatorState = Creditor->GetPlayerState<ASPlayerState>();

		if (InstigatorState)
		{
			InstigatorState->ApplyCreditsChange(CreditValue);
			Cooldown();
		}
	}

}

void ASCreditsPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	ASPlayerState* InstigatorState = InstigatorPawn->GetPlayerState<ASPlayerState>();

	if (InstigatorState != nullptr)
	{
		InstigatorState->ApplyCreditsChange(CreditValue);
		Cooldown();
	}
}