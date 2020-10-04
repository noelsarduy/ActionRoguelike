// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosive.h"
#include <DrawDebugHelpers.h>

// Sets default values
ASExplosive::ASExplosive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	Shockwave = CreateDefaultSubobject <URadialForceComponent>("Shockwave");
	Shockwave->SetupAttachment(MeshComp);
	MeshComp->SetSimulatePhysics(true);
	
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosive::OnCompHit);


	float RandomStrength = FMath::RandRange(400.0f, 800.0f);
	float ImpulseFactor = 5.0f;
	float RadiusFactor = 1.5f;
	Shockwave->bImpulseVelChange = true;
	Shockwave->ForceStrength = /*2500.0f;8*/RandomStrength;
	Shockwave->ImpulseStrength = /*2500.0f;*/RandomStrength * ImpulseFactor;
	Shockwave->Radius = /*750.0f;*/RandomStrength * RadiusFactor;
}

// Called when the game starts or when spawned
void ASExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASExplosive::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Shockwave->FireImpulse();
		UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

	// %s = string
	// %f = float
	// logs: "OtherActor: MyActor_1, at gametime: 124.4" 
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

	// Detailed info on logging in ue4
	// https://nerivec.github.io/old-ue4-wiki/pages/logs-printing-messages-to-yourself-during-runtime.html
}

// Called every frame
void ASExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

