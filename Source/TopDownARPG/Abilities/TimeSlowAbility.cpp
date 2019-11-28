// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSlowAbility.h"
#include "Triggers/TimeSlowSphere.h"
#include "TopDownARPG.h"
#include "Engine/World.h"

void UTimeSlowAbility::Activate(AActor* Source)
{
	Super::Activate(Source);

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("UTimeSlowAbility::Activate IsValid(World) == false"));
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Source;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Source->GetActorLocation() + Source->GetActorForwardVector() * 800.0f;

	AActor* TimeSlowSphere = World->SpawnActor<AActor>(TimeSlowSphereClass, SpawnLocation, Source->GetActorRotation(), SpawnParameters);

	if (IsValid(TimeSlowSphere) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("UTimeSlowAbility::Activate IsValid(TimeSlowSphere) == false"));
		return;
	}
}


