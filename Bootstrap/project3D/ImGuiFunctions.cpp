#include "ImGuiFunctions.h"

void ImGuiFunctions::ResetButton(std::string title, glm::vec3& vec3, glm::vec3 vec3Default, ImVec2 size, bool sameLine)
{
	if (sameLine)
	{
		ImGui::SameLine();
	}
	
	if (ImGui::Button(title.c_str(), size))
	{
		vec3 = vec3Default;
	}
	
	ImGui::NewLine();
}

void ImGuiFunctions::ResetButton(std::string title, glm::vec4& vec4, glm::vec4 vec4Default, ImVec2 size, bool sameLine)
{
	if (sameLine)
	{
		ImGui::SameLine();
	}

	if (ImGui::Button(title.c_str(), size))
	{
		vec4 = vec4Default;
	}
	
	ImGui::NewLine();
}

void ImGuiFunctions::ResetButton(std::string title, float& f, float fDefault, ImVec2 size, bool sameLine)
{
	if (sameLine)
	{
		ImGui::SameLine();
	}

	if (ImGui::Button(title.c_str(), size))
	{
		f = fDefault;
	}

	ImGui::NewLine();
}

void ImGuiFunctions::OpenSection(int spacingBeforeIndent, int spacingAfterIndent, bool indent, bool useSeperator)
{
	AddSpacings(spacingBeforeIndent);

	if (useSeperator)
	{
		ImGui::Separator();
	}

	ImGui::Indent();

	AddSpacings(spacingAfterIndent);
}

void ImGuiFunctions::CloseSection(int spacingBeforeUnindent, int spacingAfterUnindent, bool unindent, bool useSeperator)
{
	AddSpacings(spacingBeforeUnindent);

	ImGui::Unindent();
	
	if (useSeperator)
	{
		ImGui::Separator();
	}

	AddSpacings(spacingAfterUnindent);
}

void ImGuiFunctions::AddSpacings(int spacingCount)
{
	for (int i = 0; i < spacingCount; i++)
	{
		ImGui::Spacing();
	}
}
