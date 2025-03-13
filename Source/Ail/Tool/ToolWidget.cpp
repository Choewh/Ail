// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/ToolWidget.h"
#include "Tool/DroppedTool.h"

void UToolWidget::AddDroppedTool(ADroppedTool* NewTool)
{
	ReceiveAddDroppedTool();
	//DroppedToolDescVerticalBox->AddChild();
}

void UToolWidget::RemoveDroppedTool(ADroppedTool* NewTool)
{
	ReceiveRemoveDroppedTool();
}
