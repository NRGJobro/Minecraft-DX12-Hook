#pragma once
#include "../imgui/imgui.h"
#include <vector>
struct Particle
{
    ImVec2 pos;
    ImVec2 velocity;
    void updatePos()noexcept;
    void draw()noexcept;
};
std::vector<Particle> createDotMatrix(const ImVec2 maxPos, const ImVec2 velo, int num)noexcept;
void drawDotMatrix(std::vector<Particle>& vec, float maxDist, float colSpeed, bool trippy)noexcept;
void updateDotMatrix(const ImVec2 maxPos, std::vector<Particle>& vec)noexcept;