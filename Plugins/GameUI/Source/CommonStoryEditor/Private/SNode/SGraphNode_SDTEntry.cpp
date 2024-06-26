


#include "SNode/SGraphNode_SDTEntry.h"

#include "Node/SDTGraphNode.h"
#include "SNode/SSDTGraphPin.h"

#include "Delegates/Delegate.h"
#include "GenericPlatform/ICursor.h"
#include "HAL/PlatformCrt.h"
#include "Internationalization/Internationalization.h"
#include "Layout/Margin.h"
#include "Math/Color.h"
#include "Math/Vector2D.h"
#include "Misc/Attribute.h"
#include "Misc/Optional.h"
#include "SGraphPin.h"
#include "SNodePanel.h"
#include "SlotBase.h"
#include "Styling/AppStyle.h"
#include "Types/SlateEnums.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"



/////////////////////////////////////////////////////
// SGraphNode_SDTEntry

void SGraphNode_SDTEntry::Construct(const FArguments& InArgs, USDTGraphNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);

	this->UpdateGraphNode();
}

void SGraphNode_SDTEntry::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{

}

FSlateColor SGraphNode_SDTEntry::GetBorderBackgroundColor() const
{
	FLinearColor InactiveStateColor(0.08f, 0.08f, 0.08f);
	FLinearColor ActiveStateColorDim(0.4f, 0.3f, 0.15f);
	FLinearColor ActiveStateColorBright(1.f, 0.6f, 0.35f);

	return InactiveStateColor;
}

void SGraphNode_SDTEntry::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();


	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
		.Padding(0)
		.BorderBackgroundColor(this, &SGraphNode_SDTEntry::GetBorderBackgroundColor)
		[
			SNew(SOverlay)

			// PIN AREA
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.0f)
		[
			SAssignNew(RightNodeBox, SVerticalBox)
		]
		]
		];

	CreatePinWidgets();
}

void SGraphNode_SDTEntry::CreatePinWidgets()
{
	USDTGraphNode* SDTGraphNode = CastChecked<USDTGraphNode>(GraphNode);

	UEdGraphPin* CurPin = SDTGraphNode->GetOutputPin();
	if (!CurPin->bHidden)
	{
		TSharedPtr<SGraphPin> NewPin = SNew(SGraphSDTPinEntry, CurPin);

		this->AddPin(NewPin.ToSharedRef());
	}
}

void SGraphNode_SDTEntry::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	RightNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];
	OutputPins.Add(PinToAdd);
}

FText SGraphNode_SDTEntry::GetPreviewCornerText() const
{
	return NSLOCTEXT("SGraphNode_SDTEntry", "CornerTextDescription", "Entry point for Story Chapter Tree");
}
