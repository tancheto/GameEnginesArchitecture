// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSlowSphere.h"
#include "TopDownARPGCharacter.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TopDownARPG.h"
#include "Math/Vector.h"
#include "GameFramework/Actor.h"

// Sets default values
ATimeSlowSphere::ATimeSlowSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SphereComponent->SetCollisionProfileName("OverlapAll");
	//SphereComponent->InitSphereRadius(48.0f);
	SphereComponent->SetGenerateOverlapEvents(true);

	//something else
	RootComponent = SphereComponent;

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATimeSlowSphere::OnOverlapIn);
	SphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ATimeSlowSphere::OnOverlapOut);

	OnOverlapParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	OnOverlapParticle->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATimeSlowSphere::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10);	
}

void ATimeSlowSphere::OnOverlapIn(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("ATimeSlowSphere::OverlapBegin"));

	if (IsValid(OnOverlapParticle))
	{
		OnOverlapParticle->ActivateSystem();
	}

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Other);
	if (IsValid(Character))
	{
		Other->CustomTimeDilation = 0.5f;
	}

}
void ATimeSlowSphere::OnOverlapOut(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("ATimeSlowSphere::OverlapEnd"));

	if (IsValid(OnOverlapParticle))
	{
		OnOverlapParticle->ActivateSystem();
	}

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Other);
	if (IsValid(Character))
	{
		Other->CustomTimeDilation = 2.0f;
	}
}

// Called every frame
void ATimeSlowSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimeSlowSphere::PostActorCreated()
{
	FVector CurrentScale = GetActorScale3D();
	FVector NewScale = CurrentScale * 300.0f * GetWorld()->GetDeltaSeconds();
	SetActorScale3D(NewScale);

	UE_LOG(LogTopDownARPG, Warning, TEXT("CurrentScale: %s; NewScale: %s"),
		*CurrentScale.ToString(), *NewScale.ToString());
}



