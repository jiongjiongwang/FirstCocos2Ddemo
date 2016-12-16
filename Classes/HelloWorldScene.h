#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

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
    
    void ButtonPress(Ref* pSender);
    
    void ActionDone();
    
    
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    
    
    //长方形的下落白块精灵的数组(暂定为4）
    cocos2d::Sprite *_spriteArray[4];
    

    //精灵容器(用于存放精灵)
    std::vector<cocos2d::Sprite *> m_vecSprite;
    
    
    
    //键盘精灵数组
    cocos2d::Sprite *_keySpriteArray[64];
    
    
    
    
    //Label属性(指针)
    cocos2d::Label *_pressLabel;
    
    
    /* 重写update函数 */
    virtual void  update(float dt);
    
    //记录是否刚好接触到了键盘
    bool _isContactFlag;
    
    //判断有没有正在播放当前的音符
    bool _isPlay;
    
    //播放记时
    float _playTime;
    

    

};

#endif // __HELLOWORLD_SCENE_H__
