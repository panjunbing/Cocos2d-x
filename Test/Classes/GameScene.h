#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
	GameScene();
	~GameScene();
	void addMonster();
	void addBullet1();
	void addBullet2();

	// a selector callback
    void menuBackCallback(CCObject* pSender);
	void pause(CCObject* pSender);
	void resume(CCObject* pSender);

	void gameOver();
    
	void spriteMoveFinished(CCNode *sender);

	void collision(float dt);
	void ShowScore(float dt);
	void scrollBg(float dt);

	void GameLogic(float dt);
	void GameLogicaddBullet1(float dt);
	void GameLogicaddBullet2(float dt);

	void keyArrowClicked(int arrow);
	void ccTouchesBegan(cocos2d::CCSet *pTouches,cocos2d::CCEvent *pEvent);
	void ccTouchesMoved(cocos2d::CCSet *pTouches,cocos2d::CCEvent *pEvent);
    
	bool isPointActive;


	cocos2d::CCArray     *_monsters;
    cocos2d::CCArray     *_bullets;

	cocos2d::CCSprite    *player;
	cocos2d::CCLabelTTF  *pScore;

    cocos2d::CCMenu *pPause;
	cocos2d::CCMenu *pResume;
	//Ãªµã
	cocos2d::CCSprite *bgSprite1;  
    cocos2d::CCSprite *bgSprite2;

	cocos2d::CCSize visibleSize;
	cocos2d::CCPoint origin;
	int bgHeight;

	int score;
	int monsterSpeed;

    CREATE_FUNC(GameScene);
};

#endif // __HELLOWORLD_SCENE_H__
