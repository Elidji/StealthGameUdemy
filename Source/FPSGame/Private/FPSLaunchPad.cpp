// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/character.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	// Génération du mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	// Génération de l'overlap box
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// Définie la taille de la box, si FVector n'a qu'un paramètre les 3 coordonnées ont cette valeurs = (200.f, 200.f, 200.f)
	OverlapComp->SetBoxExtent(FVector(200.f));
	OverlapComp->SetupAttachment(RootComponent);

	DecalComp = CreateAbstractDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.f);
	DecalComp->SetupAttachment(RootComponent);

	// Permet de lier l'événement OnComponentBeginOverlap à la méthode HandleOverlap
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);

	LPForce = 1500.f;

	LPAngle = FVector(0.f, 0.f, 2.f);
}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Permet de lancer le joueur dans la direction qu'il regarde
	// FVector LaunchVelocity = LPForce*(MyPawn->GetActorForwardVector());

	/// Permet de lancer le joueur dans la direction du launchpad 
	FVector LaunchVelocity = LPForce * (this->GetActorForwardVector() + LPAngle);

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player overlapped with launchpad !"));

		OtherCharacter->LaunchCharacter
		(
			LaunchVelocity,
			true,
			true
		);
	}
	else //if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cube overlapped with launchpad !"));

		OtherComp->AddImpulse
		(
			LaunchVelocity,
			"none",
			true
		);
	}
}
