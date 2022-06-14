#pragma once
#include "../../include/imgui/imgui.h"
#include "../Module.h"

class ClickGui : public Module {
public:
	ClickGui(std::string cat) : Module(cat, "ClickGui", "Renders a clickable menu", 'C') {}
};