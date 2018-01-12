#pragma once


#include "IURoboSim.h"

#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RStaticMeshComponent.h"
#include "PxArticulation.h"
#include "PxArticulationLink.h"
#include "PxArticulationJoint.h"

#include "RArticulation.generated.h"

UCLASS()
class UROBOSIM_API ARArticulation : public AActor
{
	GENERATED_BODY()
protected:
	virtual void Tick(float DeltaTime) override;
public:
	ARArticulation();
	void MakeArticulation();

	physx::PxArticulation* articulation;
	physx::PxArticulationLink* link;
	physx::PxArticulationJoint* joint;

	USceneComponent* Root;
	USphereComponent* SphereComponent;

	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
	}
 
	FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
	}
};
