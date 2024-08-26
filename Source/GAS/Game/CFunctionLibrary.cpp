#include "CFunctionLibrary.h"
#include "Components/CAttributeComponent.h"

bool UCFunctionLibrary::ApplyDamage(AActor* DamageCausor, AActor* TargetActor, float DamageAmount)
{
    UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttributes(TargetActor);
    if (AttributeComp)
    {
        return AttributeComp->ApplyHealthChange(DamageCausor, -DamageAmount);
    }

    return false;
}

bool UCFunctionLibrary::ApplyDirectionDamage(AActor* DamageCausor, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
    if (ApplyDamage(DamageCausor, TargetActor, DamageAmount))
    {
        UPrimitiveComponent* HitComp = HitResult.GetComponent();
        if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
        {
            HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
        }

        return true;
    }

    return false;
}
