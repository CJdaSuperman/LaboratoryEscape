// Copyright Christopher Beltran 2020

#include "DoorOpener.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFrameWork/PlayerController.h"
#include "GameFrameWork/Actor.h"

#define OUT

UDoorOpener::UDoorOpener()
{	
	PrimaryComponentTick.bCanEverTick = true;
}


void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has DoorOpener component, but no PressurePlate assigned!"), *GetOwner()->GetName());
	}		

	SetAudioComponent();
}

void UDoorOpener::SetAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Audio component not found for %s"),
			*GetOwner()->GetName());
	}
}


void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate) 
	{		
		if (TotalMassOfActors() >= MassToOpenDoor || PressurePlate->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			OpenDoor(DeltaTime);
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
		else
		{
			if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
			{
				CloseDoor(DeltaTime);
			}
		}
	}
}

float UDoorOpener::TotalMassOfActors() const
{
	if (!PressurePlate) { return 0.f; }
	
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for(AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UDoorOpener::OpenDoor(float DeltaTime)
{
	if (!AudioComponent) { return; }
	
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);

	FRotator Rotation = GetOwner()->GetActorRotation();

	Rotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(Rotation);

	CloseDoorSoundPlayed = false;
	if (!OpenDoorSoundPlayed)
	{
		AudioComponent->Play();
		OpenDoorSoundPlayed = true;
	}
}

void UDoorOpener::CloseDoor(float DeltaTime)
{
	if (!AudioComponent) { return; }
	
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);

	FRotator Rotation = GetOwner()->GetActorRotation();

	Rotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(Rotation);

	OpenDoorSoundPlayed = false;
	if (!CloseDoorSoundPlayed)
	{
		AudioComponent->Play();
		CloseDoorSoundPlayed = true;
	}
}