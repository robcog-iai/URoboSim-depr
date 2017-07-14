#include "ROSBridgeActor.h"

// Sets default values for this actor's properties
AROSBridgeActor::AROSBridgeActor(){
	UE_LOG(LogTemp, Log, TEXT("Actor Created. ")); 
}

// Called when the game starts or when spawned
void AROSBridgeActor::BeginPlay() {
	UE_LOG(LogTemp, Log, TEXT("Actor BeginPlay(). "));
}

// Called every frame
void AROSBridgeActor::Tick(float DeltaTime)  {
	UE_LOG(LogTemp, Log, TEXT("Actor Ticks. "));
}
