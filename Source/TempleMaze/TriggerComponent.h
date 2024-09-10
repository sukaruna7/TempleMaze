#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLEMAZE_API UTriggerComponent : public UBoxComponent
{
    GENERATED_BODY()

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void SetMover(UMover* Mover);

public:
    UTriggerComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    FName AcceptableActorTag1;

    UPROPERTY(EditAnywhere)
    FName AcceptableActorTag2;

    UPROPERTY(EditAnywhere)
    FName AcceptableActorTag3;

    UMover* Mover;

    bool CheckForRequiredActors() const;
    void DisablePhysicsForAcceptableActors() ;
};
