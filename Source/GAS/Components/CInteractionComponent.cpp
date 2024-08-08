#include "CInteractionComponent.h"
#include "DrawDebugHelpers.h"

UCInteractionComponent::UCInteractionComponent()
{

}


void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCInteractionComponent::PrimaryInteraction()
{
	AActor* OwnerActor = GetOwner();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRoation;
	OwnerActor->GetActorEyesViewPoint(EyeLocation, EyeRoation);

	FVector Start = EyeLocation;
	FVector End = EyeLocation + (EyeRoation.Vector() * 1000.f);

	TArray<FHitResult> Hits;

	float Radius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.f, 0, 2.f);
}
