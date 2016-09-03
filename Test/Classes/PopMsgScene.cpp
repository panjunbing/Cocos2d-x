/*对话框场景类的具体实现*/
#include "PopMsgScene.h"

CCScene * PopMsgScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		PopMsgScene * layer = PopMsgScene::create();
		scene->addChild(layer);
	}
	while(0);

	return scene;
}

bool PopMsgScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize  visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		//设置这个层的背景图片，并且设置其位置为整个屏幕的中点
		CCSprite * background = CCSprite::create("dlgBackground.png");
		m_bgSprite = background;
		background->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
		this->addChild(background,0);

		//获得背景图片的大小
        CCSize contentSize = background->getContentSize();
        m_size = contentSize;
 
		//返回菜单
		CCMenuItemImage * item = CCMenuItemImage::create("okNormal.png",
			"okSelected.png",this,menu_selector(PopMsgScene::Button));

		CCMenu * menu = CCMenu::create(item,NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(m_size.width/2 ,item->getContentSize().height/2));
		//kCCMenuHandlerPriority的值为-128，代表的是菜单按钮的触摸优先级
		//设置menu优先级，这里设置为普通menu的二倍减一，原因看下边
		menu->setTouchPriority(kCCMenuHandlerPriority*2-1);

		m_bgSprite->addChild(menu);

		//设置题目和文本内容
		this->setTitle();
		this->setContent();

		this->setTouchEnabled(true);
		bRet = true;
	}
	while(0);

	return bRet;
}

void PopMsgScene::registerWithTouchDispatcher()
{
	//kCCMenuHandlerPriority=-128，将这个值设置为-128的二倍，可以比下边的层的优先级高
	//而且ccTouchBegan的返回值为true，说明其他的层将接受不到这个触摸消息了，只有这个层上边的
	//菜单的优先级比他还要打，所以它上边的菜单是可以接收到触摸消息的
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,
		kCCMenuHandlerPriority*2,true);
}

bool PopMsgScene::ccTouchBegan(CCTouch * touch,CCEvent * pevent)
{
	return true;
}

//点击菜单按钮的时候调用的事件处理函数
void PopMsgScene::yesButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}
void PopMsgScene::Button(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}
void PopMsgScene::setTitle()
{
	CCSize  visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLabelTTF * title = CCLabelTTF::create("Message","Marker Felt.ttf", 24);
	title->setPosition(ccp(m_size.width/2,m_size.height-title->getContentSize().height/2));
	title->setColor(ccc3(0,0,0));
	m_bgSprite->addChild(title);
}
void PopMsgScene::setContent()
{
	CCLabelTTF * content1 = CCLabelTTF::create("author : panjunbing","Marker Felt.ttf",16);
	CCLabelTTF * content2 = CCLabelTTF::create("Thank for YellowKiss's resources","Marker Felt.ttf",16);
	content1->setPosition(ccp(m_size.width/2,m_size.height/2));
	content2->setPosition(ccp(m_size.width/2,m_size.height/2-content1->getContentSize().height*2));
	//设置ttf的水平对齐方式
	content1->setDimensions(CCSize(this->m_size.width-60,this->m_size.height-100));
	content1->setHorizontalAlignment(kCCTextAlignmentLeft);

	content2->setDimensions(CCSize(this->m_size.width-60,this->m_size.height-100));
	content2->setHorizontalAlignment(kCCTextAlignmentLeft);

	content1->setColor(ccc3(0,0,0));
	content2->setColor(ccc3(0,0,0));
	m_bgSprite->addChild(content1);
	m_bgSprite->addChild(content2);
}