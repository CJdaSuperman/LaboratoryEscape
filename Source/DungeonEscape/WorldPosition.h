#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldPosition.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UWorldPosition : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWorldPosition();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
