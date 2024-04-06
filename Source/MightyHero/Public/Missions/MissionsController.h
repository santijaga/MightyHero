

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionsController.generated.h"

class AMissionActor;

UCLASS()
class MIGHTYHERO_API AMissionsController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionsController();

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
	UFUNCTION(BlueprintCallable, Category = "Mission Data")
	FString GetMissionDescription();

	UFUNCTION(BlueprintCallable, Category="Mission Data")
	FString GetMissionName();

	void StartGameplay();

	/*
	* Missions List
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Missions List")
	TArray<TSubclassOf<AMissionActor>> MissionsList;

	/*
	* Properties
	*/
private:
	int32 NextMission = 0;
	AMissionActor* CurrentMission;
};
