// Copyright Christopher Beltran 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UGrabHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabHandler();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Allows player to grab a Physics object. A public function in case of creating an InputHandler script in the future
	void Grab();

	//Allows player to release a Physics object. A public function in case of creating an InputHandler script in the future
	void Release();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	//Sets PhysicsHandle member variable
	void SetPhysicsHandle();

	//Sets InputComponent member variable and binds keys
	void BindInputComponent();

	//Ray Traces a Physics object and returns the object hit
	FHitResult GetPhysicsBodyInReach() const;

	//Returns the endpoint of the Ray Trace
	FVector GetPlayerReach() const;

	//Returns the PlayerViewPoint location
	FVector GetPlayerViewPos() const;
};
