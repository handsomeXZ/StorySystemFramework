


#include "SNode/SGraphNode_SDTOptions.h"

#include "Node/SDTGraphNode.h"
#include "Node/SDTGraphNode_DOptions.h"
#include "Node/SDTNode_DOptions.h"
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
#include "SLevelOfDetailBranchNode.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"



/////////////////////////////////////////////////////
// SGraphNode_SDTOptions
void SGraphNode_SDTOptions::Construct(const FArguments& InArgs, USDTGraphNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);

	this->UpdateGraphNode();
}

void SGraphNode_SDTOptions::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{

}

void SGraphNode_SDTOptions::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);

	const FMargin NodePadding = FMargin(2.0f);

	TSharedPtr<STextBlock> DescriptionText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
	TWeakPtr<SNodeTitle> WeakNodeTitle = NodeTitle;
	auto GetNodeTitlePlaceholderWidth = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredWidth = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().X : 0.0f;
		return FMath::Max(75.0f, DesiredWidth);
	};
	auto GetNodeTitlePlaceholderHeight = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredHeight = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().Y : 0.0f;
		return FMath::Max(22.0f, DesiredHeight);
	};

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot( ENodeZone::Center )
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage( FAppStyle::GetBrush( "Graph.StateNode.Body" ) )
			.Padding(0.0f)
			.BorderBackgroundColor( this, &SGraphNode_SDTOptions::GetBorderBackgroundColor )
			[
				SNew(SOverlay)

				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SHorizontalBox)

					// INPUT PIN AREA
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Top)
						[
							SAssignNew(LeftNodeBox, SVerticalBox)
						]
					]

					// STATE NAME AREA
					+SHorizontalBox::Slot()
					.Padding(NodePadding)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SAssignNew(NodeBody, SBorder)
							.BorderImage( FAppStyle::GetBrush("BTEditor.Graph.BTNode.Body") )
							.BorderBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.1f))
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.Visibility(EVisibility::SelfHitTestInvisible)
							[
								SNew(SVerticalBox)
									+SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SLevelOfDetailBranchNode)
											.UseLowDetailSlot(this, &SGraphNode_SDTOptions::UseLowDetailNodeTitles)
											.LowDetail()
											[
												SNew(SBox)
												.WidthOverride_Lambda(GetNodeTitlePlaceholderWidth)
												.HeightOverride_Lambda(GetNodeTitlePlaceholderHeight)
											]
											.HighDetail()
											[
												SNew(SHorizontalBox)
												+SHorizontalBox::Slot()
												.AutoWidth()
												.VAlign(VAlign_Center)
												[
													SNew(SImage)
													.Visibility(EVisibility::Collapsed)
													.Image(FAppStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon")))
												]
												+SHorizontalBox::Slot()
												.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
												[
													SNew(SVerticalBox)
													+SVerticalBox::Slot()
													.AutoHeight()
													[
														SAssignNew(InlineEditableText, SInlineEditableTextBlock)
														.Style( FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText" )
														.Text( NodeTitle.Get(), &SNodeTitle::GetHeadTitle )
														.OnVerifyTextChanged(this, &SGraphNode_SDTOptions::OnVerifyNameTextChanged)
														.OnTextCommitted(this, &SGraphNode_SDTOptions::OnNameTextCommited)
														.IsReadOnly( this, &SGraphNode_SDTOptions::IsNameReadOnly )
														.IsSelected(this, &SGraphNode_SDTOptions::IsSelectedExclusively)
													]
													+SVerticalBox::Slot()
													.AutoHeight()
													[
														NodeTitle.ToSharedRef()
													]
												]
											]
										]
									]
							]
						]
					]

					// OUTPUT PIN AREA
					+SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Fill)
					.Padding(0, 2.0f)
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Bottom)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
						]
					]
				]
			]
		];

	CreatePinWidgets();
	CreateOutputSideAddButton(RightNodeBox);
}

void SGraphNode_SDTOptions::CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox)
{
	FText Tmp = FText::FromString(TEXT("Add Option"));
	TSharedRef<SWidget> AddPinButton = AddPinButtonContent(FText::FromString(TEXT("")), Tmp);

	FMargin AddPinPadding = 2.0f;
	
	OutputBox->AddSlot()
		.VAlign(VAlign_Center)
		.Padding(AddPinPadding)
		[
			AddPinButton
		];
}

EVisibility SGraphNode_SDTOptions::IsAddPinButtonVisible() const
{
	EVisibility ButtonVisibility = EVisibility::Visible;//SGraphNode::IsAddPinButtonVisible();

	return ButtonVisibility;
}

void SGraphNode_SDTOptions::CreatePinWidgets()
{
	USDTGraphNode* MyNode = CastChecked<USDTGraphNode>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < MyNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = MyNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SSDTGraphPin, MyPin)
				.ToolTipText(this, &SGraphNode_SDTOptions::GetPinTooltip, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

FText SGraphNode_SDTOptions::GetPinTooltip(UEdGraphPin* GraphPinObj) const
{
	FText HoverText = FText::GetEmpty();

	check(GraphPinObj != nullptr);
	UEdGraphNode* OwningGraphNode = GraphPinObj->GetOwningNode();
	if (OwningGraphNode != nullptr)
	{
		FString HoverStr;
		OwningGraphNode->GetPinHoverText(*GraphPinObj, /*out*/HoverStr);
		if (!HoverStr.IsEmpty())
		{
			HoverText = FText::FromString(HoverStr);
		}
	}

	return HoverText;
}

void SGraphNode_SDTOptions::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillHeight(1.0f)
			.Padding(0.0f, 20.0f)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		TSharedPtr<SInlineEditableTextBlock> NodeText;

		RightNodeBox->AddSlot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillHeight(1.0f)
			.Padding(0, 2.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(NodeText, SInlineEditableTextBlock)
					.WrapTextAt(100.0f)
					.Justification(ETextJustify::Right)
					.Text_Lambda([this, PinToAdd](){
						return GetOptionsContent(PinToAdd);
					})
					.OnVerifyTextChanged(this, &SGraphNode_SDTOptions::OnVerifyNameTextChanged)
					.OnTextCommitted_Lambda([this, PinToAdd](const FText& InText, ETextCommit::Type CommitInfo){
						OnOptionTextCommit(InText, PinToAdd);
					})
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					PinToAdd
				]
			];

		NodeText->SetCursor(EMouseCursor::TextEditBeam);
		OutputPins.Add(PinToAdd);
	}

}

FReply SGraphNode_SDTOptions::OnAddPin()
{
	USDTGraphNode_DOptions* MyNode = CastChecked<USDTGraphNode_DOptions>(GraphNode);

	MyNode->Modify();

	MyNode->AddPinToNode();

	UpdateGraphNode();
	/*GraphNode->GetGraph()->NotifyGraphChanged();*/

	return FReply::Handled();
}

FSlateColor SGraphNode_SDTOptions::GetBorderBackgroundColor() const
{
	FLinearColor InactiveStateColor(0.08f, 0.08f, 0.08f);
	FLinearColor ActiveStateColorDim(0.4f, 0.3f, 0.15f);
	FLinearColor ActiveStateColorBright(1.f, 0.6f, 0.35f);
	/*return GetBorderBackgroundColor_Internal(InactiveStateColor, ActiveStateColorDim, ActiveStateColorBright);*/

	return InactiveStateColor;

}

void SGraphNode_SDTOptions::OnOptionTextCommit(const FText& InText, const TSharedRef<SGraphPin>& PinToAdd)
{
	int32 Index = OutputPins.IndexOfByKey(PinToAdd);

	USDTGraphNode_DOptions* MyNode = CastChecked<USDTGraphNode_DOptions>(GraphNode);

	if (Index >= 0 && MyNode)
	{
		Cast<USDTNode_DOptions>(MyNode->NodeInstance)->Options[Index].Content = InText;
	}

}

FText SGraphNode_SDTOptions::GetOptionsContent(const TSharedRef<SGraphPin>& PinToAdd) const
{
	int32 Index = OutputPins.IndexOfByKey(PinToAdd);

	USDTGraphNode_DOptions* MyNode = CastChecked<USDTGraphNode_DOptions>(GraphNode);
	const TArray<FSDTNode_DOptionItem>& Names = MyNode->GetOutputPinsName();
	if (Index >= 0 && Index < Names.Num())
	{
		return MyNode->GetOutputPinsName()[Index].Content;
	}
	else
	{
		return FText::FromString(TEXT("empty"));
	}
	
}
