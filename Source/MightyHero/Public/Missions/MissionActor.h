

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionActor.generated.h"

UCLASS()
class MIGHTYHERO_API AMissionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Accessors)
	FString GetDescription();

	UFUNCTION(BlueprintCallable, Category = Accessors)
	FString GetName();

	/*
	* Properties
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	FString Name;
};
