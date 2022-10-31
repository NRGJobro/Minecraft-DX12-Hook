#include "dotMatrix.h"

void Particle::updatePos()noexcept
{
	pos = { pos.x + ImGui::GetIO().DeltaTime * velocity.x,pos.y + ImGui::GetIO().DeltaTime * velocity.y };
}


ImU32 col = IM_COL32(120, 100, 125, 255);


void Particle::draw()noexcept
{
	ImGui::GetBackgroundDrawList()->AddCircleFilled(pos, 2, col);
}

std::vector<Particle> createDotMatrix(const ImVec2 maxPos, const ImVec2 velo, int num)noexcept
{
	std::vector<Particle> retVec;

	for (int i = 0; i < num; i++)
	{
		ImVec2 pos = ImVec2(rand() % (int)maxPos.x, rand() % (int)maxPos.y);
		ImVec2 vel;
		vel.x = (rand() % 2 == 0) ? (rand() % (int)velo.x) * 1.5 + 0.1 : -(rand() % (int)velo.x) * 1.5 - 0.1;
		vel.y = (rand() % 2 == 0) ? (rand() % (int)velo.y) * 1.5 + 0.1 : -(rand() % (int)velo.y) * 1.5 - 0.1;
		Particle dot = { pos,vel };
		retVec.push_back(dot);
	}
	return retVec;
}

float dist2D(ImVec2 v1, ImVec2 v2)noexcept
{
	return hypot(v1.x - v2.x, v1.y - v2.y);
}

void drawLines(std::vector<Particle>& vec, float maxDist, float colSpeed, bool trippy)noexcept
{
	static ImU32 col2 = IM_COL32(rand() % 240 + 10, rand() % 240 + 10, rand() % 240 + 10, 255);
	static std::vector<ImU32> colors;
	static float t = 0;
	static int oldVecSize = vec.size();

	if (t > 0.2 || t == 0 || oldVecSize != vec.size()) {


		t = 0.1;
		colors.clear();
		for (int i = 0; i < vec.size(); ++i)
		{
			colors.push_back(IM_COL32(rand() % 150 + 10, 0, rand() % 210 + 46, 255));
		}
		oldVecSize = vec.size();
	}

	t += colSpeed * ImGui::GetIO().DeltaTime;

	for (auto& particle : vec)
	{
		int ih = 0;
		for (auto other : vec)
		{
			if (particle.pos.x == other.pos.x && particle.pos.y == other.pos.y)
				continue;

			if (dist2D(particle.pos, other.pos) < maxDist)
			{
				auto list = ImGui::GetBackgroundDrawList();
				list->PathLineTo(particle.pos);
				list->PathLineTo(other.pos);
				list->PathStroke(colors[ih]);
			}
			ih++;
		}
	}
}

void drawDotMatrix(std::vector<Particle>& vec, float maxDist, float colSpeed, bool trippy)noexcept
{

	for (auto& dot : vec)
	{
		dot.draw();
	}
	drawLines(vec, maxDist, colSpeed, trippy);
}

void updateDotMatrix(const ImVec2 maxPos, std::vector<Particle>& vec)noexcept
{
	for (auto& dot : vec)
	{
		dot.updatePos();
		if (dot.pos.x > maxPos.x ||
			dot.pos.y > maxPos.y ||
			dot.pos.x < 0 ||
			dot.pos.y < 0)
		{
			dot.pos = ImVec2(rand() % (int)maxPos.x, rand() % (int)maxPos.y);
		}
	}
}