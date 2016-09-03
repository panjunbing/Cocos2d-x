/*�Ի��򳡾���ľ���ʵ��*/
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

		//���������ı���ͼƬ������������λ��Ϊ������Ļ���е�
		CCSprite * background = CCSprite::create("dlgBackground.png");
		m_bgSprite = background;
		background->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
		this->addChild(background,0);

		//��ñ���ͼƬ�Ĵ�С
        CCSize contentSize = background->getContentSize();
        m_size = contentSize;
 
		//���ز˵�
		CCMenuItemImage * item = CCMenuItemImage::create("okNormal.png",
			"okSelected.png",this,menu_selector(PopMsgScene::Button));

		CCMenu * menu = CCMenu::create(item,NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(m_size.width/2 ,item->getContentSize().height/2));
		//kCCMenuHandlerPriority��ֵΪ-128��������ǲ˵���ť�Ĵ������ȼ�
		//����menu���ȼ�����������Ϊ��ͨmenu�Ķ�����һ��ԭ���±�
		menu->setTouchPriority(kCCMenuHandlerPriority*2-1);

		m_bgSprite->addChild(menu);

		//������Ŀ���ı�����
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
	//kCCMenuHandlerPriority=-128�������ֵ����Ϊ-128�Ķ��������Ա��±ߵĲ�����ȼ���
	//����ccTouchBegan�ķ���ֵΪtrue��˵�������Ĳ㽫���ܲ������������Ϣ�ˣ�ֻ��������ϱߵ�
	//�˵������ȼ�������Ҫ���������ϱߵĲ˵��ǿ��Խ��յ�������Ϣ��
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,
		kCCMenuHandlerPriority*2,true);
}

bool PopMsgScene::ccTouchBegan(CCTouch * touch,CCEvent * pevent)
{
	return true;
}

//����˵���ť��ʱ����õ��¼�������
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
	//����ttf��ˮƽ���뷽ʽ
	content1->setDimensions(CCSize(this->m_size.width-60,this->m_size.height-100));
	content1->setHorizontalAlignment(kCCTextAlignmentLeft);

	content2->setDimensions(CCSize(this->m_size.width-60,this->m_size.height-100));
	content2->setHorizontalAlignment(kCCTextAlignmentLeft);

	content1->setColor(ccc3(0,0,0));
	content2->setColor(ccc3(0,0,0));
	m_bgSprite->addChild(content1);
	m_bgSprite->addChild(content2);
}