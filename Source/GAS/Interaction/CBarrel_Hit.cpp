#include "CBarrel_Hit.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"

ACBarrel_Hit::ACBarrel_Hit()
{
	//Create Components
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->Radius = 250.f;
	ForceComp->SetAutoActivate(false);
	ForceComp->ImpulseStrength = 200000.f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	ForceComp->AddCollisionChannelToAffect(ECC_Pawn);
}

void ACBarrel_Hit::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComp->OnComponentHit.AddDynamic(this, &ACBarrel_Hit::OnActorHit);
}

void ACBarrel_Hit::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	if (ensure(Explosion))
	{
		FVector Location = GetActorLocation();
		Location.Z += 100.f;

		FTransform TM;
		TM.SetLocation(Location);
		TM.SetRotation(FQuat(GetActorRotation()));
		TM.SetScale3D(FVector(3));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, TM);
	}
}
