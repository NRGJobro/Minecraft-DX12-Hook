#pragma once
#include <algorithm>
#define PI 3.14159265359

//Visuals
#include "../Modules/Visual/ClickGui.h"
#include <vector>
#include "../Modules/Module.h"
#include <string>

class ModuleHandler {
public:
    std::vector<Module*> modules;
    struct CompareArg { bool operator()(Module* mod1, Module* mod2) { return mod1->name < mod2->name; } };

    void InitModules() {
        std::string visual = "Visual"; // Visuals

        modules.push_back(new ClickGui(visual));
        
        // Sort modules
        std::sort(modules.begin(), modules.end(), CompareArg());
    }
};