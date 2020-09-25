// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosive.h"

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


	float RandomStrength = FMath::RandRange(700.0f, 1000.0f);
	float ImpulseFactor = 5.0f;
	float RadiusFactor = 1.5f;
	Shockwave->ForceStrength = RandomStrength;
	Shockwave->ImpulseStrength = RandomStrength * ImpulseFactor;
	Shockwave->Radius = RandomStrength * RadiusFactor;
}

// Called when the game starts or when spawned
void ASExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASExplosive::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Shockwave->FireImpulse();
}

// Called every frame
void ASExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

