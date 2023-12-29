#include "UserWidgetBase.h"

void UUserWidgetBase::ShowWidget()
{
	this->AddToViewport();
}

void UUserWidgetBase::RemoveWidget()
{
	this->RemoveFromParent();
}