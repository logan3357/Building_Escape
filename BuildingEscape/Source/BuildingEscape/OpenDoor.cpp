// copyrights Nthan Smith 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties

UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
    
	
}

void UOpenDoor::OpenDoor()
{
//Set the door rotation
    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
    //Set the door rotation
    Owner->SetActorRotation(FRotator(0.f, 90.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// poll the trigger volume
     //if the ActorThatOpens
    if(PressurePlate)
    {
        if (PressurePlate->IsOverlappingActor(ActorThatOpens))
        {
            OpenDoor();
            LastDoorOpenTime = GetWorld()->GetTimeSeconds();
        }
    }
    
    //Check if its time to close the door
    if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay )
    {
        CloseDoor();
    }
}

