// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// D�finie la taille de la box, si FVector n'a qu'un param�tre les 3 coordonn�es ont cette valeurs = (200.f, 200.f, 200.f)
	OverlapComp->SetBoxExtent(FVector(200.f));

	RootComponent = OverlapComp;

	// Rend visible la box OverlapComp
	OverlapComp->SetHiddenInGame(false);

	// Permet de lier l'�v�nement OnComponentBeginOverlap � la m�thode HandleOverlap
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateAbstractDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.f);
	DecalComp->SetupAttachment(RootComponent);
}


void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr)
	{
		return;
	}

	if (MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlapped with extraction zone !"));
}