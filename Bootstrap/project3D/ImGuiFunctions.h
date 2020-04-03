#pragma once
#include "imgui.h"
#include "glm/ext.hpp"
#include <string>

static class ImGuiFunctions
{
public:
	static void ResetButton(std::string title, glm::vec3& vec3, glm::vec3 vec3Default, ImVec2 size, bool sameLine = true);
	static void ResetButton(std::string title, glm::vec4& vec4, glm::vec4 vec4Default, ImVec2 size, bool sameLine = true);
	static void ResetButton(std::string title, float& f, float floatDefault, ImVec2 size, bool sameLine = true);

	static void OpenSection(int spacingBeforeIndent, int spacingAfterIndent, bool indent = true, bool useSeperator = true);
	static void CloseSection(int spacingBeforeUnindent, int spacingAfterUnindent, bool unindent = true, bool useSeperator = true);

	static void AddSpacings(int spacingCount);
};

