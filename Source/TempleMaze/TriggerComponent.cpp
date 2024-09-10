#include "TriggerComponent.h"

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    bool ShouldMove = CheckForRequiredActors();

    if (ShouldMove)
    {
        DisablePhysicsForAcceptableActors();
        Mover->SetShouldMove(true);
        UE_LOG(LogTemp, Warning, TEXT("Mover set to move: %s"), *GetOwner()->GetName());
    }
    else
    {
        Mover->SetShouldMove(false);
        UE_LOG(LogTemp, Warning, TEXT("Mover stopped moving: %s"), *GetOwner()->GetName());
    }
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

bool UTriggerComponent::CheckForRequiredActors() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    // Flags to track the presence of required actors
    bool FoundTag1 = false;
    bool FoundTag2 = false;
    bool FoundTag3 = false;

    // Check if a single actor has all three tags
    for (AActor* Actor : Actors)
    {
        bool HasTag1 = Actor->ActorHasTag(AcceptableActorTag1);
        bool HasTag2 = Actor->ActorHasTag(AcceptableActorTag2);
        bool HasTag3 = Actor->ActorHasTag(AcceptableActorTag3);
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");

        // If one actor has all three tags and is not grabbed
        if (HasTag1 && HasTag2 && HasTag3 && !IsGrabbed)
        {
            return true;
        }

        // Track the tags if they're found individually on separate actors
        if (HasTag1 && !IsGrabbed) FoundTag1 = true;
        if (HasTag2 && !IsGrabbed) FoundTag2 = true;
        if (HasTag3 && !IsGrabbed) FoundTag3 = true;
    }

    // Return true if all tags are found on separate actors and none are grabbed
    return FoundTag1 && FoundTag2 && FoundTag3;
}




void UTriggerComponent::DisablePhysicsForAcceptableActors()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    // Disable physics for all actors with acceptable tags
    for (AActor* Actor : Actors)
    {
        // Check for acceptable tags
        if (Actor->ActorHasTag(AcceptableActorTag1) || 
            Actor->ActorHasTag(AcceptableActorTag2) || 
            Actor->ActorHasTag(AcceptableActorTag3))
        {
            UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
            if (Component != nullptr)
            {
                Component->SetSimulatePhysics(false);
                // Attach the actor to this component
                Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
            }
        }
    }
}


