#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

//声明cocos2d的命名空间
//#define USING_NS_CC  using namespace cocos2d
USING_NS_CC;


//创建场景
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    //场景节点
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    //层节点:HelloWorld继承自层节点Layer
    auto layer = HelloWorld::create();

    //将初始化之后的实例layer挂载到父节点上
    scene->addChild(layer);
    
    

    //返回父节点对象指针
    return scene;
}

//在此设计UI
bool HelloWorld::init()
{
    
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    
    //场景大小:width(宽度)和height(高度)
    //当前的width:480.000031 Height:270.422546(横向屏幕)
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    
    
    
    //场景的坐标:x(x轴)和y(y轴)
    //当前的x:-0.0000152587891 y:24.7887268
    //OpenGL坐标系:原点在左下角
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    

    //添加关闭按钮到菜单层上
    //MenuItemImage类->MenuItemSprite类->MenuItem类->Node类->Ref类
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(WIN_ORIGIN.x + WIN_SIZE.width - closeItem->getContentSize().width/2 ,
                                WIN_ORIGIN.y + closeItem->getContentSize().height/2));
    
    //1-利用图片生成按钮
    /*
    //(1)创建图片精灵(Sprite类)
    auto pressImage = Sprite::create("HelloWorld.png");
    
    //(2)生成使用精灵图片的按钮(MenuItemSprite类)
    //MenuItemSprite类->MenuItem类->Node类->Ref类
    auto pressItem = MenuItemSprite::create(pressImage, pressImage, CC_CALLBACK_1(HelloWorld::ButtonPress, this));
    
    pressItem->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    */
    
    
    
    //2-使用标签label生成按钮
    //2.1 生成Label类
    //Label类->Node类->Ref类
    _pressLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf",25);
    
    //2.2 利用label对象生成MenuItemLabel类对象
    //MenuItemLabel类->MenuItem类->Node类->Ref类
    auto labelPress = MenuItemLabel::create(_pressLabel, CC_CALLBACK_1(HelloWorld::ButtonPress, this));
    
    labelPress->setPosition(Vec2(WIN_SIZE.width/2 + WIN_ORIGIN.x, WIN_SIZE.height/2 + WIN_ORIGIN.y));

    
    //3-Menu类:负责管理场景上的UI控件
    //Menu类->Layer类->Node类->Ref类
    auto menu = Menu::create(closeItem,labelPress, NULL);
    menu->setPosition(Vec2::ZERO);
    
    //1为层级数，决定背景精灵的绘制顺序,参数数值越大，其绘制时就会越靠前
    this->addChild(menu, 1);
    
    
    
    //精灵类1
    //精灵类Sprite->Node类->Ref类
    _spriteButton1 = Sprite::create("button_gray_disable.9.png");
    _spriteButton1->setPosition(Vec2(_spriteButton1->getContentSize().width/2 + WIN_ORIGIN.x, WIN_SIZE.height/2 + WIN_ORIGIN.y));
    
    this->addChild(_spriteButton1, 0);
    
    
    
    //精灵类2
    _spriteButton2 = Sprite::create("button_gray_disable.9.png");
    _spriteButton2->setPosition(Vec2(_spriteButton2->getContentSize().width/2 + WIN_ORIGIN.x, WIN_SIZE.height/2 + WIN_ORIGIN.y - _spriteButton2->getContentSize().height));
    
    //设置精灵2的颜色(在原有的白色图片基础上进行重新绘色为红色)
    _spriteButton2->setColor(Color3B(255,0,0));
    //设置精灵2的透明度
    _spriteButton2->setOpacity(128);
    
    
    this->addChild(_spriteButton2, 0);

    
    //添加底部的键盘精灵
    //1-左
    _leftKeySprite = Sprite::create("Left.png");
    
    _leftKeySprite->setPosition(Vec2(_leftKeySprite->getContentSize().width/2 + WIN_ORIGIN.x,WIN_ORIGIN.y + _leftKeySprite->getContentSize().height/2));
    
    this->addChild(_leftKeySprite);
    
    _keySpriteHeight = _leftKeySprite->getContentSize().height;
    
    
    
    
    //批量渲染
    /*
    //1-利用图片建立SpriteBatchNode对象(最多同时绘30个精灵)
    auto midBatchNode = SpriteBatchNode::create("Mid.png",30);
    this->addChild(midBatchNode);
    
    //2-生成精灵对象的过程
    
    for (int i = 1; i <= 20; i++)
    {
        auto midKeySprite = Sprite::createWithTexture(midBatchNode->getTexture(), Rect(60 * i + origin.x + _leftKeySprite->getContentSize().width, origin.y + 150/2, 60, 150));
        
        midBatchNode->addChild(midKeySprite);

    }
    */
    
    
    //2-中

    auto midKeySprite = Sprite::create("Mid.png");
    
    midKeySprite->setPosition(Vec2(midKeySprite->getContentSize().width/2 + WIN_ORIGIN.x + _leftKeySprite->getContentSize().width,WIN_ORIGIN.y + midKeySprite->getContentSize().height/2));
    
    this->addChild(midKeySprite);
    
    
    
    //3-右
    auto rightKeySprite = Sprite::create("Right.png");
    
    rightKeySprite->setPosition(Vec2(rightKeySprite->getContentSize().width/2 + WIN_ORIGIN.x + _leftKeySprite->getContentSize().width + midKeySprite->getContentSize().width,WIN_ORIGIN.y + rightKeySprite->getContentSize().height/2));
    
    this->addChild(rightKeySprite);
    
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::ButtonPress(Ref* pSender)
{
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    
    //场景的坐标:x(x轴)和y(y轴)
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    //1-添加动作类:移动动作
    //auto actionMoveBy = MoveBy::create(4.0f, Vec2(0,-visibleSize.height/2));
    auto actionMoveTo = MoveTo::create(4.0f, Vec2(_spriteButton1->getContentSize().width/2 + WIN_ORIGIN.x,WIN_ORIGIN.y + _keySpriteHeight));
    
    //Move1的反向操作
    //auto actionByBack = actionMove1->reverse();
    
    
    //2-添加动作类:缩放动作
    //auto labelScaleTo = ScaleTo::create(1.0f,0.0f,0.0f);
    //auto actionScaleBy = ScaleBy::create(1.0f,2.0f);
    
    
    //设置锚点
    //_pressLabel->setAnchorPoint(Vec2(0.5f,0.5f));
    
    //3-添加动作类:淡入淡出动作
    //3.1 淡入动作:透明度从0慢慢变为255从而显示在屏幕上
   // auto fadeInAction = FadeIn::create(1.0f);
    //3.2 淡出动作:透明度从255逐渐变为0从而在屏幕上消失
    auto fadeOutAction = FadeOut::create(0.5f);
    
    _pressLabel->runAction(fadeOutAction);
    //移除开始label
    this->removeChild(_pressLabel);
    
    
    //4-变色类动作
    /*
    auto tintToAction = TintTo::create(2, 255, 0, 255);
    
    _leftKeySprite->runAction(tintToAction);
    */
    auto tintToAction = TintTo::create(4, 255, 0, 255);
    
    //5-多个动作同步进行
    auto spawnAction = Spawn::create(actionMoveTo,tintToAction, NULL);
    
    //运行动作1
    _spriteButton1->runAction(spawnAction);
    
    
    
    
     //精灵2的动作
     //1-运动1
     auto actionMove1 = MoveBy::create(3.0f, Vec2(WIN_SIZE.width/2,0));
     //2-运动2:delay动作(延时2秒)
     auto actionDelay = DelayTime::create(2);
    //3-运动3
    auto actionMove2 = MoveBy::create(4.0f, Vec2(-WIN_SIZE.width/2,0));
    //4-调用函数动作
    //auto actionCalFun = CallFunc::create(HelloWorld::ActionDone());
    
    
     //顺序动作
    auto FiniteTimeAction = Sequence::create(actionMove1,actionDelay,actionMove2, NULL);
    
    
     _spriteButton2->runAction(FiniteTimeAction);
    
    
    
    
}

//实现update方法
//参数dt:上一次调用这个函数到本次调用这个函数之间间隔多少秒
void HelloWorld::update(float dt)
{
    //printf("update\n");
    
    //获取精灵1的坐标值
    Vec2 sprite1Positin = _spriteButton1->getPosition();
    
    //接触到了键盘之后了
    if (sprite1Positin == Vec2(_spriteButton1->getContentSize().width/2 + WIN_ORIGIN.x,WIN_ORIGIN.y + _leftKeySprite->getContentSize().height))
    {
        printf("接触到了键盘\n");
    }


}

void HelloWorld::ActionDone()
{
    
}
//点击右下角的退出按钮的触发事件
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

