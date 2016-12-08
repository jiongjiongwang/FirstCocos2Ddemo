#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

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
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    
    
    //场景的坐标:x(x轴)和y(y轴)
    //当前的x:-0.0000152587891 y:24.7887268
    //OpenGL坐标系:原点在左下角
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    

    //添加关闭按钮到菜单层上
    //MenuItemImage类->MenuItemSprite类->MenuItem类->Node类->Ref类
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    //(1)创建图片精灵(Sprite类)
    auto pressNormal = Sprite::create("HelloWorld.png");
    
    //(2)生成使用精灵图片的按钮(MenuItemSprite类)
    //MenuItemSprite类->MenuItem类->Node类->Ref类
    auto pressItem = MenuItemSprite::create(pressNormal, pressNormal, CC_CALLBACK_1(HelloWorld::ButtonPress, this));
    
    pressItem->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    

    //Menu类->Layer类->Node类->Ref类
    auto menu = Menu::create(closeItem,pressItem, NULL);
    menu->setPosition(Vec2::ZERO);
    
    //1为层级数，决定背景精灵的绘制顺序,参数数值越大，其绘制时就会越靠前
    this->addChild(menu, 1);
    
    
    
    
    //label
    /*
    // 3. add your codes below...
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 25);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    
    
    
    auto smalLabel = Label::createWithTTF("by Cocos2d", "fonts/Marker Felt.ttf", 25);
    
    // position the label on the center of the screen
    smalLabel->setPosition(Vec2(origin.x + visibleSize.width/7 * 6,
                            origin.y + visibleSize.height - label->getContentSize().height - 5));
    
    // add the label as a child to this layer
    this->addChild(smalLabel, 1);
    */
    
    /*
    //创建精灵类
    auto sprite = Sprite::create("HelloWorld.png");

    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    this->addChild(sprite, 0);
    */
    
    
    
    
    
    //精灵类1
    //精灵类Sprite->Node类->Ref类
    _spriteButton1 = Sprite::create("button_gray_disable.9.png");
    _spriteButton1->setPosition(Vec2(_spriteButton1->getContentSize().width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(_spriteButton1, 0);
    
    
    
    //精灵类2
    _spriteButton2 = Sprite::create("button_gray_disable.9.png");
    _spriteButton2->setPosition(Vec2(_spriteButton2->getContentSize().width/2 + origin.x, visibleSize.height/2 + origin.y - _spriteButton2->getContentSize().height));
    
    this->addChild(_spriteButton2, 0);
    

    
    //添加底部的键盘精灵
    //1-左
    auto leftKeySprite = Sprite::create("Left.png");
    
    leftKeySprite->setPosition(Vec2(leftKeySprite->getContentSize().width/2 + origin.x,origin.y + leftKeySprite->getContentSize().height/2));
    
    this->addChild(leftKeySprite);
    
    
    //2-中
    auto midKeySprite = Sprite::create("Mid.png");
    
    midKeySprite->setPosition(Vec2(midKeySprite->getContentSize().width/2 + origin.x + leftKeySprite->getContentSize().width,origin.y + midKeySprite->getContentSize().height/2));
    
    this->addChild(midKeySprite);
    
    //3-右
    auto rightKeySprite = Sprite::create("Right.png");
    
    rightKeySprite->setPosition(Vec2(rightKeySprite->getContentSize().width/2 + origin.x + leftKeySprite->getContentSize().width + midKeySprite->getContentSize().width,origin.y + rightKeySprite->getContentSize().height/2));
    
    this->addChild(rightKeySprite);
    
    
    
    return true;
}

void HelloWorld::ButtonPress(Ref* pSender)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    //添加动作类1
    auto actionMove1 = MoveBy::create(4.0f, Vec2(0,-visibleSize.height/2));
    //运行动作1
    _spriteButton1->runAction(actionMove1);
    
    
    //添加动作类2
    auto actionMove2 = MoveBy::create(3.0f, Vec2(0,-visibleSize.height/2 + _spriteButton2->getContentSize().height));
    
    //运行动作2
    _spriteButton2->runAction(actionMove2);
    
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
