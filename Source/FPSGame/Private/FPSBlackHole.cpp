// Fill out your copyright notice in the Description page of Project Settings.
// Test plug in github VS2019

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereDestroy = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDestroy"));
	SphereDestroy->SetupAttachment(MeshComp);

	// Bind to event
	SphereDestroy->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapSphereDestroy);

	SphereForce = CreateDefaultSubobject<USphereComponent>(TEXT("SphereForce"));
	SphereForce->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBlackHole::OverlapSphereDestroy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Paramètres nécessaire à la méthode AddRadialForce
	FVector BHLocation = GetActorLocation();
	float BHRadius = SphereForce->GetScaledSphereRadius();
	float BHStrentgh = -2000.f;

	// Recherche des components pouvant être gérés physiquement
	TArray<UPrimitiveComponent*> OverlappingComps;
	SphereForce->GetOverlappingComponents(OverlappingComps);

	// Pour chaque component trouvé on applique une force
	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			PrimComp->UPrimitiveComponent::AddRadialForce
			(
				BHLocation,
				BHRadius,
				BHStrentgh,
				RIF_Constant,
				true
			);
		}
	}

}