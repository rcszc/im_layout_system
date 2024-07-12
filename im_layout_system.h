// im_layout_system. RCSZ 2024_07_12.
// update: 2024_07_13, version 0.1

#ifndef __IM_LAYOUT_SYSTEM_H
#define __IM_LAYOUT_SYSTEM_H
#include "imgui.h"
#include "imgui_internal.h"

#ifdef EXT_IMGUI_MATHS
static inline ImVec2 operator+(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x + s.x, v.y + s.y); }
static inline ImVec2 operator+(const ImVec2& v, float s) { return ImVec2(v.x + s, v.y + s); }

static inline ImVec2 operator-(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x - s.x, v.y - s.y); }
static inline ImVec2 operator-(const ImVec2& v, float s) { return ImVec2(v.x - s, v.y - s); }
static inline ImVec2 operator-(float s, const ImVec2& v) { return ImVec2(s - v.x, s - v.y); }

static inline ImVec2 operator*(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x * s.x, v.y * s.y); }
static inline ImVec2 operator*(const ImVec2& v, float s) { return ImVec2(v.x * s, v.y * s); }

static inline ImVec2 operator/(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x / s.x, v.y / s.y); }
static inline ImVec2 operator/(const ImVec2& v, float s) { return ImVec2(v.x / s, v.y / s); }
static inline ImVec2 operator/(float s, const ImVec2& v) { return ImVec2(s / v.x, s / v.y); }

static inline ImVec2& operator+=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 + v2; }
static inline ImVec2& operator-=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 - v2; }
static inline ImVec2& operator*=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 * v2; }
static inline ImVec2& operator/=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 / v2; }

static inline ImVec4 operator+(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x + s.x, v.y + s.y, v.z + s.z, v.w + s.w); }
static inline ImVec4 operator+(const ImVec4& v, float s) { return ImVec4(v.x + s, v.y + s, v.z + s, v.w + s); }

static inline ImVec4 operator-(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x - s.x, v.y - s.y, v.z - s.z, v.w - s.w); }
static inline ImVec4 operator-(const ImVec4& v, float s) { return ImVec4(v.x - s, v.y - s, v.z - s, v.w - s); }
static inline ImVec4 operator-(float s, const ImVec4& v) { return ImVec4(s - v.x, s - v.y, s - v.z, s - v.w); }

static inline ImVec4 operator*(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x * s.x, v.y * s.y, v.z * s.z, v.w * s.w); }
static inline ImVec4 operator*(const ImVec4& v, float s) { return ImVec4(v.x * s, v.y * s, v.z * s, v.w * s); }

static inline ImVec4 operator/(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x / s.x, v.y / s.y, v.z / s.z, v.w / s.w); }
static inline ImVec4 operator/(const ImVec4& v, float s) { return ImVec4(v.x / s, v.y / s, v.z / s, v.w / s); }
static inline ImVec4 operator/(float s, const ImVec4& v) { return ImVec4(s / v.x, s / v.y, s / v.z, s / v.w); }

static inline ImVec4& operator+=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 + v2; }
static inline ImVec4& operator-=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 - v2; }
static inline ImVec4& operator*=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 * v2; }
static inline ImVec4& operator/=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 / v2; }
#endif

namespace ImLayout {
	float GetImGuiSpacingWidth();

	typedef ImVec2(*CalcAnchorPointFunc)(const ImVec2& pos, const ImVec2& size);

	ImVec2 CalcAnchorPoint_Default   (const ImVec2& pos, const ImVec2& size);
	ImVec2 CalcAnchorPoint_Center    (const ImVec2& pos, const ImVec2& size);
	ImVec2 CalcAnchorPoint_LowerLeft (const ImVec2& pos, const ImVec2& size);
	ImVec2 CalcAnchorPoint_LowerRight(const ImVec2& pos, const ImVec2& size);

	// parameter usage '%'.
	struct LayoutParameters {
		CalcAnchorPointFunc AnchorPoint;

		ImVec2 PositionRatio;
		ImVec2 SizeRatio;
		ImVec2 SizeMinLimit; // pixel_value.

		LayoutParameters() : 
			AnchorPoint(nullptr), PositionRatio(ImVec2()), SizeRatio(ImVec2()), SizeMinLimit(ImVec2()) 
		{}
		LayoutParameters(CalcAnchorPointFunc ptr, const ImVec2& pos, const ImVec2& sz, 
			const ImVec2& sz_min = ImVec2()) :
			AnchorPoint  (ptr),
			PositionRatio(pos),
			SizeRatio    (sz),
			SizeMinLimit (sz_min)
		{}
	};

	using CstringID = const char*;
	// context "LayoutBegin - ImGui::End".
	bool LayoutBegin(
		const LayoutParameters& layout, CstringID name, bool* p_open = (bool*)0, ImGuiWindowFlags flags = 0
	);
	// context "LayoutBeginChild - ImGui::EndChild".
	bool LayoutBeginChild(
		const LayoutParameters& layout, ImGuiID id, bool border = false, ImGuiWindowFlags extra_flags = 0
	);

	// "ImGui::SetCursorPos", return item_size, offset => pixel.
	ImVec2 LayoutCursorPos(const LayoutParameters& layout, const ImVec2& offset = ImVec2());
	ImVec2 LayoutCursorPos_ConstSize(
		const ImVec2& size, const LayoutParameters& layout, const ImVec2& offset = ImVec2()
	);

	using CstringText = const char*;
	void LayoutText       (const LayoutParameters& layout, ImGuiID id, CstringText text);
	void LayoutTextColored(const LayoutParameters& layout, ImGuiID id, CstringText text, const ImVec4& color);
}

#endif