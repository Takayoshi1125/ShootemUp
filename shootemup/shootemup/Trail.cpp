#include<DxLib.h>
#include"HomingShot.h"
#include "Trail.h"

constexpr size_t history_limit = 10;

Trail::Trail(const HomingShot& owner):
	owner_(owner)
{

}

void Trail::SetHandle(int handle)
{
	handle_=handle;
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
	float thickness = 5.0f;
	auto lastPos = owner_.pos;
	if (history_.empty())return;
	float div = 1.0f / static_cast<float>(history_.size());
	float u = 0.0f;

	for (const auto& pos : history_)
	{
		if (lastPos==pos)continue;

		auto v = pos-lastPos;
		v.Normalize();
		v = Vector2(-v.y, v.x);

		auto p1 = lastPos + v * 16;
		auto p2 = pos+v*16;
		auto p3 = pos-v*16;
		auto p4 = lastPos - v * 16;

		DrawRectModiGraph(
			p1.x, p1.y,
			p2.x, p2.y,
			p3.x, p3.y,
			p4.x, p4.y,
			u*256,0,
			div*256,64,
			handle_,true);

		u += div;

		/*DrawLineAA(lastPos.x, lastPos.y,
			pos.x, pos.y, 0xffffff, thickness);
		thickness *= 0.90f;*/
		lastPos = pos;
	}
}

void Trail::Clear(void)
{
	history_.clear();
}
