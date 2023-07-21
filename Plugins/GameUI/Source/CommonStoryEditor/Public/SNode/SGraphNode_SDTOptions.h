#pragma once

#include "Containers/Array.h"
#include "Internationalization/Text.h"
#include "SGraphNode.h"
#include "SGraphPin.h"
#include "Styling/SlateColor.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SGraphPin;
struct FGraphInformationPopupInfo;
struct FNodeInfoContext;

class SGraphNode_SDTOptions : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_SDTOptions) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class USDTGraphNode* InNode);

	// SNodePanel::SNode interface
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	// End of SNodePanel::SNode interface

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;
	virtual EVisibility IsAddPinButtonVisible() const override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual FReply OnAddPin() override;
	// End of SGraphNode interface

	FText GetPinTooltip(UEdGraphPin* GraphPinObj) const;
	FSlateColor GetBorderBackgroundColor() const;
	FText GetOptionsContent(const TSharedRef<SGraphPin>& PinToAdd) const;
	void OnOptionTextCommit(const FText& InText, const TSharedRef<SGraphPin>& PinToAdd);

	TSharedPtr<SHorizontalBox> OutputPinBox;
	/** The node body widget, cached here so we can determine its size when we want ot position our overlays */
	TSharedPtr<SBorder> NodeBody;
};
