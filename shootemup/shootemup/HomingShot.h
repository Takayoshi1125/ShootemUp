#pragma once
#include"Geometry.h"
#include"Trail.h"

struct Bullet {
	Position2 pos;//座標
	Vector2 vel;//速度
	bool isActive = false;//生きてるか〜？
	Position2 GetPos()const
	{
		//isActive = false;
		return pos;
	}
};

struct HomingShot :public Bullet
{
	HomingShot();
	Trail trail;//奇跡
};

