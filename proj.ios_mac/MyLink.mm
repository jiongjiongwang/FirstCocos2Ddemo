//
//  MyLink.cpp
//  MyFirstDemo
//
//  Created by dn210 on 16/12/21.
//
//

#include "MyLink.h"
#include "TheEventData.h"

#import "RootViewController.h"



//声明cocos2d的命名空间
//#define USING_NS_CC  using namespace cocos2d
USING_NS_CC;

//声明方法
int GetPianoNumWithMidiCode(NSString *midiCode);

//事件数组，用于存放要播放的事件
NSMutableArray<ChunkEvent *> *tempEventArray;





//根据播放事件和间隔时间来生成事件
void TempLinkOC::CreateEvent(float playTime,float dtTime)
{

    RootViewController *rootVC = kRootViewController;
    
    //当获取到播放时间之后，根据播放时间来生成事件数组
    NSMutableArray *pianoEventArray  =  [rootVC.playMusic PlayForGameWithStartTime:playTime andEndTime:playTime + dtTime];
    
    
    //获取到了当前时间的事件数组
    //判断事件数组是否有数据
    if(pianoEventArray.count > 0)
    {
        for (ChunkEvent *pianoEvent in pianoEventArray)
        {
            //1-精灵的长度由事件的持续时间来决定
            //取出事件的持续时间(float)
            float duratime = pianoEvent.eventDuration;
            
            
            //2-精灵的x轴的位置由播放音符来决定
            //取出钢琴事件的音符号(int)
            NSString *midiCode = pianoEvent.midiCode;
            
            int pianoNum =  GetPianoNumWithMidiCode(midiCode);
            
            
            
            if (pianoNum < 64)
            {
                //将持续时间(duratime:float和钢琴事件的音符号pianoNum:int)打包发送给HelloWorldScene
                //printf("mm-->当前事件的持续时间是%f,音符号是%d\n",duratime,pianoNum);
                auto eventData = MyEventData::MyEventData();
                
                eventData.eventDudration = duratime;
                
                eventData.eventPianoNum = pianoNum;
                
                cocos2d::__NotificationCenter::getInstance()->postNotification("EventNum",&eventData);
                
                
                //同时将当前的事件存入到待播放事件数组中
                [tempEventArray addObject:pianoEvent];
            }
        }
        
    }
    else if (pianoEventArray == nil)
    {
        
#warning 发送通知,使停止播放
       cocos2d::__NotificationCenter::getInstance()->postNotification("CreateNoEvent",NULL);
    }
    
}

//外接一个方法，用于接收外来的播放时间和间隔时间,用于生成事件，并返回生成事件结构体
LinkEventData* TempLinkOC::CreateEventGetData(float playTime,float dtTime)
{
    //声明一个结构体指针
    struct LinkEventData *head,*p,*q = NULL;
    
    head = NULL;
    
    RootViewController *rootVC = kRootViewController;
    
    //当获取到播放时间之后，根据播放时间来生成事件数组
    NSMutableArray *pianoEventArray  =  [rootVC.playMusic PlayForGameWithStartTime:playTime andEndTime:playTime + dtTime];
    
    if (pianoEventArray == nil)
    {
#warning 发送通知,使停止播放
        cocos2d::__NotificationCenter::getInstance()->postNotification("CreateNoEvent",NULL);
    }
    else
    {
        //获取到了当前时间的事件数组
        //判断事件数组是否有数据
        if(pianoEventArray.count > 0)
        {
            for (ChunkEvent *pianoEvent in pianoEventArray)
            {
                //1-精灵的长度由事件的持续时间来决定
                //取出事件的持续时间(float)
                float duratime = pianoEvent.eventDuration;
                
                
                //2-精灵的x轴的位置由播放音符来决定
                //取出钢琴事件的音符号(int)
                NSString *midiCode = pianoEvent.midiCode;
                
                int pianoNum =  GetPianoNumWithMidiCode(midiCode);
                
                
                
                if (pianoNum < 64)
                {
                    
                    //将持续时间(duratime:float和钢琴事件的音符号pianoNum:int)打包发送给HelloWorldScene
                    //printf("mm-->当前事件的持续时间是%f,音符号是%d\n",duratime,pianoNum);
                    
                    //动态申请一个空间，用于存放一个结点，并用临时指针指向这个结点
                    p = (struct LinkEventData *)malloc(sizeof(struct LinkEventData));
                    p->duraTime = duratime;
                    
                    p->pianoNum = pianoNum;
                    
                    p->next = NULL;
                    
                    //如果这是第一个创建的结点，则将头指针指向这个结点
                    if (head == NULL)
                    {
                        head = p;
                    }
                    //如果不是第一个创建的结点,则将上一个结点的后继指针指向当前结点
                    else
                        q->next = p;
                    
                    q = p;
                    
                    
                    //同时将当前的事件存入到待播放事件数组中
                    [tempEventArray addObject:pianoEvent];
                }
            }
            
        }
    }
    return head;
}



//外接声明一个方法，用接收外来的事件索引来播放事件
void TempLinkOC::PlayEvent(int midiIndex)
{
    
    //取出音符，播放声音
    ChunkEvent *pianoEvent = tempEventArray[midiIndex];
    
    //取出当前的音符事件有没有被播放过
    BOOL eventHasPlay = pianoEvent.isHasPlay;
    //表示之前还没有被播放
    if (eventHasPlay == NO)
    {
        //printf("音符%zu刚接触到了键盘,开始播放声音\n",i);
        //NSLog(@"播放事件为%@",pianoEvent);
        
        RootViewController *rootVC = kRootViewController;
        
        
        [rootVC.playMusic PlaySoundWithChunkEvent:pianoEvent];
        
        //已经播放了，更新播放信息
        pianoEvent.isHasPlay = YES;
    }
    
}

//外接一个方法，用于接收外来的事件索引来删除某个事件
void TempLinkOC::DeleteEvent(int midiIndex)
{
    
    [tempEventArray removeObjectAtIndex:midiIndex];
    
}








void TempLinkOC::addObserver()
{
    
    //初始化事件数组
    tempEventArray = [NSMutableArray array];
}


//静态方法(关掉所有的音)
void TempLinkOC::CloseAllSound()
{
    
    //关掉所有的音
    RootViewController *rootVC = kRootViewController;
    
    
    for (int i = 176; i <= 191; i++)
    {
        NSString *eventStatus = [NSString stringWithFormat:@"%02x",i];
        
        [rootVC.playMusic sendMIDIControlMsgWithStatus:eventStatus andData1Str:@"78" andData2Str:@"00"];
    }
    
}




//封装一个方法:根据传入的音符号(NSString)来返回键盘编号(int)
int GetPianoNumWithMidiCode(NSString *midiCode)
{
    int num = 0;
    
    //先以16为参数告诉strtoul字符串参数表示16进制数字，然后使用0x%X转为数字类型
    num = int(strtoul([midiCode UTF8String],0,16));
    
    //容错处理
    if (num < 32 || num > 95)
    {
        num = 64;
    }
    else
    {
        num = num - 32;
    }
    
    
    return num;
}

