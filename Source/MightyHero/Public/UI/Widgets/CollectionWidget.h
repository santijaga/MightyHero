

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "CollectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UCollectionWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseCollection();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RefreshAllCards();

	UFUNCTION(BlueprintCallable, Category = "TEST")
	void LockAllCards();

	UFUNCTION(BlueprintCallable, Category = "TEST")
	void AddMoreCoins();
};
