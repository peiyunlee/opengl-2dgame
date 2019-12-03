#include "EnemyManager.h"
#include "Enemy.h"
#include "PBoat.h"
#include "Bullet.h"

EnemyManager::EnemyManager(mat4 &mxView, mat4 &mxProjection,int totCount_s,int totCount_m) {
	_state = LEVEL2;

	srand((unsigned)time(NULL));

	_totCount_s = _storeCount_s = totCount_s;
	_totCount_m = _storeCount_m = totCount_m;
	_totCount_b = _storeCount_b = 1;
	_storetotCount = _totCount_s + _totCount_m + _totCount_b;
	_usetotCount = 0;
	_useCount_s = 0;
	_useCount_m = 0;
	_useCount_b = 0;

	_killCount = 0;

	_timer  = 0.0f;
	_genDuration = RandomTime(1.0f,1.0f);
	_genMat = RandomPosition();

	//bulid Enemy
	pEHead_s = pETail_s = pEHead_m = pETail_m = NULL;
	pEHead_s = new EnemySmall(mxView, mxProjection);
	pETail_s = pEHead_s;
	for (int i = 1; i < _totCount_s; i++)
	{
		pENewGet = new EnemySmall(mxView, mxProjection);
		pENewGet->_prelink = pETail_s;
		pETail_s->_nextlink = pENewGet;
		pETail_s = pENewGet;
	}

	pEHead_m = new EnemyMiddle(mxView, mxProjection);
	pETail_m = pEHead_m;
	for (int i = 1; i < _totCount_m; i++)
	{
		pENewGet = new EnemyMiddle(mxView, mxProjection);
		pENewGet->_prelink = pETail_m;
		pETail_m->_nextlink = pENewGet;
		pETail_m = pENewGet;
	}

	pEHead_b = new EnemyBoss(mxView, mxProjection);

	for (int i = 0; i < 5; i++)
	{
		_smokeDead[i] = new Smoke(mxView, mxProjection, Smoke::DEAD);
		_smokeHurt[i] = new Smoke(mxView, mxProjection, Smoke::HURT2);
		_isSmokeDeadUse[i] = false;
		_isSmokeHurtUse[i] = false;
	}
}

EnemyManager::~EnemyManager() {
	Clear();
}

void EnemyManager::GetComponent(PBoat *getPBoat) {
	_getPBoat = getPBoat;
}

void EnemyManager::Clear() {
	Enemy *pClearGet;

	for (int i = 0; i < _totCount_s; i++)
	{
		pClearGet = pEHead_s;
		if (pClearGet != NULL) {
			pEHead_s = pClearGet->_nextlink;
			delete pClearGet;
			i++;
		}
	}

	for (int i = 0; i < _totCount_m; i++)
	{
		pClearGet = pEHead_m;
		if (pClearGet != NULL) {
			pEHead_m = pClearGet->_nextlink;
			delete pClearGet;
			i++;
		}
	}

	if (pEHead_b != NULL) delete pEHead_b;

	for (int i = 0; i < 5; i++)
	{
		if(_smokeDead[i]!=NULL)
			delete _smokeDead[i];
		if (_smokeHurt[i] != NULL)
			delete _smokeHurt[i];
	}
}


bool isLeftEnemyDead = true;
bool isRightEnemyDead = true;
float leftKillTimer = 0.0f;
float rightKillTimer = 0.0f;

void EnemyManager::EnemyGenerater(char type, mat4 &mat) {

	Enemy *pNewEnemyGet,*pHead,*pTail;
	int *useCount,*storeCount,*totCount;
	pNewEnemyGet = pHead = pTail = NULL;
	useCount = storeCount = totCount = NULL;

	switch (type) {
	case 's':
		pHead = pEHead_s;
		pTail = pETail_s;
		useCount = &_useCount_s;
		storeCount= &_storeCount_s;
		totCount = &_totCount_s;
		break;
	case 'm':
		pHead = pEHead_m;
		pTail = pETail_m;
		useCount = &_useCount_m;
		storeCount = &_storeCount_m;
		totCount = &_totCount_m;
		break;
	case 'b':
		pHead = pEHead_b;
		pTail = pEHead_b;
		useCount = &_useCount_b;
		storeCount = &_storeCount_b;
		totCount = &_totCount_b;
		break;
	}

	if (_usetotCount == 0)	//當目前use槽沒東西，跟敵人池要敵人
	{
		pNewEnemyGet = pHead;
		pNewEnemyGet->_transform->_mxOri = mat;	//存取初始位置
		pEUseHead = pNewEnemyGet;
		if (*totCount != 1) {
			pHead = pHead->_nextlink;
			pHead->_prelink = NULL;	//敵人從敵人池移出
		}

		pNewEnemyGet->_prelink = NULL;
		pNewEnemyGet->_nextlink = NULL;
		pEUseTail = pNewEnemyGet;
		(*useCount)++;
		_usetotCount++;
		(*storeCount)--;
	}
	else
	{
		if (*useCount < *totCount) {
			pNewEnemyGet = pHead;	//跟敵人池要敵人
			if (type == 'm' && isLeftEnemyDead && !isRightEnemyDead) {
				pNewEnemyGet->_isDefaultEnemy = false;
			}
			pNewEnemyGet->_transform->_mxOri = mat;	//存取初始位置
			if (*useCount < *totCount - 1) {
				pHead = pHead->_nextlink;
				pHead->_prelink = NULL;	//敵人從敵人池移出
			}
			else if (pHead == pTail) {
				//store剩最後一個
				pHead = pTail = NULL;	//敵人從敵人池移出
			}
			pEUseTail->_nextlink = pNewEnemyGet;
			pNewEnemyGet->_prelink = pEUseTail;
			pNewEnemyGet->_nextlink = NULL;
			pEUseTail = pNewEnemyGet;
			(*useCount)++;
			_usetotCount++;
			(*storeCount)--;
		}
		else {
			printf("敵人子彈用盡\n");
		}
	}

	switch (type) {
	case 's':
		pEHead_s = pHead;
		pETail_s = pTail;
		break;
	case 'm':
		pEHead_m = pHead;
		pETail_m = pTail;
		break;
	}
}

void EnemyManager::Draw() {
	if (_usetotCount > 0) {
		Enemy *pDrawGet;
		pDrawGet = pEUseHead;
		for (int i = 0; i < _usetotCount; i++)
		{
			pDrawGet->Draw();
			pDrawGet = pDrawGet->_nextlink;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (_state != GameEnd) {
			if (_isSmokeHurtUse[i]) {
				_smokeHurt[i]->Draw();
			}
		}
		if (_isSmokeDeadUse[i]) {
			_smokeDead[i]->Draw();
		}
	}
}

void EnemyManager::PushTail() {
	Enemy *pGet,*pTail;
	pTail = NULL;
	pGet = pEUpdateGet;

	switch (pGet->_type)
	{
		case 's':
			pTail = pETail_s;
			break;
		case 'm':
			pTail = pETail_m;
			break;
		case 'b':
			pTail = pEHead_b;
			break;
	}

	pTail->_nextlink = pGet;
	pGet->_prelink = pTail;
	pGet->Reset();
	pTail = pGet;
	pTail->_nextlink = NULL;

	switch (pGet->_type)
	{
	case 's':
		pETail_s = pTail;
		break;
	case 'm':
		pETail_m = pTail;
		break;
	case 'b':
		pTail = pEHead_b;
		break;
	}
}

void EnemyManager::DestroyEnemy()
{
	Enemy *pGetPre, *pGetNext;
	pEUpdateGet->_bulletList->ResetBulletList();
	if (pEUpdateGet == pEUseHead) {
		//如果消失的是顯示的第一個敵人(_prelink==NULL)
		if (pEUseHead == pEUseTail) //只剩一顆
			pEUseHead = pEUseTail = NULL;
		else
		{
			pEUseHead = pEUseHead->_nextlink;
			pEUseHead->_prelink = NULL;
		}

		PushTail();	//將子彈丟回子彈池
		pEUpdateGet = pEUseHead;
	}
	else if (pEUpdateGet == pEUseTail)
	{
		//如果消失的是顯示的最後一個敵人(_nextlink==NULL)
		//且一定是pUseHead != pUseTail 不只剩一顆
		pGetPre = pEUpdateGet->_prelink;
		pEUseTail = pGetPre;
		pEUseTail->_nextlink = NULL;
		PushTail();	//將子彈丟回子彈池
		pEUpdateGet = NULL;	//結束檢查
	}
	else {
		pGetPre = pEUpdateGet->_prelink;
		pGetNext = pEUpdateGet->_nextlink;

		pGetPre->_nextlink = pGetNext;
		pGetNext->_prelink = pGetPre;	//重新連結

		PushTail();	//將子彈丟回子彈池
		pEUpdateGet = pGetNext;
	}
}

void EnemyManager::Update(float dt) {
	_timer += dt;
	if(isLeftEnemyDead)
		leftKillTimer += dt;
	if (isRightEnemyDead)
		rightKillTimer += dt;
	//產生敵人
	EGeneratorController();

	//做畫面Enemy要做的事
	if (_usetotCount > 0) {
		pEUpdateGet = pEUseHead;
		int k = 0,ds = 0,dm = 0;
		for (int i = 0; i < _usetotCount; i++)
		{
			if (pEUpdateGet != NULL) {
				if (pEUpdateGet->_isDead) {
					//如果敵人死了，清除敵人
					switch (pEUpdateGet->_type)
					{
					case 's':
						ds++;
						_killCount++;
						Print(_killCount);
						break;
					case 'm':
						dm++;
						if (pEUpdateGet->_isDefaultEnemy)
							isRightEnemyDead = true;
						else
							isLeftEnemyDead = true;
						break;
					case 'b':
						_state = GameEnd;
						printf("GameWin\n");
						break;
					}

					for (int i = 0; i < 5; i++)
					{
						if (!_isSmokeDeadUse[i]) {
							_isSmokeDeadUse[i] = true;
							_smokeDeadPosition[i] = pEUpdateGet->_transform->_mxTRS;
							_smokeDead[i]->Show(Smoke::DEAD);
							i = 5;
						}
					}

					DestroyEnemy();
					k++;
				}
				else if (pEUpdateGet->_type != 'b' && (pEUpdateGet->_transform->_mxTRS._m[1].w >= 2.6f || pEUpdateGet->_transform->_mxTRS._m[1].w <= -2.6f)) {
					//如果敵人非BOSS且碰到邊界，清除敵人
					switch (pEUpdateGet->_type)
					{
					case 's':
						ds++;
						break;
					case 'm':
						dm++;
						break;
					}

					for (int i = 0; i < 5; i++)
					{
						if (!_isSmokeDeadUse[i]) {
							_isSmokeDeadUse[i] = true;
							_smokeDeadPosition[i] = pEUpdateGet->_transform->_mxTRS;
							_smokeDead[i]->Show(Smoke::DEAD);
							i = 5;
						}
					}

					DestroyEnemy();	//如果碰到邊界要刪除
					k++;
				}
				else if (pEUpdateGet->_ftottime > 0.0f && _colliSystem.OnBoxCollision(pEUpdateGet->_transform->_mxTRS, pEUpdateGet->_colliderSize, _getPBoat->_transform->_mxTRS, _getPBoat->_colliderSize)) {
					//如果敵人碰到玩家，NORMAL或DEFENSE都可以
					if (_getPBoat->_playerState == PBoat::NORMAL) {
						//玩家那邊會自動判斷是否是DEFENSE不扣血
						_getPBoat->Hurt();
					}
					//else if (_getPBoat->_playerState == PBoat::DEFENSE && pEUpdateGet->_type != 'b') {
					//	pEUpdateGet->Hurt();
					//	Print("enemy touch true");
					//}

					pEUpdateGet->Action(dt, _getPBoat);	//攻擊、move

					if (pEUpdateGet != pEUseTail)
						pEUpdateGet = pEUpdateGet->_nextlink;
				}
				else {
					pEUpdateGet->Action(dt,_getPBoat);	//攻擊、move
					if (pEUpdateGet != pEUseTail)
						pEUpdateGet = pEUpdateGet->_nextlink;
				}
			}
		}

		_usetotCount -= k;
		_useCount_s -= ds;
		_useCount_m -= dm;
		_storetotCount += k;
		_storeCount_s += ds;
		_storeCount_m += dm;

	}

	for (int i = 0; i < 5; i++)
	{
		if (_state != GameEnd) {
			if (_isSmokeHurtUse[i]) {
				_smokeHurt[i]->Update(dt, _smokeHurtPosition[i]);
				if (_smokeHurt[i]->_smokeState == Smoke::NONE) {
					_isSmokeHurtUse[i] = false;
				}
			}
		}
		if (_isSmokeDeadUse[i]) {
			_smokeDead[i]->Update(dt, _smokeDeadPosition[i]);
			if (_smokeDead[i]->_smokeState == Smoke::NONE) {
				_isSmokeDeadUse[i] = false;
			}
		}
	}
}

float EnemyManager::RandomTime(float min, float max) {
	float result;

	/* 產生 [min , max) 的浮點數亂數 */
	result = (max - min) * rand() / (RAND_MAX + 1.0f) + min;

	return result;
}

mat4 EnemyManager::RandomPosition(float minX, float maxX, float Y) {
	float x,y;
	mat4 result;

	/* 產生 [min , max) 的浮點數亂數 */
	x = (maxX - minX) * rand() / (RAND_MAX + 1.0f) + minX;
	y = Y;

	result._m[0].w = x;
	result._m[1].w = y;
	return result;
}

bool flag=false;

void EnemyManager::EGeneratorController(){
	switch (_state)
	{
		case LEVEL1:
			if (_timer > _genDuration) {
				_timer = 0.0f;
				EnemyGenerater('s', _genMat);
				_genDuration = RandomTime();
				_genMat = RandomPosition();
			}

			if (_killCount >= 5) {
				GoLevel(LEVEL2);
			}
			break;
		case LEVEL2:
			if (_timer > _genDuration && !flag) {
				flag = true;
				_genMat._m[0].w = 2.5f;
				_genMat._m[1].w = 0.0f;
				EnemyGenerater('m', _genMat);
				isRightEnemyDead = false;
				_genMat._m[0].w = -2.5f;
				_genMat._m[1].w = 0.0f;
				EnemyGenerater('m', _genMat);
				isLeftEnemyDead = false;
				_genDuration = 8.0f;
			}
			else if (_useCount_m < 2 && flag) {
				if(isLeftEnemyDead && leftKillTimer>= 3.0f){
					_genMat._m[0].w = -2.5f;
					_genMat._m[1].w = 0.0f;
					EnemyGenerater('m', _genMat);
					isLeftEnemyDead = false;
					leftKillTimer = 0.0f;
				}
				else if(isRightEnemyDead && rightKillTimer >= 3.0f) {
					_genMat._m[0].w = 2.5f;
					_genMat._m[1].w = 0.0f;
					EnemyGenerater('m', _genMat);
					isRightEnemyDead = false;
					rightKillTimer = 0.0f;
				}
			}

			if (_timer > _genDuration && flag) {
				_timer = 0.0f;
				_genDuration = 4.0f;
				_genMat = RandomPosition(-0.8f,0.8f);
				EnemyGenerater('s', _genMat);
			}

			if (_killCount >= 5) {
				GoLevel(LEVEL3);
			}

			break;
		case LEVEL3:
			if (_timer > _genDuration && !flag) {
				flag = true;
				_genMat = RandomPosition(0.0f,0.0f,3.0f);
				EnemyGenerater('b', _genMat);
			}
			break;
		case GameEnd:
			//清掉畫面上的敵人
			if (_usetotCount > 0) {
				pEUpdateGet = pEUseHead;
				int k = 0, ds = 0, dm = 0;
				for (int i = 0; i < _usetotCount; i++)
				{
					if (pEUpdateGet != NULL) {
						switch (pEUpdateGet->_type)
						{
						case 's':
							ds++;
							break;
						case 'm':
							dm++;
							if (pEUpdateGet->_isDefaultEnemy)
								isRightEnemyDead = true;
							else
								isLeftEnemyDead = true;
							break;
						}

						for (int i = 0; i < 5; i++)
						{
							if (!_isSmokeDeadUse[i]) {
								_isSmokeDeadUse[i] = true;
								_smokeDeadPosition[i] = pEUpdateGet->_transform->_mxTRS;
								_smokeDead[i]->Show(Smoke::DEAD);
								i = 5;
							}
						}

						DestroyEnemy();
						k++;

						if (pEUpdateGet != pEUseTail)
							pEUpdateGet = pEUpdateGet->_nextlink;
					}
				}

				_usetotCount -= k;
				_useCount_s -= ds;
				_useCount_m -= dm;
				_storetotCount += k;
				_storeCount_s += ds;
				_storeCount_m += dm;
			}
			break;

			for (int i = 0; i < 5; i++)
			{
				_isSmokeHurtUse[i] = false;
				_smokeHurt[i]->Reset(Smoke::HURT2);
			}
		default:
			break;
		}
}

void EnemyManager::GoLevel(int level) {

	_killCount = 0;
	flag = false;
	_timer = 0.0f;

	switch (level)
	{
	case LEVEL1:
		_state = LEVEL1;
		_genDuration = 1.0f;
		break;
	case LEVEL2:
		_state = LEVEL2;
		_genDuration = 2.0f;
		isRightEnemyDead = true;
		isLeftEnemyDead = true;
		break;
	case LEVEL3:
		_state = LEVEL3;
		_genDuration = 2.0f;
		break;
	default:
		break;
	}
}