#pragma once


#include "IURoboSim.h"
#include "RStaticMeshComponent.h"
#include "RController.generated.h"

UCLASS()
class UROBOSIM_API URController : public UObject
{
  GENERATED_BODY()
 public:
  URController()
    {
      //      UE_LOG(LogTemp, Log, TEXT("Create Controller %s"), *GetName());
    };

  ~URController()
    {
    };

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  URStaticMeshComponent* Caster;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  URStaticMeshComponent* Target;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  FQuat TargetOrientation;

  virtual void ShowThatCreated()
  {
    UE_LOG(LogTemp, Error, TEXT("Created"));
  };


  virtual void ControllComand(float DeltaTime){};
  virtual void InitController(){};

};

UCLASS()
class UROBOSIM_API URWheelController : public URController
{
  GENERATED_BODY()
 public:
  URWheelController(){};

  virtual void ControllComand(float DeltaTime);
  virtual void InitController();
};

UCLASS()
class UROBOSIM_API URRevoluteController : public URController
{
  GENERATED_BODY()
 public:

  URRevoluteController(){};
  virtual void ControllComand(float DeltaTime);

};

UCLASS()
class UROBOSIM_API URCasterController : public URController
{
  GENERATED_BODY()
    public:

    URCasterController(){};
  virtual void ControllComand(float DeltaTime);
  //virtual void InitController();
};

UCLASS()
class UROBOSIM_API URCasterOrientationController : public URController
{
  GENERATED_BODY()
    public:

    URCasterOrientationController(){};
  virtual void ControllComand(float DeltaTime);
  //virtual void InitController();
};


UCLASS()
class UROBOSIM_API URControllerFactory : public UObject
{
  GENERATED_BODY()
    public:

    URControllerFactory(){};
  URController* CreateController(FString Type, URStaticMeshComponent* Target);


};
