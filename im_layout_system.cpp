// im_layout_system.
#define EXT_IMGUI_MATHS
#include "im_layout_system.h"

using namespace std;

#define IM_MIN_LIMIT(value, min) ((value) < (min) ? (min) : (value))
namespace ImLayout {
	inline void ValueMinLimit(ImVec2& value, const ImVec2& min) {
		value.x = IM_MIN_LIMIT(value.x, min.x);
		value.y = IM_MIN_LIMIT(value.y, min.y);
	}

	float GetImGuiSpacingWidth() {
		// imgui style item_spacing (x)width.
		return ImGui::GetStyle().ItemSpacing.x;
	}

	// 锚点计算 2024_07_12 :
	// 默认(左上角), 居中([x,y]中心), 左下角, 右下角.

	ImVec2 CalcAnchorPoint_Default   (const ImVec2& pos, const ImVec2& size) { return pos; }
	ImVec2 CalcAnchorPoint_Center    (const ImVec2& pos, const ImVec2& size) { return pos - size * 0.5f; }
	ImVec2 CalcAnchorPoint_LowerLeft (const ImVec2& pos, const ImVec2& size) { return ImVec2(pos.x, pos.y - size.y); }
	ImVec2 CalcAnchorPoint_LowerRight(const ImVec2& pos, const ImVec2& size) { return pos - size; }

	bool LayoutBegin(const LayoutParameters& layout, CstringID name, bool* p_open, ImGuiWindowFlags flags) {
		ImGuiWindowFlags TempFlags = flags | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

		// calc imgui window size & position.
		ImVec2 WindowSize = layout.SizeRatio * 0.01f * ImGui::GetIO().DisplaySize;
		ValueMinLimit(WindowSize, layout.SizeMinLimit);

		ImVec2 DrawPositionFmt = layout.PositionRatio * 0.01f * ImGui::GetIO().DisplaySize;
		ImVec2 WindowPosition  = layout.AnchorPoint(DrawPositionFmt, WindowSize);

		ImGui::SetNextWindowSize(WindowSize);
		ImGui::SetNextWindowPos(WindowPosition);
		
		// imgui window context_begin.
		return ImGui::Begin(name, p_open, TempFlags);
	}

	inline void WindowItemLayout(const LayoutParameters& layout, ImVec2* size, ImVec2* position) {
		// calc imgui draw_item size & position.
		*size = layout.SizeRatio * 0.01f * ImGui::GetWindowSize();
		ValueMinLimit(*size, layout.SizeMinLimit);

		*position = layout.PositionRatio * 0.01f * ImGui::GetWindowSize();
		*position = layout.AnchorPoint(*position, *size);
	}

	bool LayoutBeginChild(const LayoutParameters& layout, ImGuiID id, bool border, ImGuiWindowFlags extra_flags) {
		// calc imgui child_window size & position.
		ImVec2 WindowSize = {}, WindowPosition = {};
		WindowItemLayout(layout, &WindowSize, &WindowPosition);

		ImGui::SetCursorPos(WindowPosition);
		// imgui child_window context_begin.
		return ImGui::BeginChild(id, WindowSize, border, extra_flags);
	}

	ImVec2 LayoutCursorPos(const LayoutParameters& layout, const ImVec2& offset) {
		// calc imgui draw size & position.
		ImVec2 DrawItemSize = {}, DrawSizePosition = {};
		WindowItemLayout(layout, &DrawItemSize, &DrawSizePosition);

		ImGui::SetCursorPos(DrawSizePosition + offset);
		return DrawItemSize;
	}

	ImVec2 LayoutCursorPos_ConstSize(const ImVec2& size, const LayoutParameters& layout, const ImVec2& offset) {
		// calc imgui draw position.
		ImVec2 DrawPositionFmt = layout.PositionRatio * 0.01f * ImGui::GetWindowSize();
		ImVec2 DrawPosition    = layout.AnchorPoint(DrawPositionFmt, size);

		ImGui::SetCursorPos(DrawPosition + offset);
		return size;
	}

	inline ImVec2 TextScaleSizeCalc(CstringText text, float size) {
		ImGui::SetWindowFontScale(size);
		ImVec2 TextSize = ImGui::CalcTextSize(text);
		ImGui::SetWindowFontScale(1.0f);
		return TextSize;
	}

	void LayoutText(const LayoutParameters& layout, ImGuiID id, CstringText text) {
		// calc imgui draw_text(child_win) position.
		ImVec2 ScaleTextSize = TextScaleSizeCalc(text, layout.SizeRatio.x);
		LayoutCursorPos_ConstSize(ScaleTextSize, layout);
		
		// text scale child_window.
		ImGui::BeginChild(id, ScaleTextSize);
		ImGui::SetWindowFontScale(layout.SizeRatio.x);
		ImGui::Text(text);
		ImGui::EndChild();
	}

	void LayoutTextColored(const LayoutParameters& layout, ImGuiID id, CstringText text, const ImVec4& color) {
		// calc imgui draw_text(child_win) position.
		ImVec2 ScaleTextSize = TextScaleSizeCalc(text, layout.SizeRatio.x);
		LayoutCursorPos_ConstSize(ScaleTextSize, layout);

		// text scale child_window.
		ImGui::BeginChild(id, ScaleTextSize);
		ImGui::SetWindowFontScale(layout.SizeRatio.x);
		ImGui::TextColored(color, text);
		ImGui::EndChild();
	}
}