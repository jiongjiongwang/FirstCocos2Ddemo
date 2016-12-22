#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "TheEventData.h"
#include "MyLink.h"


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

    
    //添加stop停止按钮
    _pressPauseLabel = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf",25);
    
    //初始时不显示暂停label
    _pressPauseLabel->setOpacity(0);
    
    //利用labelStop对象生成MenuItemLabel类对象
    //MenuItemLabel类->MenuItem类->Node类->Ref类
    auto labelPausePress = MenuItemLabel::create(_pressPauseLabel, CC_CALLBACK_1(HelloWorld::ButtonPausePress, this));
    
    //设置锚点
    labelPausePress->setAnchorPoint(Vec2(0.5f,1.0f));
    
    labelPausePress->setPosition(Vec2(WIN_SIZE.width/2 + WIN_ORIGIN.x, WIN_SIZE.height + WIN_ORIGIN.y));
    
    
    
    //3-Menu类:负责管理场景上的UI控件
    //Menu类->Layer类->Node类->Ref类
    //auto menu = Menu::create(closeItem,labelPress, NULL);
    auto menu = Menu::create(labelPress,labelPausePress,NULL);
    menu->setPosition(Vec2::ZERO);
    
    //1为层级数，决定背景精灵的绘制顺序,参数数值越大，其绘制时就会越靠前
    this->addChild(menu, 1);
    
    
    
    
    
    //2-先生成底部的静止的键盘精灵，在生成上面的运动的长方形精灵
    
    //循环添加底部的键盘
    for (int i = 0; i < 63; i++)
    {
        auto sp = Sprite::create("Mid.png");
        sp->setAnchorPoint(Vec2(0.0f,1.0f));
        
        //键盘整体左移25个键盘位
        float deltaDistance  = sp->getContentSize().width * deltaKeyNum;
        
        
        sp->setPosition(Vec2(sp->getContentSize().width * i
                             + WIN_ORIGIN.x - deltaDistance,WIN_ORIGIN.y + sp->getContentSize().height));
        sp->setVisible(true);
        this->addChild(sp,1);
        
        //添加到数组中
        _keySpriteArray[i] = sp;
    }
    
    
    //初始化为不在播放
    _isPlay = false;
    
    _isHasPause = false;
    
    //初始化记时时间
    _playTime = 0.0f;
    
    
    this->scheduleUpdate();
    
    
    
    
    //创建一个Link实例
    auto LinkInstance = TempLinkOC::TempLinkOC();
    
    LinkInstance.addObserver();
    
    
    
#warning 接收的通知1:接受TempLinkOC处理好的钢琴事件的通知
    cocos2d::__NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(HelloWorld::GetEvent),"EventNum", NULL);
    
    
    return true;
}

void HelloWorld::ButtonPress(Ref* pSender)
{
    
    //2-label添加动作类:淡入淡出动作
    //2.1 淡出动作:透明度从255逐渐变为0从而在屏幕上消失
    auto fadeOutAction = FadeOut::create(0.5f);
    
    _pressLabel->runAction(fadeOutAction);
    
    printf("开始了\n");
    
    
    //重置所有精灵容器中的精灵运动
    if (_isHasPause)
    {
        //遍历在精灵容器内的精灵
        size_t len = m_vecSprite.size();
        
        for (size_t i =0; i < len; i ++)
        {
            auto spriteMove = m_vecSprite[i];
            
            if (spriteMove != NULL)
            {
                spriteMove->resume();
            }
        }
    }
    
    //更新正在播放
    _isPlay = true;
    
    //暂停按钮显示
    _pressPauseLabel->setOpacity(255);

    
}

//暂停点击按钮触发事件
void HelloWorld::ButtonPausePress(Ref* pSender)
{
    printf("点击了暂停\n");
    
    
    //暂停按钮消失
    _pressPauseLabel->setOpacity(0);
    
    
    //2-label添加动作类:淡入淡出动作
    //2.1 淡出动作:透明度从0逐渐变为255从而在屏幕上消失
    auto fadeOutAction = FadeIn::create(0.5f);
    
    _pressLabel->runAction(fadeOutAction);
    
    
    
    //更新正在暂停
    _isPlay = false;
    
    
    //表示已经暂停过至少一次
    _isHasPause = true;
     
    //停止所有精灵容器中的精灵运动
    //遍历在精灵容器内的精灵
    size_t len = m_vecSprite.size();
    
    for (size_t i =0; i < len; i ++)
    {
        auto spriteMove = m_vecSprite[i];
        
        if (spriteMove != NULL)
        {
            spriteMove->pause();
        }
    }
    
    
    //关掉所有的音
    //静态方法:关掉所有的音
    TempLinkOC::CloseAllSound();
}

void HelloWorld::GetEvent(Ref* sender)
{
    
    auto myEvent = (MyEventData *)sender;
    
    //持续时间
    float duratime = myEvent->eventDudration;
    
    //根据持续时间来得出长度
    float spriteHeight = duratime * (WIN_SIZE.height - _keySpriteArray[0]->getContentSize().height) / preActionTime;
    
    //音符号
    int pianoNum = myEvent->eventPianoNum;
    
    //利用持续时间和音符号来创建长方形精灵
    //1-生成
    //精灵类Sprite->Node类->Ref类
    auto spriteMove = Sprite::create("spriteButton.png");
    
    
    
    //设置锚点(锚点下移)
    spriteMove->setAnchorPoint(Vec2(0.5f,0.0f));
    //位置坐标(锚点的坐标)
    //位置坐标由音符号来设置(音符号--->钢琴琴键的索引值---->精灵的x坐标位置)
    spriteMove->setPosition(Vec2(_keySpriteArray[pianoNum]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[pianoNum]->getPosition().x, WIN_SIZE.height + WIN_ORIGIN.y));
    
    
    
    
    //得出精灵的原长度
    float oldSpriteHeight = spriteMove->getContentSize().height;
    
    //printf("精灵的原长度%f\n",oldSpriteHeight);
    
    //求出需要放大/缩小的倍数
    float ratio = spriteHeight/oldSpriteHeight;
    
    //精灵放大
    spriteMove->setScaleY(ratio);
    
    //printf("精灵的现在的长度%f\n",spriteMove->getContentSize().height);
    
    //精灵放大的同时也要把contentsize设置一下
    spriteMove->setContentSize(cocos2d::Size(spriteMove->getContentSize().width,spriteHeight));
    
    
    
    //设置颜色为红色
    spriteMove->setColor(Color3B(255, 0, 0));
    this->addChild(spriteMove, 0);
    
    
    
    //2-运动
    //开始做运动
    //数组中的动作(生成之后马上开始动作)
    //1-移动动作(前戏移动)
    auto preActionMoveTo = MoveTo::create(preActionTime, Vec2(_keySpriteArray[pianoNum]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[pianoNum]->getPosition().x, preDistance));
    
    
    
    //2-入戏运动
    auto pressActionMoveTo = MoveTo::create(duratime, Vec2(_keySpriteArray[pianoNum]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[pianoNum]->getPosition().x, preDistance - spriteHeight));
    
    //3-顺序动作
    auto sequenceMove = Sequence::create(preActionMoveTo,pressActionMoveTo, NULL);
    
    //运行两个动作
    spriteMove->runAction(sequenceMove);
    
    
    
    //将生成的精灵存入到精灵容器中
    m_vecSprite.push_back(spriteMove);
    
    
}


//实现update方法
//参数dt:上一次调用这个函数到本次调用这个函数之间间隔多少秒
//时时刻刻都在调用这个方法进行界面帧的刷新
void HelloWorld::update(float dt)
{
    
    //如果已经开始播放了
    //已经开始播放的情况下记时开始
    if (_isPlay)
    {
        //printf("播放时间%f\n",_playTime);
        
     
#warning 发送的通知1:发送播放时间和间隔时间的通知
        //1-发送当前的播放时间
        auto nowPlayTime = __Float::create(_playTime);
    
        
        __NotificationCenter::getInstance()->postNotification("PlayTime",nowPlayTime);
        
        
        //2-发送间隔时间
        auto nowDTTime = __Float::create(dt);
        
        __NotificationCenter::getInstance()->postNotification("DTTime",nowDTTime);
        
        
        
        
        
        _playTime += dt;
    }
    
    //2-判断长方形的接触(是否接触到了键盘钢琴)
    //遍历还在精灵容器内的精灵

    //size_t len = m_vecSprite.size();

    for (size_t i =0; i < m_vecSprite.size(); i ++)
    {
        auto spriteMove = m_vecSprite[i];
        
        
        //判断精灵是否有值
        //判断取出来的精灵是否有值(判断精灵数组是否为空)
        if (spriteMove != NULL)
        {
            //获取精灵的坐标值
            Vec2 sprite1Positin = spriteMove->getPosition();
            
            
            //直接取出精灵的长度
            float spriteHeight = spriteMove->getContentSize().height;
            
            
            
            //当长方的精灵恰好接触到了键盘之后了(只记录一次)
            if ((sprite1Positin.y <= WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height)&&(sprite1Positin.y> WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height - spriteHeight))
            {
                
                
#warning 发送的通知2:发i通知给TempLinkOC 让其播放音符的通知
                auto nowPlayMIDI = __Integer::create((int)i);
                
                
                
                __NotificationCenter::getInstance()->postNotification("PlayMIDI",nowPlayMIDI);
                
            }
            //当播放结束时
            else if ((sprite1Positin.y + spriteHeight<= WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height))
            {
                //printf("当前音符播放结束\n");
                
                //将button从界面上移除
                this->removeChild(spriteMove);
                
                
                //将当前的精灵从容器中删除
                m_vecSprite.erase(m_vecSprite.begin() + i);
                
                
                
#warning 发送的通知3:发送删除事件数组中某个事件的通知
                auto nowDeleteMIDI = __Integer::create((int)i);
                
                __NotificationCenter::getInstance()->postNotification("DeleteEvent",nowDeleteMIDI);
                
                //删除一个，i不更新为i++
                i--;
            }
            
        }
        
        
    }
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

