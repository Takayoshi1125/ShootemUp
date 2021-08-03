#include<DxLib.h>
#include"HomingShot.h"
#include "Trail.h"

constexpr size_t history_limit = 20;

Trail::Trail(HomingShot& owner):
	owner_(owner)
{

}

void Trail::SetHandle(int handle)
{
	handle;
}

void Trail::Update(void)
{
	history_.push_front(owner_.pos);
	if (history_.size() > history_limit)
	{
		history_.pop_back();
	}
}
 
void Trail::Draw(void)
{
	float thickness = 20.0f;
	auto lastPos = owner_.pos;
	for (const auto& pos : history_)
	{
		//if (pos ==lastPos)continue;
		DrawLineAA(lastPos.x, lastPos.y,
			pos.x, pos.y, 0xffffff, thickness);
		thickness *= 0.90f;
		lastPos = pos;
	}
}

void Trail::Clear(void)
{
	history_.clear();
}
