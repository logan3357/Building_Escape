// copyrights Nthan Smith 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT
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
    if (!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing Pressure Plate"), *GetOwner()->GetName())
    }
	Super::BeginPlay();
        Owner = GetOwner();
	
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
        if (GetTotalMassOfActorsOnPlate() > 30.f)
        {
            OpenDoor();
            LastDoorOpenTime = GetWorld()->GetTimeSeconds();
        }
    
    //Check if its time to close the door
    if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay )
    {
        CloseDoor();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    //find all the ovelapping actors
    TArray<AActor*> OverlappingActors;
    if(!PressurePlate) {return TotalMass;}
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    
    //iterate them adding their masses
    for (const auto& Actor : OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s is on pressure plate"), *Actor->GetName())
    }
    
    
    return TotalMass;
}

