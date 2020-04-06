#include "ImGuiFunctions.h"

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

void ImGuiFunctions::DisplayError(std::string errorInfo, ImVec4 errorColour, bool showError)
{
	if (!showError) return;

	ImGui::Begin("Issues Detected");

	ImGui::TextColored(errorColour, errorInfo.c_str());

	ImGui::End();

	/*ImGui::BeginMainMenuBar();

	ImGui::TextColored(errorColour, errorInfo.c_str());

	ImGui::EndMainMenuBar();*/
}
