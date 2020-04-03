#pragma once
#include "imgui.h"
#include "glm/ext.hpp"
#include <string>

static class ImGuiFunctions
{
public:
	template <typename T>
	static void ResetButton(std::string title, T& value, T valueDefault, ImVec2 size, bool sameLine = true);

	static void OpenSection(int spacingBeforeIndent, int spacingAfterIndent, bool indent = true, bool useSeperator = true);
	static void CloseSection(int spacingBeforeUnindent, int spacingAfterUnindent, bool unindent = true, bool useSeperator = true);

	static void AddSpacings(int spacingCount);
};

template<typename T>
inline void ImGuiFunctions::ResetButton(std::string title, T& value, T valueDefault, ImVec2 size, bool sameLine)
{
	if (sameLine)
	{
		ImGui::SameLine();
	}

	if (ImGui::Button(title.c_str(), size))
	{
		value = valueDefault;
	}

	ImGui::NewLine();
}
