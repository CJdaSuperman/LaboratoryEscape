// Copyright Christopher Beltran 2020


#include "GrabHandler.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

UGrabHandler::UGrabHandler()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabHandler::BeginPlay()
{
	Super::BeginPlay();

	SetPhysicsHandle();
	BindInputComponent();				
}

void UGrabHandler::SetPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle component not found for %s"), 
		*GetOwner()->GetName());
	}
}

void UGrabHandler::BindInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabHandler::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabHandler::Release);
}

void UGrabHandler::Grab()
{
	FHitResult Hit = GetPhysicsBodyInReach();
	AActor* ActorHit = Hit.GetActor();
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();	

	//if our Hit result was an Actor with a Physics Body, then grab it
	if(ActorHit)
	{
		
		if (!PhysicsHandle) { return; }
		
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
	}	
}

void UGrabHandler::Release()
{
	if (!PhysicsHandle) { return; }
	
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabHandler::GetPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayerViewPos(), GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	return Hit;
}

FVector UGrabHandler::GetPlayerReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabHandler::GetPlayerViewPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

void UGrabHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }

	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}