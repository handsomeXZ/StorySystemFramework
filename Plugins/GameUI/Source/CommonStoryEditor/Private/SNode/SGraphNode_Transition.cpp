


#include "SNode/SGraphNode_Transition.h"

#include "Node/SCTGraphNode_Transition.h"
#include "Node/SCTNode_Transition.h"

#include "ConnectionDrawingPolicy.h"
#include "Containers/EnumAsByte.h"
#include "Delegates/Delegate.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "HAL/PlatformCrt.h"
#include "IDocumentation.h"
#include "Internationalization/Internationalization.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Layout/Geometry.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Attribute.h"
#include "SGraphPanel.h"
#include "SlotBase.h"
#include "Styling/AppStyle.h"
#include "Templates/Casts.h"
#include "Types/SlateEnums.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/STextBlock.h"



class SWidget;
class UEdGraphPin;
class UObject;
struct FPointerEvent;
struct FSlateBrush;

#define LOCTEXT_NAMESPACE "SGraphNode_Transition"

//////////////////////////////////////////////////////////////////////////

/** Widget for overlaying an Priority Order onto a node */
class SSCTPriorityOrder : public SCompoundWidget
{
public:
	/** Delegate event fired when the hover state of this widget changes */
	DECLARE_DELEGATE_OneParam(FOnHoverStateChanged, bool /* bHovered */);

	/** Delegate used to receive the color of the node, depending on hover state and state of other siblings */
	DECLARE_DELEGATE_RetVal_OneParam(FSlateColor, FOnGetIndexColor, bool /* bHovered */);

	SLATE_BEGIN_ARGS(SSCTPriorityOrder) {}
		SLATE_ATTRIBUTE(FText, Text)
		SLATE_EVENT(FOnHoverStateChanged, OnHoverStateChanged)
		SLATE_EVENT(FOnGetIndexColor, OnGetIndexColor)
	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs )
	{
		OnHoverStateChangedEvent = InArgs._OnHoverStateChanged;
		OnGetIndexColorEvent = InArgs._OnGetIndexColor;

		const FSlateBrush* IndexBrush = FAppStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Index"));

		ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				// Add a dummy box here to make sure the widget doesnt get smaller than the brush
				SNew(SBox)
				.WidthOverride(IndexBrush->ImageSize.X)
				.HeightOverride(IndexBrush->ImageSize.Y)
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(IndexBrush)
				.BorderBackgroundColor(this, &SSCTPriorityOrder::GetColor)
				.Padding(FMargin(4.0f, 0.0f, 4.0f, 1.0f))
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(InArgs._Text)
					.Font(FAppStyle::GetFontStyle("BTEditor.Graph.BTNode.IndexText"))
				]
			]
		];
	}

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		OnHoverStateChangedEvent.ExecuteIfBound(true);
		SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
	}

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
	{
		OnHoverStateChangedEvent.ExecuteIfBound(false);
		SCompoundWidget::OnMouseLeave(MouseEvent);
	}

	/** Get the color we use to display the rounded border */
	FSlateColor GetColor() const
	{
		if (OnGetIndexColorEvent.IsBound())
		{
			return OnGetIndexColorEvent.Execute(IsHovered());
		}

		return FSlateColor::UseForeground();
	}

private:
	/** Delegate event fired when the hover state of this widget changes */
	FOnHoverStateChanged OnHoverStateChangedEvent;

	/** Delegate used to receive the color of the node, depending on hover state and state of other siblings */
	FOnGetIndexColor OnGetIndexColorEvent;
};

/////////////////////////////////////////////////////
// SGraphNode_Transition

void SGraphNode_Transition::Construct(const FArguments& InArgs, USCTGraphNode_Transition* InNode)
{
	this->GraphNode = InNode;
	this->UpdateGraphNode();
}

void SGraphNode_Transition::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{
	
}

void SGraphNode_Transition::MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	// Ignored; position is set by the location of the attached Chapter nodes
}

void SGraphNode_Transition::PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const
{
	USCTGraphNode_Transition* TransNode = CastChecked<USCTGraphNode_Transition>(GraphNode);

	// Find the geometry of the Chapter nodes we're connecting
	FGeometry StartGeom;
	FGeometry EndGeom;

	int32 TransIndex = 0;
	int32 NumOfTrans = 1;

	USCTGraphNode* PrevNode = TransNode->GetPrevNode();
	USCTGraphNode* NextNode = TransNode->GetNextNode();
	if ((PrevNode != NULL) && (NextNode != NULL))
	{
		const TSharedRef<SNode>* pPrevNodeWidget = NodeToWidgetLookup.Find(PrevNode);
		const TSharedRef<SNode>* pNextNodeWidget = NodeToWidgetLookup.Find(NextNode);
		if ((pPrevNodeWidget != NULL) && (pNextNodeWidget != NULL))
		{
			const TSharedRef<SNode>& PrevNodeWidget = *pPrevNodeWidget;
			const TSharedRef<SNode>& NextNodeWidget = *pNextNodeWidget;

			StartGeom = FGeometry(FVector2D(PrevNode->NodePosX, PrevNode->NodePosY), FVector2D::ZeroVector, PrevNodeWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(NextNode->NodePosX, NextNode->NodePosY), FVector2D::ZeroVector, NextNodeWidget->GetDesiredSize(), 1.0f);

			TArray<USCTGraphNode_Transition*> Transitions;
			PrevNode->GetTransitionList(Transitions);

			Transitions = Transitions.FilterByPredicate([NextNode](const USCTGraphNode_Transition* InTransition) -> bool
				{
					return InTransition->GetNextNode() == NextNode;
				});

			TransIndex = Transitions.IndexOfByKey(TransNode);
			NumOfTrans = Transitions.Num();

			PrevNodeWidgetPtr = PrevNodeWidget;
		}
	}

	//Position Node
	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, TransIndex, NumOfTrans);
}

void SGraphNode_Transition::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	PriorityOrderOverlay = SNew(SSCTPriorityOrder)
		.ToolTipText(this, &SGraphNode_Transition::GetOrderTooltipText)
		.Visibility(this, &SGraphNode_Transition::GetOrderVisibility)
		.Text(this, &SGraphNode_Transition::GetOrderText)
		.OnHoverStateChanged(this, &SGraphNode_Transition::OnOrderHoverStateChanged)
		.OnGetIndexColor(this, &SGraphNode_Transition::GetOrderColor);

	this->ContentScale.Bind( this, &SGraphNode::GetContentScale );
	this->GetOrAddSlot( ENodeZone::Center )
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SNew(SImage)
				.Image( FAppStyle::GetBrush("Graph.TransitionNode.ColorSpill") )
				.ColorAndOpacity( this, &SGraphNode_Transition::GetTransitionColor )
			]
			+SOverlay::Slot()
			[
				SNew(SImage)
				.Image( this, &SGraphNode_Transition::GetTransitionIconImage )
			]
		];
}

TArray<FOverlayWidgetInfo> SGraphNode_Transition::GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const
{
	TArray<FOverlayWidgetInfo> Widgets;

	check(PriorityOrderOverlay.IsValid());

	FVector2D Origin(0.0f, 0.0f);

	FOverlayWidgetInfo Overlay(PriorityOrderOverlay);
	Overlay.OverlayOffset = FVector2D(WidgetSize.X - (PriorityOrderOverlay->GetDesiredSize().X * 0.1f), Origin.Y);
	Widgets.Add(Overlay);

	return Widgets;
}

FSlateColor SGraphNode_Transition::GetTransitionColor() const
{
	// Highlight the transition node when the node is hovered or when the previous Node is hovered
	USCTGraphNode_Transition* TransNode = CastChecked<USCTGraphNode_Transition>(GraphNode);
	return StaticGetTransitionColor(TransNode, (IsHovered() || (PrevNodeWidgetPtr.IsValid() && PrevNodeWidgetPtr.Pin()->IsHovered())));
}

const FSlateBrush* SGraphNode_Transition::GetTransitionIconImage() const
{
	return FAppStyle::GetBrush("Graph.TransitionNode.Icon");
}

FLinearColor SGraphNode_Transition::StaticGetTransitionColor(USCTGraphNode_Transition* TransNode, bool bIsHovered)
{
	//@TODO: Make configurable by styling
	const FLinearColor ActiveColor(1.0f, 0.4f, 0.3f, 1.0f);
	const FLinearColor HoverColor(0.724f, 0.256f, 0.0f, 1.0f);
	FLinearColor BaseColor(0.9f, 0.9f, 0.9f, 1.0f);

	return bIsHovered ? HoverColor : BaseColor;
}

void SGraphNode_Transition::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// Store Hover information in UGraphNode, 
	// so that it is convenient to obtain Hover information in other places than SGraphNode.
	USCTGraphNode_Transition* TransNode = CastChecked<USCTGraphNode_Transition>(GraphNode);
	if (UEdGraphPin* Pin = TransNode->GetInputPin())
	{
		GetOwnerPanel()->AddPinToHoverSet(Pin);
	}

	SGraphNode::OnMouseEnter(MyGeometry, MouseEvent);
}

void SGraphNode_Transition::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	USCTGraphNode_Transition* TransNode = CastChecked<USCTGraphNode_Transition>(GraphNode);
	if (UEdGraphPin* Pin = TransNode->GetInputPin())
	{
		GetOwnerPanel()->RemovePinFromHoverSet(Pin);
	}

	SGraphNode::OnMouseLeave(MouseEvent);
}

void SGraphNode_Transition::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	const float Height = 30.0f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	// Calculate node offset in the case of multiple transitions between the same two nodes
	// MultiNodeOffset: the offset where 0 is the centre of the transition, -1 is 1 <size of node>
	// towards the PrevStateNode and +1 is 1 <size of node> towards the NextStateNode.

	const float MutliNodeSpace = 0.2f; // Space between multiple transition nodes (in units of <size of node> )
	const float MultiNodeStep = (1.f + MutliNodeSpace); //Step between node centres (Size of node + size of node spacer)

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

//////////////////////////////////////////////////////////////////////////

EVisibility SGraphNode_Transition::GetOrderVisibility() const
{
	return EVisibility::Visible;
}

FText SGraphNode_Transition::GetOrderText() const
{
	USCTGraphNode_Transition* GraphNode_Transition = Cast<USCTGraphNode_Transition>(GraphNode);
	USCTNode_Transition* Node_Transition = Cast<USCTNode_Transition>(GraphNode_Transition->NodeInstance);
	return FText::AsNumber(Node_Transition->PriorityOrder);
}

FText SGraphNode_Transition::GetOrderTooltipText() const
{
	return LOCTEXT("PriorityOrderTooltip", "Priority Order: this shows the order in which Transition are executed.");
}

FSlateColor SGraphNode_Transition::GetOrderColor(bool bHovered) const
{
	static const FName HoveredColor("BTEditor.Graph.BTNode.Index.HoveredColor");
	static const FName DefaultColor("BTEditor.Graph.BTNode.Index.Color");

	return bHovered ? FAppStyle::Get().GetSlateColor(HoveredColor) : FAppStyle::Get().GetSlateColor(DefaultColor);
}

void SGraphNode_Transition::OnOrderHoverStateChanged(bool bHovered)
{
	
}

/////////////////////////////////////////////////////
#undef LOCTEXT_NAMESPACE