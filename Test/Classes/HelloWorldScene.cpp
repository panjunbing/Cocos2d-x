#include "HelloWorldScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }
    
    visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLayer *layer = CCLayer::create();
	this->addChild(layer);

	//预加载音乐
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("gameMusic.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("shoot.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("explosion.mp3");

	
	//开始按钮
	CCMenuItemImage *pPlayItem = CCMenuItemImage::create("PlayNormal.png","PlaySelected.png",this,menu_selector(HelloWorld::menuPlayGame));
	pPlayItem->setPosition (ccp(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height/2-pPlayItem->getContentSize().height));
	pMenuPlay = CCMenu::create(pPlayItem, NULL);
	pMenuPlay->setPosition (CCPointZero);
	layer->addChild(pMenuPlay,1);

	//关于按钮
    CCMenuItemImage *pMsgItem = CCMenuItemImage::create("massageNormal.png","massageSelectd.png",this,menu_selector(HelloWorld::menuMsg));
	pMsgItem->setPosition (ccp(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height/2-pMsgItem->getContentSize().height*2));
	pMenuMsg = CCMenu::create(pMsgItem, NULL);
	pMenuMsg->setPosition (CCPointZero);
	layer->addChild(pMenuMsg,1);

	//退出按钮
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("qiutNormal.png","quitSelected.png",this,menu_selector(HelloWorld::menuCloseCallback));
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height/2-pCloseItem->getContentSize().height*3));
	pMenuClose = CCMenu::create(pCloseItem, NULL);
    pMenuClose->setPosition(CCPointZero);
    layer->addChild(pMenuClose, 1);

	//说明按钮
    CCMenuItemImage *pIntruductionItem = CCMenuItemImage::create("intruductionNormal.png","intruductionSelected.png",this,menu_selector(HelloWorld::menuIntruduction));
	pIntruductionItem->setPosition(ccp(origin.x + visibleSize.width - pIntruductionItem->getContentSize().width ,
		                          origin.y +pIntruductionItem->getContentSize().height/2));
	CCMenu* pMenuIntruduction = CCMenu::create(pIntruductionItem, NULL);
    pMenuIntruduction->setPosition(CCPointZero);
    layer->addChild(pMenuIntruduction, 1);

    //设置按钮
    CCMenuItemImage *pSettingItem = CCMenuItemImage::create("settingNormal.png","settingSelected.png",this,menu_selector(HelloWorld::menuSetting));
	pSettingItem->setPosition(ccp(origin.x + visibleSize.width - pSettingItem->getContentSize().width*2 ,
		                          origin.y +pSettingItem->getContentSize().height/2));
	CCMenu* pMenuSetting = CCMenu::create(pSettingItem, NULL);
    pMenuSetting->setPosition(CCPointZero);
    layer->addChild(pMenuSetting, 1);

	//音乐开关按钮
	CCMenuItemImage *pMusicItem = CCMenuItemImage::create("musicClose.png","musicClose.png",this,menu_selector(HelloWorld::menuMusic));
	pMusicItem->setPosition(ccp(origin.x + pMusicItem->getContentSize().width ,
		                          origin.y +pMusicItem->getContentSize().height/2));
	CCMenu* pMenuMusic = CCMenu::create(pMusicItem, NULL);
    pMenuMusic->setPosition(CCPointZero);
    layer->addChild(pMenuMusic, 1);

	//背景
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    layer->addChild(pSprite, 0);
    
    return true;
}

void HelloWorld::menuPlayGame(CCObject *pSender){
CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.6f, GameScene::scene()));
}
void HelloWorld::menuMsg(CCObject *pSender){


	CCLayer *msgLayer=CCLayer::create();                           //Msg图层
    msgLayer->setPosition(ccp(visibleSize.width/2 - msgLayer->getContentSize().width /2 , visibleSize.height/2 - msgLayer->getContentSize().height /2));  
    this->addChild(msgLayer,2);

	CCSprite *dialog = CCSprite::create("dialog.png");
	dialog->setPosition (ccp(visibleSize.width/2 , visibleSize.height/2 ));
	msgLayer->addChild(dialog);

	CCLabelTTF *label = CCLabelTTF::create("Tips", "Arial", 30);
	label->setPosition(ccp(dialog->getContentSize().width/2 , dialog->getContentSize().height - label->getContentSize().height));
	label->setColor(ccc3(0,0,0));
	dialog->addChild (label);

	CCLabelTTF * content1 = CCLabelTTF::create("author : panjunbing","Arial",16);
	CCLabelTTF * content2 = CCLabelTTF::create("Thank for YellowKiss's resources","Arial",16);
	content1->setColor(ccc3(0,0,0));
	content2->setColor(ccc3(0,0,0));
	content1->setPosition(ccp(content1->getContentSize().width - 10 , dialog->getContentSize().height *3/4 ));
	content2->setPosition(ccp(content2->getContentSize().width - 56 , dialog->getContentSize().height /2 ));
    dialog->addChild (content1);
	dialog->addChild (content2);

	//返回菜单
		CCMenuItemImage * okItem = CCMenuItemImage::create("okNormal.png",
			"okSelected.png",this,menu_selector(HelloWorld::ok));

		CCMenu * okMenu = CCMenu::create(okItem,NULL);
		okMenu->setPosition(ccp(dialog->getContentSize().width/2 , okItem->getContentSize().height));
		dialog->addChild(okMenu);

		pMenuPlay->setEnabled(false);
		pMenuMsg->setEnabled(false);
		pMenuClose->setEnabled(false);
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
void HelloWorld::ok(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.6f, HelloWorld::scene()));
}
void HelloWorld::menuIntruduction(CCObject *pSender){}
void HelloWorld::menuSetting(CCObject *pSender){}
void HelloWorld::menuMusic(CCObject *pSender){}
