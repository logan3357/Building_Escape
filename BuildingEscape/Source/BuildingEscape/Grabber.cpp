// copyrights Nthan Smith 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
    UE_LOG(LogTemp, Warning, TEXT("Grabber Reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
  
	//Get Player Viewpoint
    FVector PlayerViewpointLocation;
    
    FRotator PlayerViewpointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
    (
        OUT PlayerViewpointLocation,
                                                               
        OUT PlayerViewpointRotation
    );
    
   /* UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
            *PlayerViewpointLocation.ToString(),
            *PlayerViewpointRotation.ToString()
    )*/
    
    FVector LineTraceEnd = PlayerViewpointLocation + (PlayerViewpointRotation.Vector() * Reach);
    
    //Draw a red trace in the world to visualise
    DrawDebugLine(
        GetWorld(),
        PlayerViewpointLocation,
        LineTraceEnd,
        FColor(255, 0, 0),
        false,
        0.f,
        0.f,
        10.f
    );
    
    ///Setup Query Params
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
  
    //Ray-cast out to reach distance
    
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType
    (
    OUT Hit,
    PlayerViewpointLocation,
    LineTraceEnd,
    FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
    TraceParams
    );
    
    //See what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning,TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
    }
    
    
}

