#include "GameScene.h"
#include "HelloWorldScene.h"
#include "PopGameOverScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//响应键盘事件
	this->setKeypadEnabled(true);
	//响应触摸事件
	this->setTouchEnabled(true);

	//敌方飞机和子弹的数组
	this->_monsters = CCArray::create();
	this->_monsters->retain();
	this->_bullets = CCArray::create();
	this->_bullets->retain();

	//背景音乐
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("gameMusic.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("gameMusic.mp3",true);

	//返回菜单按钮
	CCMenuItemImage *pBackItem = CCMenuItemImage::create(
                                        "returnNormal.png",
                                        "returnSelected.png",
                                        this,
                                        menu_selector(GameScene::menuBackCallback));
    
	pBackItem->setPosition(ccp(origin.x + visibleSize.width - pBackItem->getContentSize().width/2 ,
                                origin.y + pBackItem->getContentSize().height/2));

	CCMenu *pBack = CCMenu::create(pBackItem, NULL);
    pBack->setPosition(CCPointZero);
    this->addChild(pBack, 3);

    //暂停按钮
	CCMenuItemImage *pPauseItem = CCMenuItemImage::create(
                                        "pauseNormal.png",
                                        "pauseSelected.png",
										this,
                                        menu_selector(GameScene::pause));

	pPauseItem->setPosition(ccp(origin.x + visibleSize.width - pPauseItem->getContentSize().width/2 - pBackItem->getContentSize().width ,
                                origin.y + pPauseItem->getContentSize().height/2));
	pPause = CCMenu::create(pPauseItem, NULL);
    pPause->setPosition(CCPointZero);
    this->addChild(pPause, 3);

	//继续按钮
	CCMenuItemImage *pResumeItem = CCMenuItemImage::create(
                                        "resumeNormal.png",
                                        "resumeSelectd.png",
										this,
                                        menu_selector(GameScene::resume));

	pResumeItem->setPosition(ccp(origin.x + visibleSize.width - pResumeItem->getContentSize().width/2 - pBackItem->getContentSize().width ,
                                origin.y + pResumeItem->getContentSize().height/2));
	pResume = CCMenu::create(pResumeItem, NULL);
    pResume->setPosition(CCPointZero);
    this->addChild(pResume, 3);
	pResume->setVisible(false);

	//背景
	bgHeight =  (visibleSize.height + origin.y);

    bgSprite1=CCSprite::create("background.png");  
    bgSprite1->setAnchorPoint(ccp(0.5, 0));  
    bgSprite1->setPosition(ccp(visibleSize.width/2 + origin.x,bgHeight));  
      
    this->addChild(bgSprite1, 0);
  
    bgSprite2=CCSprite::create("background.png");  
    bgSprite2->setAnchorPoint(ccp(0.5, 0));  
    bgSprite2->setPosition(ccp(visibleSize.width/2 + origin.x,bgHeight+1)); 

	this->addChild(bgSprite2, 0);
    
	//分数
	CCLabelTTF* strScore = CCLabelTTF::create ("Score: ","Arial",30);
	strScore->setPosition(ccp(origin.x + visibleSize.width - strScore->getContentSize().width,
                            origin.y + visibleSize.height - strScore->getContentSize().height/2));

    this->addChild(strScore, 1);

	//玩家飞机
	player = CCSprite::create("player.png");
	player->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + player->getContentSize().height/2));
	this->addChild(player,1);
	
	//生成敌方飞机
	this->schedule(schedule_selector(GameScene::GameLogic),0.5);

	//生成子弹
	this->schedule(schedule_selector(GameScene::GameLogicaddBullet2),0.3);
	
	//子弹和飞机碰撞判定
	this->schedule(schedule_selector(GameScene::collision));

	//滚动背景
	this->schedule(schedule_selector(GameScene::scrollBg));

	//显示分数
	this->schedule(schedule_selector(GameScene::ShowScore));

    return true;
}
GameScene::GameScene(){
	_monsters = NULL;
	_bullets = NULL;
	score = 0;
	monsterSpeed = 300;
}
GameScene::~GameScene(){
	 if (_monsters)
    {
        _monsters->release();
        _monsters = NULL;
    }
    if (_bullets)
    {
        _bullets->release();
        _bullets = NULL;
    }
}
void GameScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.6f, HelloWorld::scene()));

}
void GameScene::pause(CCObject* pSender){ 

	this->pauseSchedulerAndActions();
	//停止键盘和触屏操作
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);

	pPause->setVisible(false);
	pResume->setVisible(true);
}
void GameScene::resume(CCObject* pSender){

	this->resumeSchedulerAndActions();

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	pPause->setVisible(true);
	pResume->setVisible(false);
}
void GameScene::scrollBg(float dt){

	bgHeight--;  
	if (bgHeight<=0)  
	{    
		bgHeight = (visibleSize.height + origin.y);    
	}  
	//设置滚动背景
	bgSprite1->setPosition(ccp(bgSprite2->getPosition().x, bgHeight-(visibleSize.height + origin.y)));  
	bgSprite2->setPosition(ccp(bgSprite2->getPosition().x, bgHeight-1));  
}
void GameScene::addBullet1(){
	CCSprite* pBullet = CCSprite::create("bullet1.png");
	float x = player->getPositionX();
	float y = player->getPositionY();
	pBullet->setPosition(ccp(x,y+40));
	this->addChild(pBullet,2);

	//子弹音效
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shoot.mp3");


	CCMoveTo *actionMove = CCMoveTo::create(1,ccp(x,origin.y + visibleSize.height - pBullet->getContentSize().height));
	CCCallFuncN *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
    pBullet->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));

	//给子弹添加标签，并把子弹添加到数组中
	pBullet->setTag(2);
    _bullets->addObject(pBullet);
}
void GameScene::addBullet2(){
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* pBullet1 = CCSprite::create("bullet2.png");
	CCSprite* pBullet2 = CCSprite::create("bullet2.png");
	float x = player->getPositionX();
	float y = player->getPositionY();
	pBullet1->setPosition(ccp(x+30,y+30));
	pBullet2->setPosition(ccp(x-30,y+30));
	this->addChild(pBullet1,2);
	this->addChild(pBullet2,2);

	//子弹音效
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shoot.mp3");


	CCMoveTo *actionMove1 = CCMoveTo::create(1,ccp(x + 30 , origin.y + visibleSize.height - pBullet1->getContentSize().height));
	CCMoveTo *actionMove2 = CCMoveTo::create(1,ccp(x - 30 , origin.y + visibleSize.height - pBullet2->getContentSize().height)); 
	CCCallFuncN *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
    pBullet1->runAction(CCSequence::create(actionMove1, actionMoveDone, NULL));
	pBullet2->runAction(CCSequence::create(actionMove2, actionMoveDone, NULL));

	//给子弹添加标签，并把子弹添加到数组中
	pBullet1->setTag(2);
    _bullets->addObject(pBullet1);

	pBullet2->setTag(2);
    _bullets->addObject(pBullet2);
}
void GameScene::addMonster(){
	CCSprite* pMonster = CCSprite::create("monster.png");

    int minX = pMonster->getContentSize().width  /2;
    int maxX = visibleSize.width - pMonster->getContentSize().width /2;
    int rangeX = maxX - minX;
    int actualX = (rand() % rangeX) + minX;


	pMonster->setPosition(ccp(actualX,origin.y + visibleSize.height - pMonster->getContentSize().height));
	this->addChild(pMonster,1);

	int time = (origin.y + visibleSize.height - pMonster->getContentSize().height)/monsterSpeed;
    //time s内从原来位置到ccp(actualX,origin.y + pMonster->getContentSize().height/2)位置
    CCMoveTo *actionMove = CCMoveTo::create(time,ccp(actualX,origin.y + pMonster->getContentSize().height/2));
    CCCallFuncN *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
    pMonster->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));

	//给敌方飞机添加标签，并把敌方飞机添加到数组中
	pMonster->setTag(1);
    _monsters->addObject(pMonster);

}
void GameScene::spriteMoveFinished(CCNode *sender)
{
    CCSprite *sprite = (CCSprite*)sender;
    this->removeChild(sprite, true);

	if (sprite->getTag() == 1)
	{
		_monsters->removeObject(sprite);
	}
	else if (sprite->getTag() ==2)
	{
		_bullets->removeObject(sprite);
	}
}
void GameScene::collision(float dt){
	CCArray *bulletsToDelete = CCArray::create();

	CCObject *pObject = NULL;
	CCObject *pObject2 = NULL;

	CCARRAY_FOREACH(_bullets, pObject){                                         //遍历子弹数组
		CCSprite *bullet = (CCSprite*)pObject;
		CCArray *monstersToDelete = CCArray::create();
		CCARRAY_FOREACH(_monsters, pObject2)                                   //遍历敌方飞机数组
		{
			CCSprite *monster = (CCSprite*)pObject2;
			if(bullet->boundingBox().intersectsRect(monster->boundingBox()))    //判断子弹的边界是否碰到敌方飞机的边界       
			{
				monstersToDelete->addObject(monster);                           //子弹碰到飞机边界的话就放入monstersToDelete中
				bulletsToDelete->addObject(bullet);                             //同样子弹也要放入bulletsToDelete中
			}           
		}
		CCARRAY_FOREACH(monstersToDelete, pObject2)                             //遍历monstersToDelete，把待删除的飞机都删除了
		{
			CCSprite *monster = (CCSprite*)pObject2;
			_monsters->removeObject(monster);
			this->removeChild(monster, true);
		}
		monstersToDelete->release();

	}
	CCARRAY_FOREACH(bulletsToDelete, pObject)                                    //遍历bulletsToDelete，把待删除的子弹都删除了
	{
		CCSprite *bullet = (CCSprite*)pObject;
		_bullets->removeObject(bullet);

		//爆炸点
		CCSprite *monsterExplosion = CCSprite::create ("monsterExplosion.png");

		float x = bullet->getPositionX();
		float y = bullet->getPositionY();
		monsterExplosion->setPosition (ccp(x,y));

		this->removeChild(bullet, true);

		//爆炸
		this->addChild(monsterExplosion,2);
		CCActionInterval * fadeout = CCFadeOut::create(1);
		CCCallFuncN *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
		monsterExplosion->runAction(CCSequence::create(fadeout,actionMoveDone,NULL));

		//爆炸音效
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("explosion.mp3");

		//分数加一
		score += 1;
	}
	bulletsToDelete->release();


	//玩家飞机与敌方飞机碰撞
	CCARRAY_FOREACH(_monsters, pObject2)                             //遍历敌方飞机数组
	{
		CCSprite *monster = (CCSprite*)pObject2;
		if(player->boundingBox().intersectsRect(monster->boundingBox()))          //判断玩家飞机的边界是否与敌方飞机碰撞
		{
			CCSprite *playerExplosion = CCSprite::create("playerExplosion.png");
			playerExplosion->setPosition(player->getPosition());
			this->addChild(playerExplosion,2);

			//失败画面
			GameScene::gameOver();
		}
	}
}
void GameScene::ShowScore(float dt){
    //显示分数
	this->removeChild(pScore,true);
	CCString *string = CCString::createWithFormat("%i",score);
	pScore = CCLabelTTF::create(string->getCString(),"Arial", 30);
    pScore->setPosition(ccp(origin.x + visibleSize.width - pScore->getContentSize().width/2,
                            origin.y + visibleSize.height - pScore->getContentSize().height/2));
	pScore->setColor (ccc3(225,0,0));
    this->addChild(pScore, 3);
}
void GameScene::gameOver(){
	
	this->pauseSchedulerAndActions();
	//停止键盘和触屏操作
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	
	pPause->setEnabled(false);
	pResume->setEnabled(false);

	CCLayer *gameoverLayer=CCLayer::create();                           //暂停图层
    gameoverLayer->setPosition(ccp(visibleSize.width/2 - gameoverLayer->getContentSize().width /2 , visibleSize.height/2 - gameoverLayer->getContentSize().height /2));  
    this->addChild(gameoverLayer, 4);

	CCSprite *dialog = CCSprite::create("dialog.png");
	dialog->setPosition (ccp(visibleSize.width/2 , visibleSize.height/2 ));
	gameoverLayer->addChild(dialog);

	CCLabelTTF *label = CCLabelTTF::create("Score", "Arial", 30);
	label->setPosition(ccp(dialog->getContentSize().width/2 , dialog->getContentSize().height - label->getContentSize().height));
	label->setColor(ccc3(0,0,0));
	dialog->addChild (label);

	CCString *string = CCString::createWithFormat("%i",score);
	CCLabelTTF *pScroe = CCLabelTTF::create(string->getCString() , "Arial", 30);
	pScroe->setPosition(ccp(dialog->getContentSize().width/2 , dialog->getContentSize().height/2));
	pScroe->setColor(ccc3(0,0,0));
	dialog->addChild (pScroe);
    
	CCMenuItemImage * okItem = CCMenuItemImage::create("okNormal.png",
		"okSelected.png",this,menu_selector(GameScene::menuBackCallback));

	CCMenu * menu = CCMenu::create(okItem,NULL);
	menu->alignItemsHorizontallyWithPadding(5);
	menu->setPosition(ccp(dialog->getContentSize().width/2 , okItem->getContentSize().height));
	dialog->addChild(menu);
	//kCCMenuHandlerPriority的值为-128，代表的是菜单按钮的触摸优先级
	//设置menu优先级，这里设置为普通menu的二倍减一，原因看下边
	menu->setTouchPriority(kCCMenuHandlerPriority*2-1);
}
void GameScene::GameLogic(float dt)
{
    this->addMonster();
}
void GameScene::GameLogicaddBullet1(float dt){
	this->addBullet1();
}
void GameScene::GameLogicaddBullet2(float dt){
	this->addBullet2();
}
/*void GameScene::keyArrowClicked(int arrow)  
{   
    float x = player->getPositionX();
	float y = player->getPositionY();
	float playerContentSizeWidth = player->getContentSize().width;
	float playerContentSizeHeight = player->getContentSize().height;
	if (arrow == kTypeLeftArrowClicked)  
    {  
		// 左方向键按下
		if(x > playerContentSizeWidth/2)
			x -= 10;
	}  
    else if (arrow == kTypeRightArrowClicked)  
    {  
        // 右方向键按下  
		if(x < visibleSize.width - playerContentSizeWidth/2)
			x += 10;
    }  
      
    if (arrow == kTypeUpArrowClicked)  
    {  
        // 上方向键按下
		if(y < visibleSize.height - playerContentSizeHeight/2)
			y += 10;
    }  
    else if (arrow == kTypeDownArrowClicked)  
    {  
        // 下方向键按下 
		if(y >playerContentSizeHeight/2)
			y -= 10;
    }
	CCMoveTo *actionMove = CCMoveTo::create(0.5,ccp(x,y));
    player->runAction(actionMove);
}
*/
void GameScene::ccTouchesBegan(cocos2d::CCSet *pTouches,cocos2d::CCEvent *pEvent){

	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = this->convertTouchToNodeSpace(touch);

	//如果触摸点在屏外则将点设在屏幕边缘
	if(location.x < origin.x + player->getContentSize().width/2)
		location.x = origin.x + player->getContentSize().width/2;
	if(location.x > origin.x + visibleSize.width - player->getContentSize().width/2)
		location.x = origin.x + visibleSize.width - player->getContentSize().width/2;
	if(location.y < origin.y+player->getContentSize().height/2)
		location.y = origin.y+player->getContentSize().height/2;
	if(location.y > origin.y + visibleSize.height - player->getContentSize().height/2)
		location.y = origin.y + visibleSize.height - player->getContentSize().height/2;

    float x1 = player->getPositionX();
	float y1 = player->getPositionY();
	float x2 = location.x;
	float y2 = location.y;

	float length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
	float speed  = 480;
	float time   = length/speed;

	//停止之前的动作
	player->stopAllActions();

	CCMoveTo *actionMove = CCMoveTo::create(time,location);
    player->runAction(actionMove);
}
void GameScene::ccTouchesMoved(cocos2d::CCSet *pTouches,cocos2d::CCEvent *pEvent){
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = this->convertTouchToNodeSpace(touch);

	//如果触摸点在屏外则将点设在屏幕边缘
	if(location.x < origin.x+player->getContentSize().width/2)
		location.x = origin.x+player->getContentSize().width/2;
	if(location.x > origin.x + visibleSize.width - player->getContentSize().width/2)
		location.x = origin.x + visibleSize.width - player->getContentSize().width/2;
	if(location.y < origin.y+player->getContentSize().height/2)
		location.y = origin.y+player->getContentSize().height/2;
	if(location.y > origin.y + visibleSize.height - player->getContentSize().height/2)
		location.y = origin.y + visibleSize.height - player->getContentSize().height/2;
  
	player->setPosition(location);
}
