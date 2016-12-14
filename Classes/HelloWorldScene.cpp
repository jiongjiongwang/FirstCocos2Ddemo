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
        
        
        //键盘整体左移25个键盘位
        float deltaDistance  = sp->getContentSize().width * deltaKeyNum;
        
        sp->setPosition(Vec2(sp->getContentSize().width * i
                             + WIN_ORIGIN.x - deltaDistance,WIN_ORIGIN.y + sp->getContentSize().height));
        sp->setVisible(true);
        this->addChild(sp,0);
        
        //添加到数组中
        _keySpriteArray[i] = sp;
        
        //m_vecKeyNormal.push_back(sp);
    }
    
    
    
    
    //初始化为不在播放
    _isPlay = false;
    
    //初始化记时时间
    _playTime = 0.0f;
    
    _isContactFlag = false;
    
    _isCreateEventFlag0 = false;
    
    _isCreateEventFlag1 = false;
    
    _isCreateEventFlag2 = false;
    
    _isCreateEventFlag3 = false;
    
    
    this->scheduleUpdate();
    
    
    
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
    
    
    printf("开始了\n");
    
    //更新正在播放
    _isPlay = true;
    
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
        
        //传入时间作为参数点
        if (_playTime >= 0.0f && _isCreateEventFlag0 == false)
        {
            //在此动态地根据时间来生成精灵，并存入精灵数组中
            //在生成精灵之前先求出精灵的长度(根据精灵的播放时间，当前假设每一个播放时间都为0.15秒)
            //精灵的长度由事件的持续时间来设置
            //float spriteHeight = 0.15 * preDistance / preActionTime;
            
            //printf("0.15秒的长度是%f\n",spriteHeight);
            
            //1-生成
            //精灵类Sprite->Node类->Ref类
            auto spriteMove = Sprite::create("spriteButton.png");
            //设置锚点(锚点下移)
            spriteMove->setAnchorPoint(Vec2(0.5f,0.0f));
            //位置坐标(锚点的坐标)
            //位置坐标由音符号来设置(音符号--->钢琴琴键的索引值---->精灵的x坐标位置)
            spriteMove->setPosition(Vec2(_keySpriteArray[28]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[28]->getPosition().x, WIN_SIZE.height + WIN_ORIGIN.y));
            
            
            
            
            //设置颜色为红色
            spriteMove->setColor(Color3B(255, 0, 0));
            this->addChild(spriteMove, 0);
            
            
            
            //2-运动
            //开始做运动
            //数组中的动作(生成之后马上开始动作)
            //1-移动动作(前戏移动)
            auto preActionMoveTo = MoveTo::create(preActionTime, Vec2(_keySpriteArray[28]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[28]->getPosition().x, preDistance));
            
            //2-入戏运动
            auto pressActionMoveTo = MoveTo::create(0.15, Vec2(_keySpriteArray[28]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[28]->getPosition().x, preDistance - spriteMove->getContentSize().height));
            
            //3-顺序动作
            auto sequenceMove = Sequence::create(preActionMoveTo,pressActionMoveTo, NULL);
            
            //运行两个动作
            spriteMove->runAction(sequenceMove);
            
            
            //添加到数组中
            _spriteArray[0] = spriteMove;
            
            
            
            _isCreateEventFlag0 = true;
        }
        if (_playTime >= 0.15f && _isCreateEventFlag1 == false)
        {
            //在此动态地根据时间来生成精灵，并存入精灵数组中
            //在生成精灵之前先求出精灵的长度(根据精灵的播放时间，当前假设每一个播放时间都为0.15秒)
            //精灵的长度由事件的持续时间来设置
            //float spriteHeight = 0.15 * preDistance / preActionTime;
            
            //printf("0.15秒的长度是%f\n",spriteHeight);
            
            //1-生成
            //精灵类Sprite->Node类->Ref类
            auto spriteMove = Sprite::create("spriteButton.png");
            //设置锚点(锚点下移)
            spriteMove->setAnchorPoint(Vec2(0.5f,0.0f));
            //位置坐标(锚点的坐标)
            //位置坐标由音符号来设置(音符号--->钢琴琴键的索引值---->精灵的x坐标位置)
            spriteMove->setPosition(Vec2(_keySpriteArray[26]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[26]->getPosition().x, WIN_SIZE.height + WIN_ORIGIN.y));
            
            //设置颜色为红色
            spriteMove->setColor(Color3B(255, 0, 0));
            this->addChild(spriteMove, 0);
            
            
            
            //2-运动
            //开始做运动
            //数组中的动作(生成之后马上开始动作)
            //1-移动动作(前戏移动)
            auto preActionMoveTo = MoveTo::create(preActionTime, Vec2(_keySpriteArray[26]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[26]->getPosition().x, preDistance));
            
            //2-入戏运动
            auto pressActionMoveTo = MoveTo::create(0.15, Vec2(_keySpriteArray[26]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[26]->getPosition().x, preDistance - spriteMove->getContentSize().height));
            
            //3-顺序动作
            auto sequenceMove = Sequence::create(preActionMoveTo,pressActionMoveTo, NULL);
            
            //运行两个动作
            spriteMove->runAction(sequenceMove);
            
            //添加到数组中
            _spriteArray[1] = spriteMove;
            
            _isCreateEventFlag1 = true;
        }
        if (_playTime >= 0.30f && _isCreateEventFlag2 == false)
        {
            //在此动态地根据时间来生成精灵，并存入精灵数组中
            //在生成精灵之前先求出精灵的长度(根据精灵的播放时间，当前假设每一个播放时间都为0.15秒)
            //精灵的长度由事件的持续时间来设置
            //float spriteHeight = 0.15 * preDistance / preActionTime;
            
            //printf("0.15秒的长度是%f\n",spriteHeight);
            
            //1-生成
            //精灵类Sprite->Node类->Ref类
            auto spriteMove = Sprite::create("spriteButton.png");
            //设置锚点(锚点下移)
            spriteMove->setAnchorPoint(Vec2(0.5f,0.0f));
            //位置坐标(锚点的坐标)
            //位置坐标由音符号来设置(音符号--->钢琴琴键的索引值---->精灵的x坐标位置)
            spriteMove->setPosition(Vec2(_keySpriteArray[25]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[25]->getPosition().x, WIN_SIZE.height + WIN_ORIGIN.y));
            
            //设置颜色为红色
            spriteMove->setColor(Color3B(255, 0, 0));
            this->addChild(spriteMove, 0);
            
            
            
            //2-运动
            //开始做运动
            //数组中的动作(生成之后马上开始动作)
            //1-移动动作(前戏移动)
            auto preActionMoveTo = MoveTo::create(preActionTime, Vec2(_keySpriteArray[25]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[25]->getPosition().x, preDistance));
            
            //2-入戏运动
            auto pressActionMoveTo = MoveTo::create(0.15, Vec2(_keySpriteArray[25]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[25]->getPosition().x, preDistance - spriteMove->getContentSize().height));
            
            //3-顺序动作
            auto sequenceMove = Sequence::create(preActionMoveTo,pressActionMoveTo, NULL);
            
            //运行两个动作
            spriteMove->runAction(sequenceMove);
            
            //添加到数组中
            _spriteArray[2] = spriteMove;
            
            _isCreateEventFlag2 = true;
        }
        if (_playTime >= 0.45f && _isCreateEventFlag3 == false)
        {
            //在此动态地根据时间来生成精灵，并存入精灵数组中
            //在生成精灵之前先求出精灵的长度(根据精灵的播放时间，当前假设每一个播放时间都为0.15秒)
            //精灵的长度由事件的持续时间来设置
            //float spriteHeight = 0.15 * preDistance / preActionTime;
            
            //printf("0.15秒的长度是%f\n",spriteHeight);
            
            //1-生成
            //精灵类Sprite->Node类->Ref类
            auto spriteMove = Sprite::create("spriteButton.png");
            //设置锚点(锚点下移)
            spriteMove->setAnchorPoint(Vec2(0.5f,0.0f));
            //位置坐标(锚点的坐标)
            //位置坐标由音符号来设置(音符号--->钢琴琴键的索引值---->精灵的x坐标位置)
            spriteMove->setPosition(Vec2(_keySpriteArray[26]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[26]->getPosition().x, WIN_SIZE.height + WIN_ORIGIN.y));
            
            //设置颜色为红色
            spriteMove->setColor(Color3B(255, 0, 0));
            this->addChild(spriteMove, 0);
            
            
            
            //2-运动
            //开始做运动
            //数组中的动作(生成之后马上开始动作)
            //1-移动动作(前戏移动)
            auto preActionMoveTo = MoveTo::create(preActionTime, Vec2(_keySpriteArray[26]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[26]->getPosition().x, preDistance));
            
            //2-入戏运动
            auto pressActionMoveTo = MoveTo::create(0.15, Vec2(_keySpriteArray[26]->getContentSize().width/2 + WIN_ORIGIN.x + _keySpriteArray[26]->getPosition().x, preDistance - spriteMove->getContentSize().height));
            
            //3-顺序动作
            auto sequenceMove = Sequence::create(preActionMoveTo,pressActionMoveTo, NULL);
            
            //运行两个动作
            spriteMove->runAction(sequenceMove);
            
            //添加到数组中
            _spriteArray[3] = spriteMove;
            
            
            _isCreateEventFlag3 = true;
        }
        
        _playTime += dt;        
    }
    
    //遍历还在精灵数组中的精灵
    for (int i = 0; i < 4; i++)
    {
        auto spriteMove = _spriteArray[i];
        
        //判断取出来的精灵是否有值
        if (spriteMove != NULL)
        {
         
            //获取精灵的坐标值
            Vec2 sprite1Positin = spriteMove->getPosition();
            
            //当长方的精灵恰好接触到了键盘之后了(只记录一次)
            if ((sprite1Positin.y <= WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height)&&(sprite1Positin.y> WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height - spriteMove->getContentSize().height)&&_isContactFlag == false)
            {
                printf("音符%d刚接触到了键盘,开始播放声音\n",i);
                
                _isContactFlag = true;
            }
            //当播放结束时
            else if ((sprite1Positin.y + spriteMove->getContentSize().height<= WIN_ORIGIN.y + _keySpriteArray[0]->getContentSize().height)&&_isContactFlag == true)
            {
                printf("当前音符%d播放结束\n",i);
                
                _isContactFlag = false;
                
                //播放结束
                _isPlay = false;
                
                //将button从界面上移除
                this->removeChild(spriteMove);
                
                _spriteArray[i] = NULL;
            }
            
        }
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

