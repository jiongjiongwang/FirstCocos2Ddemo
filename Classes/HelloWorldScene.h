#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "MyLink.h"


//场景大小的宏
//当前的width:480.000031 Height:270.422546(横向屏幕)
#define WIN_SIZE   Director::getInstance()->getVisibleSize()


//场景初始坐标的宏
//当前的x:-0.0000152587891 y:24.7887268
#define WIN_ORIGIN   Director::getInstance()->getVisibleOrigin()



#define preDistance (WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height)




//设置前戏时间(固定不变)
#define preActionTime 3.0f

//左移的键盘数目
#define deltaKeyNum 25

class HelloWorld : public cocos2d::Layer
{
    
    
    
    
public:
    
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    //点击开始
    void ButtonPress(Ref* pSender);
    
    //点击暂停按钮
    void ButtonPausePress(Ref* pSender);
    
    
    //有关通知的方法
    //void GetEvent(Ref* sender);
    
    
    
    void CreateNoEvent(Ref* sender);
    
    //声明一个方法，用于接受外接的事件结构体来生成精灵
    void GetDataCreateSprite(float duratime,int pianoNum);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    //键盘精灵数组
    cocos2d::Sprite *_keySpriteArray[64];
    
    //精灵容器(用于存放下落白块精灵)
    std::vector<cocos2d::Sprite *> m_vecSprite;
    
    
    
    
    
    
    //Label属性(指针)(点击开始)
    cocos2d::Label *_pressLabel;
    
    //Label(点击暂停)
    cocos2d::Label *_pressPauseLabel;
    
    
    
    /* 重写update函数 */
    virtual void  update(float dt);
    
    
    //判断有没有正在播放当前的音符
    bool _isPlay;
    
    //判断是否暂停过
    bool _isHasPause;
    
    
    //播放记时
    float _playTime;
    
    
    //声明一个TempLinkOC变量
    TempLinkOC _LinkInstance;


};

#endif // __HELLOWORLD_SCENE_H__
