#ifndef _GAMEOBJECTMANAGER_H
#define _GAMEOBJECTMANAGER_H

#include "GameObject.h"
#include "Utils\TemplatedVectorMapManager.h"

class CGameObjectManager : public CTemplatedVectorMapManager<CGameObject>
{
private:
	CGameObject* m_Player;

public:
	CGameObjectManager();
	~CGameObjectManager();

	void SetPlayer(CGameObject* Player){ m_Player = Player; };
	CGameObject* GetPlayer() const{ assert(m_Player != nullptr); return m_Player; };
};

#endif