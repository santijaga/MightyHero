

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "CharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ACharacterBase : public APaperZDCharacter
{
	GENERATED_BODY()

private:
	float ForwardSpeed = .0f;

	class UCharacterMovementComponent* MovementComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay();
};
