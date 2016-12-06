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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}
///look for attached Phisics Handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle)
    {
        //Physics handle was found
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s PhysicsHandle component not found"), *GetOwner()->GetName())
    }
}

///look for attached  input component (only appears at start)
void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input component found"))
        ///bind the input axis
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s Input component not found"), *GetOwner()->GetName())
    }
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
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
    return FHitResult();
}


void UGrabber::Grab()
{
     UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
    
    //try and reach any actors with collision body set
    GetFirstPhysicsBodyInReach();
    
    //if we hit something then attach a physics handle
    //TODO attach phuysics handle
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
    //release physics handle
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    //if the physics handle is attached
        //move the object we're holding
    
    
}

