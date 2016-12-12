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
    
    
    

    //添加关闭按钮到菜单层上
    //MenuItemImage类->MenuItemSprite类->MenuItem类->Node类->Ref类
    /*
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(WIN_ORIGIN.x + WIN_SIZE.width - closeItem->getContentSize().width/2 ,
                                WIN_ORIGIN.y + closeItem->getContentSize().height/2));
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
    //auto menu = Menu::create(closeItem,labelPress, NULL);
    auto menu = Menu::create(labelPress, NULL);
    menu->setPosition(Vec2::ZERO);
    
    //1为层级数，决定背景精灵的绘制顺序,参数数值越大，其绘制时就会越靠前
    this->addChild(menu, 1);
    
    
    
    //2-先生成底部的静止的键盘精灵，在生成上面的运动的长方形精灵
    
    //循环添加底部的键盘
    for (int i = 0; i < 63; i++)
    {
        auto sp = Sprite::create("Mid.png");
        sp->setAnchorPoint(Vec2(0.0f,1.0f));
        sp->setPosition(Vec2(sp->getContentSize().width * i
                             + WIN_ORIGIN.x,WIN_ORIGIN.y + sp->getContentSize().height));
        sp->setVisible(true);
        this->addChild(sp,0);
        
        //添加到数组中
        _keySpriteArray[i] = sp;
        
        //m_vecKeyNormal.push_back(sp);
    }
    
    
    //精灵类1(与左边的键盘对齐)
    //精灵类Sprite->Node类->Ref类
    _spriteButton1 = Sprite::create("button_gray_disable.9.png");
    //设置锚点
    _spriteButton1->setAnchorPoint(Vec2(0.5f,0.5f));
    //位置坐标(锚点的坐标)
    _spriteButton1->setPosition(Vec2(_keySpriteArray[0]->getContentSize().width/2 + WIN_ORIGIN.x, WIN_SIZE.height/2 + WIN_ORIGIN.y));
    
    //设置颜色为红色
    _spriteButton1->setColor(Color3B(255, 0, 0));
    this->addChild(_spriteButton1, 0);
    
    
    
    
    
    this->scheduleUpdate();
    
    _isContactFlag = false;
    
    return true;
}

void HelloWorld::ButtonPress(Ref* pSender)
{
    
    //2-label添加动作类:淡入淡出动作
    //2.1 淡出动作:透明度从255逐渐变为0从而在屏幕上消失
    auto fadeOutAction = FadeOut::create(0.5f);
    
    _pressLabel->runAction(fadeOutAction);
    //移除开始label
    this->removeChild(_pressLabel);
    
    
    
    //1-精灵1的动作
    //1.1-添加动作类:移动动作
    auto actionMoveTo = MoveTo::create(4.0f, Vec2(_spriteButton1->getContentSize().width/2 + WIN_ORIGIN.x,WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height - _spriteButton1->getContentSize().height/2));
    
    
    //运行动作1
    _spriteButton1->runAction(actionMoveTo);
    
    
}

//实现update方法
//参数dt:上一次调用这个函数到本次调用这个函数之间间隔多少秒
void HelloWorld::update(float dt)
{
    //获取精灵1的坐标值
    Vec2 sprite1Positin = _spriteButton1->getPosition();
    
    //当长方的精灵恰好接触到了键盘之后了(只记录一次)
    if ((sprite1Positin.y <= WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height + _spriteButton1->getContentSize().height/2)&&(sprite1Positin.y> WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height - _spriteButton1->getContentSize().height/2)&&_isContactFlag == false)
    {
        printf("刚接触到了键盘,开始播放声音\n");
        
        _isContactFlag = true;
    }
    //当播放结束时
    else if ((sprite1Positin.y + _spriteButton1->getContentSize().height/2 <= WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height)&&_isContactFlag == true)
    {
        printf("当前音符播放结束\n");
        
        _isContactFlag = false;
        
        //将button从界面上移除
        //this->removeChild(_spriteButton1);
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

