// copyrights Nthan Smith 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
    
    
    
    
private:
    //How Far ahead of the player can we reach in centimeters
    float Reach = 100.f;
    
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    
    UInputComponent* InputComponent = nullptr;
    // Ray-Casts and grabs what in reach
    void Grab();
    // reverses Grab
    void Release();
    
    //Find attached physics
    void FindPhysicsHandleComponent();
    
    //setup attached input component
    void SetupInputComponent();

    //Return hit for physics body in reach
    const FHitResult GetFirstPhysicsBodyInReach();
	
};
