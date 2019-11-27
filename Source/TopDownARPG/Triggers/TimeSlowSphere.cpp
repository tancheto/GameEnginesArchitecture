// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSlowSphere.h"
#include "TopDownARPGCharacter.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TopDownARPG.h"
#include "Math/Vector.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Sets default values
ATimeSlowSphere::ATimeSlowSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RootComponent = SphereComponent;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimeSlowSphere::OnOverlapIn);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATimeSlowSphere::OnOverlapOut);

	SphereComponent->SetGenerateOverlapEvents(true);

	OnOverlapParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	OnOverlapParticle->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATimeSlowSphere::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.5f);
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	TimerDel.BindUFunction(this, FName("PreDestructionOfSphere"));
	//Calling PreDestructrionOfSphere after 3 seconds
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 3.4f, false);
}

void ATimeSlowSphere::PreDestructionOfSphere()
{
	UE_LOG(LogTopDownARPG, Display, TEXT("ATimeSlowSphere::PreDestructrionOfSphere"));

	for (AActor* SlowActor : AffectedActors)
	{
		SlowActor->CustomTimeDilation = 1.f;
	}

	AffectedActors.Empty();
}

void ATimeSlowSphere::OnOverlapIn(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("ATimeSlowSphere::OverlapBegin"));

	if (IsValid(OnOverlapParticle))
	{
		OnOverlapParticle->ActivateSystem();
	}

	if (IsValid(Other))
	{
		Other->CustomTimeDilation = 0.5f;
		AffectedActors.Add(Other);
	}
}

void ATimeSlowSphere::OnOverlapOut(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("ATimeSlowSphere::OverlapEnd"));

	if (IsValid(OnOverlapParticle))
	{
		OnOverlapParticle->ActivateSystem();
	}

	if (IsValid(Other))
	{
		Other->CustomTimeDilation = 1.f;
		AffectedActors.Remove(Other);
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
	FVector NewScale = CurrentScale * 1000.0f * GetWorld()->GetDeltaSeconds();
	SetActorScale3D(NewScale);

	UE_LOG(LogTopDownARPG, Display, TEXT("CurrentScale: %s; NewScale: %s"),
		*CurrentScale.ToString(), *NewScale.ToString());
}



