#include<DxLib.h>
#include<cmath>
#include<vector>
#include<memory>
#include"HomingShot.h"


///当たり判定関数
///@param posA Aの座標
///@param radiusA Aの半径
///@param posB Bの座標
///@param radiusB Bの半径
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//当たり判定を実装してください
	return (posA-posB).Magnitude()<=radiusA+radiusB;
}
using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("1916003_石橋雄貴");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//背景用
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	auto bulletH=LoadGraph("img/bullet.png");

	auto arrow = LoadGraph("img/arrow_short.png");

	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	//struct Bullet {
	//	Position2 pos;//座標
	//	Vector2 vel;//速度
	//	bool isActive = false;//生きてるか〜？
	//};

	//弾の半径
	float bulletRadius = 5.0f;

	//自機の半径
	float playerRadius = 10.0f;

	//適当に256個くらい作っとく
	Bullet bullets[256];

	HomingShot homingShots[16] = {};
	for (auto& shot : homingShots)
	{
		shot.trail.SetHandle(arrow);
	}

	Position2 enemypos(320,25);//敵座標
	Position2 playerpos(320, 400);//自機座標

	//自機弾
	Bullet shots[8] = {};

	unsigned int frame = 0;//フレーム管理用

	char keystate[256] = {};
	char lastkeystate[256] = {};
	bool isRight = false;
	bool isDebugMode = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;
	constexpr float player_shot_speed = 8.0f;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//背景
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);


		//プレイヤー
		if (keystate[KEY_INPUT_RIGHT]/*&&lastkeystate[]*/) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}

		if (keystate[KEY_INPUT_Z]&&!lastkeystate[KEY_INPUT_Z])
		{
			//弾発射
			int count = 0;
			for (auto& hs : homingShots)
			{
				if (!hs.isActive)
				{
					hs.isActive = true;
					hs.pos = playerpos;
					hs.vel =isRight ? Vector2(player_shot_speed,1.0) :
						Vector2(-player_shot_speed, 1.0);
					/*hs.vel.Normalize();
					hs.vel *= player_shot_speed;*/
					hs.trail.Clear();

					//Vector2(count==0?1.0f:-1.0f,1.0);
					isRight = !isRight;
					if (++count > 1)
					{
						break;
					}
					
				}
			}
		}

		//弾の更新・描画
		for (auto& hshot : homingShots)
		{
			if (!hshot.isActive)continue;
			{
				//敵狙いに
				constexpr bool isSimple = true;

				if (frame % 2 == 0)
				{
					hshot.trail.Update();
				}
				hshot.pos += hshot.vel;

				hshot.trail.Draw();

				if (isSimple)
				{
					hshot.vel = hshot.vel + (enemypos - hshot.pos).Normalized();
					hshot.vel = hshot.vel.Normalized() * player_shot_speed;
					/*auto nVelocity  =shot.vel.Normalized();
					auto nTarget = (enemypos - shot.pos).Normalized();
					auto dot = Dot(nVelocity, nTarget);
					auto cross = Cross(nVelocity, nTarget);
					auto angle = acos(dot);
					angle = std::fminf(angle, DX_PI_F / 24.0f);
					angle = cross >= 0.0f ? angle : -angle;
					angle = atan2(nVelocity.y, nVelocity.x) + angle;

					shot.vel = Vector2(cosf(angle), sinf(angle)) * player_shot_speed;*/
				}
				else
				{
					auto nShotvel = hshot.vel.Normalized();
					auto nToEnemyVec = (enemypos - hshot.pos).Normalized();

					float dot = Dot(nShotvel, nToEnemyVec);
					float c = acos(dot);
					float cross = Cross(nShotvel, nToEnemyVec);
				}

				/*DrawCircleAA(hshot.pos.x, hshot.pos.y,
					5.0f, 16, 0xff0000);*/

				/*if (hshot.pos.x < -10 || 650 < hshot.pos.x ||
					hshot.pos.y < -10 || 490 < hshot.pos.y)
				{
					hshot.isActive = false;
				}*/

				if ((enemypos - hshot.pos).SQMagnitude() < 900.0f)
				{
					hshot.isActive = false;
				}

				if (hshot.pos.x+16 < 0 || 640 < hshot.pos.x-16 ||
					hshot.pos.y+24 <0 || 480 < hshot.pos.y-24)
				{
					hshot.isActive = false;
				}
				/*if (IsHit(hshot.pos, 5.0f,enemypos,3))
				{
					hshot.isActive = false;
				}*/

			}
		}
		DrawCircleAA(enemypos.x, enemypos.y,30.0f,16, 0x0000ff, false,3.0f);

		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//自機の本体(当たり判定)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		//弾発射
		if (frame % 12 == 0) {
			for (auto& b : bullets) {
				if (!b.isActive) {
					//b.pos = enemypos;
					//b.vel = Vector2(0, 5);
					//b.isActive = true;
					break;
				}
			}
		}

		//弾の更新および表示
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			//弾の現在座標に弾の現在速度を加算してください
			
			float angle = 0.0f;
			//弾の角度をatan2で計算してください。angleに値を入れるんだよオゥ
			
			DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//弾の本体(当たり判定)
				DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			}

			//敵に接触
		//	if(())
			//弾を殺す
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//あたり！
			//↓のIsHitは実装を書いてません。自分で書いてください。
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//当たった反応を書いてください。
			}
		}

		//敵の表示
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//敵の本体(当たり判定)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}
		++frame;
		ScreenFlip();
		copy(begin(keystate), end(keystate), begin(lastkeystate));
	}

	DxLib_End();

	return 0;
}