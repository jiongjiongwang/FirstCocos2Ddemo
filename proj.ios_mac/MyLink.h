//
//  MyLink.hpp
//  MyFirstDemo
//
//  Created by dn210 on 16/12/21.
//
//

#ifndef MyLink_hpp
#define MyLink_hpp

#include <stdio.h>

#endif /* MyLink_hpp */

#include "cocos2d.h"


//定义一个结构体，用于传递持续时间和音符号
struct LinkEventData
{
    //1-音符的持续时间
    float duraTime;
    //2-音符号
    int pianoNum;
    
    //指向下一个结构体的指针
    struct LinkEventData *next;
};


class TempLinkOC : public cocos2d::Ref
{
public:
    
    
    float _getPlayTime;
    
    int midiIndex;
    
    //有关通知的方法
    /*
    void Play(Ref* sender);
    
    void DTTime(Ref* sender);
    
    void PlayMIDI(Ref* sender);
    
    void DeleteEvent(Ref* sender);
    */
    
    
    
    void addObserver();
    
    
    
    
    //关掉所有音的方法(静态方法)
    static void CloseAllSound();
    
    
    //外接声明一个方法，用于接收外来的播放时间和间隔时间,用于生成事件
    void CreateEvent(float playTime,float dtTime);
    
    
    //外接一个方法，用于接收外来的播放时间和间隔时间,用于生成事件，并返回生成事件结构体
    LinkEventData* CreateEventGetData(float playTime,float dtTime);
    
    
    //外接声明一个方法，用接收外来的事件索引来播放事件
    void PlayEvent(int midiIndex);
    
    //外接一个方法，用于接收外来的事件索引来删除某个事件
    void DeleteEvent(int midiIndex);
    
    
    
    
};
