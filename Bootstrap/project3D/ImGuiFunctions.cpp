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
