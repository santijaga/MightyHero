

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaperSprite.h"
#include "CollectionCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UCollectionCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	bool isActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	FString CardName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	UPaperSprite* SplashSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	UPaperSprite* CardSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	UClass* CardValue;

	UFUNCTION(BlueprintCallable, Category = Collection)
	void OnSelectItemFromCollection();

	UFUNCTION(BlueprintCallable, Category = Collection)
	void RefreshCardStatus();
};
