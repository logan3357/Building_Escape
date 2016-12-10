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
    if (PhysicsHandle == nullptr)
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
    //Ray-cast out to reach distance
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType
    (
     OUT Hit,
     GetReachStart(),
     GetReachEnd(),
     FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
     TraceParams
     );
    return Hit;
}


void UGrabber::Grab()
{
    //try and reach any actors with collision body set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    //if we hit something then attach a physics handle
    if(ActorHit)
    {
    if(!PhysicsHandle) {return;}
    //attach physics handle
    PhysicsHandle->GrabComponent(
            ComponentToGrab,
            NAME_None,//no bones needed
            ComponentToGrab->GetOwner()->GetActorLocation(),
            true//rotation
        );
    }
}

void UGrabber::Release()
{
    if(!PhysicsHandle) {return;}
    PhysicsHandle->ReleaseComponent();
}
FVector UGrabber::GetReachEnd()
{
    //Get Player Viewpoint
    FVector PlayerViewpointLocation;
    
    FRotator PlayerViewpointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
    (
     OUT PlayerViewpointLocation,
     
     OUT PlayerViewpointRotation
     );
    
    return PlayerViewpointLocation + (PlayerViewpointRotation.Vector() * Reach);
}


FVector UGrabber::GetReachStart()
{
    //Get Player Viewpoint
    FVector PlayerViewpointLocation;
    
    FRotator PlayerViewpointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
    (
     OUT PlayerViewpointLocation,
     
     OUT PlayerViewpointRotation
     );
    
    return PlayerViewpointLocation;
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    GetReachEnd();
    if(!PhysicsHandle) {return;}
    //if the physics handle is attached
    if(PhysicsHandle->GrabbedComponent)
    {
        //move the object we're holding
        PhysicsHandle->SetTargetLocation(GetReachEnd());
    }
    
    
}

