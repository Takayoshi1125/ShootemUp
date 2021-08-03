#pragma once
#include"Geometry.h"
#include"Trail.h"

struct Bullet {
	Position2 pos;//À•W
	Vector2 vel;//‘¬“x
	bool isActive = false;//¶‚«‚Ä‚é‚©`H
	Position2 GetPos()const
	{
		//isActive = false;
		return pos;
	}
};

struct HomingShot :public Bullet
{
	HomingShot();
	Trail trail;//ŠïÕ
};

