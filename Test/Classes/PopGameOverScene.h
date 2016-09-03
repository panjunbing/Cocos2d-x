/*�Ի��򳡾����ͷ�ļ�*/
#ifndef _POPGAMEOVERSCENE_SCENE_H_
#define _POPGAMEOVERSCENE_SCENE_H_
#include "cocos2d.h"

using namespace cocos2d;

class PopGameOverScene : public CCLayer
{
public:
	static CCScene * scene();
	
	bool init();
	CREATE_FUNC(PopGameOverScene);

private:
	//ע�ᴥ���¼���ʵ��ccTouchBegan()����
	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
	//�ڵ����ĶԻ����ϼ�������ť�����µĺ����Ƕ�Ӧ�İ�ť�Ĵ����¼�
	void yesButton(CCObject * object);
	void Button(CCObject * object);
	//���öԻ����title
	void setTitle();
	//���öԻ�����ı�����
	void setContent();
	//m_size������ǶԻ��򱳾��Ĵ�С
	CCSize m_size;
	//�Ի���ı�������
	CCSprite * m_bgSprite;
};
#endif