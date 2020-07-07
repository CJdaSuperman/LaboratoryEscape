#include "WorldPosition.h"
#include "GameFramework/Actor.h"

UWorldPosition::UWorldPosition()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();
	FString ObjectPosition = GetOwner()->GetActorLocation().ToString();

	UE_LOG(LogTemp, Warning, TEXT("%s position in world is %s"), *ObjectName, *ObjectPosition);	

}

void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

