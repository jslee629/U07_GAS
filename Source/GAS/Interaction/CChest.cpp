#include "CChest.h"
#include "Components/StaticMeshComponent.h"

ACChest::ACChest()
{
	//initialize variables
	MaxPitch = 110.f;

	//Create Components
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
}

void ACChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACChest::Interaction_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(MaxPitch, 0, 0));
}


