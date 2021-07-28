#pragma once
#include<list>
#include"Geometry.h"

//��ՃN���X
struct HomingShot;
class Trail
{
private:
	HomingShot& owner_;
	std::list<Position2>history_;
public:
	Trail(HomingShot& owner);
	void Update(void);
	void Draw(void);
	void Clear(void);
};

