

#pragma once

#include "Containers/Array.h"
#include "Internationalization/Text.h"
#include "SGraphNode.h"
#include "Styling/SlateColor.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SGraphPin;
class USCTGraphNode_Transition;
struct FGraphInformationPopupInfo;
struct FNodeInfoContext;

class SGraphNode_Transition : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_Transition) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, USCTGraphNode_Transition* InNode);

	// SNodePanel::SNode interface
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	virtual TArray<FOverlayWidgetInfo> GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const override;
	virtual bool RequiresSecondPassLayout() const override { return true; }
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;
	// End of SNodePanel::SNode interface

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	// End of SGraphNode interface

	// SWidget interface
	void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	// End of SWidget interface

	// Calculate position for multiple nodes to be placed between a start and end point, by providing this nodes index and max expected nodes 
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;

	static FLinearColor StaticGetTransitionColor(USCTGraphNode_Transition* TransNode, bool bIsHovered);

protected:
	FSlateColor GetBorderBackgroundColor() const;
	FText GetPreviewCornerText() const;

	/** Get the visibility of the Priority Order overlay */
	EVisibility GetOrderVisibility() const;
	/** Get the text to display in the Priority Order overlay */
	FText GetOrderText() const;
	/** Get the tooltip for the Priority Order overlay */
	FText GetOrderTooltipText() const;
	/** Get the color to display for the Priority Order overlay. This changes on hover state of sibling nodes */
	FSlateColor GetOrderColor(bool bHovered) const;
	/** Handle hover state changing for the Priority Order widget - we use this to highlight sibling nodes */
	void OnOrderHoverStateChanged(bool bHovered);
private:
	FSlateColor GetTransitionColor() const;
	const FSlateBrush* GetTransitionIconImage() const;
private:
	/** Cache of the widget representing the previous node */
	mutable TWeakPtr<SNode> PrevNodeWidgetPtr;

	/** The widget we use to display the Priority Order of the node */
	TSharedPtr<SWidget> PriorityOrderOverlay;

};
