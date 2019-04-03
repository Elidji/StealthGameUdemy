// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "Math/Vector.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	// G�n�ration du mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	// G�n�ration de l'overlap box
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// D�finie la taille de la box, si FVector n'a qu'un param�tre les 3 coordonn�es ont cette valeurs = (200.f, 200.f, 200.f)
	OverlapComp->SetBoxExtent(FVector(200.f));
	OverlapComp->SetupAttachment(RootComponent);

	DecalComp = CreateAbstractDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.f);
	DecalComp->SetupAttachment(RootComponent);

	// Permet de lier l'�v�nement OnComponentBeginOverlap � la m�thode HandleOverlap
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

	if (MyPawn)
	{
		// Permet de lancer le joueur dans la direction qu'il regarde
		// FVector LaunchVelocity = LPForce*(MyPawn->GetActorForwardVector());

		/// Permet de lancer le joueur dans la direction du launchpad 
		FVector LaunchVelocity = LPForce * (this->GetActorForwardVector() + LPAngle) ;

		MyPawn->LaunchCharacter
		(
			LaunchVelocity,
			true,
			false
		);
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlapped with launchpad !"));
}
