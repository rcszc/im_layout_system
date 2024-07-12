# im_layout_system

> update: 2024_07_13 RCSZ

```namespace ImLayout```

## Version 0.1 20240712

获取ImGui控件间距宽度
```cpp
float GetImGuiSpacingWidth();
```

布局参数结构体, 位置大小均使用百分比值 (%)
```cpp
struct LayoutParameters {
	CalcAnchorPointFunc AnchorPoint; // 锚点计算 (函数指针)

	ImVec2 PositionRatio; // 位置比例 (相对父级)
	ImVec2 SizeRatio;     // 大小比例 (相对父级)
	ImVec2 SizeMinLimit;  // 最小大小限制 (像素)

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
```

锚点计算函数
```cpp
typedef ImVec2(*CalcAnchorPointFunc)(const ImVec2& pos, const ImVec2& size);

CalcAnchorPoint_Default;   // 控件左上角 (默认)
CalcAnchorPoint_Center;    // 控件中心
CalcAnchorPoint_LowerLeft  // 控件左下角
CalcAnchorPoint_LowerRight // 控件右下角
```

布局窗口上下文: LayoutBegin - ImGui::End
> __父级:__ 系统窗口
```cpp
// params: layout (布局信息), name (窗口名称), ...(与"ImGui::Begin"相同)
bool LayoutBegin(const LayoutParameters& layout, CstringID name, bool* p_open = (bool*)0, ImGuiWindowFlags flags = 0);
```

布局子窗口上下文: LayoutBeginChild - ImGui::EndChild
> __父级:__ ImGui上下文窗口
```cpp
// params: layout (布局信息), id (窗口唯一ID), ...(与"ImGui::BeginChild"相同)
bool LayoutBeginChild(const LayoutParameters& layout, ImGuiID id, bool border = false, ImGuiWindowFlags extra_flags = 0);
```

布局(设置)绘制光标, 内部会调用"ImGui::SetCursorPos"
> __父级:__ ImGui窗口, ImGui子窗口
```cpp
// params: layout (布局信息), offset (像素偏移量)
// return: 绘制大小
ImVec2 LayoutCursorPos(const LayoutParameters& layout, const ImVec2& offset = ImVec2());
```

布局(设置)绘制光标, 固定大小, 内部会调用"ImGui::SetCursorPos"
> __父级:__ ImGui窗口, ImGui子窗口
```cpp
// params: size (控件预留大小), layout (布局信息), offset (像素偏移量)
// return: 绘制大小
ImVec2 LayoutCursorPos_ConstSize(const ImVec2& size, const LayoutParameters& layout, const ImVec2& offset = ImVec2());
```

布局文本绘制, 通过子窗口缩放不同文本大小 ("ImGui::SetWindowFontScale")
> __父级:__ ImGui窗口, ImGui子窗口
```cpp
// 字体缩放值设置 (常为 0.5f - 2.0f):
MyLayout.SizeRatio.x；

// params: layout (布局信息), id (子窗口唯一ID), text (文本)
void LayoutText(const LayoutParameters& layout, ImGuiID id, CstringText text);

// params: layout (布局信息), id (子窗口唯一ID), text (文本), color (文本颜色)
void LayoutTextColored(const LayoutParameters& layout, ImGuiID id, CstringText text, const ImVec4& color);
```

---

```END```