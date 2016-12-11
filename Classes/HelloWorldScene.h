#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

//屏幕大小的宏
#define WIN_SIZE   Director::getInstance()->getVisibleSize()

//屏幕初始坐标的宏
#define WIN_ORIGIN   Director::getInstance()->getVisibleOrigin()


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
    
    
    
    //精灵属性(指针)
    cocos2d::Sprite *_spriteButton1;
    
    cocos2d::Sprite *_spriteButton2;
    
    //键盘精灵属性
    cocos2d::Sprite *_leftKeySprite;
    
    //Label属性(指针)
    cocos2d::Label *_pressLabel;
    
    
    float _keySpriteHeight;
    
    /* 重写update函数 */
    virtual void  update(float dt);
};

#endif // __HELLOWORLD_SCENE_H__
