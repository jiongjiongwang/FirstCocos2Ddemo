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

class TempLinkOC : public cocos2d::Ref
{
public:
    
    
    float _getPlayTime;
    
    int midiIndex;
    
    void Play(Ref* sender);
    
    void DTTime(Ref* sender);
    
    void PlayMIDI(Ref* sender);
    
    void DeleteEvent(Ref* sender);
    
    void addObserver();
    
    //关掉所有音的方法(静态方法)
    static void CloseAllSound();
    
    //外接声明一个方法，用于接收外来的播放时间和间隔时间,用于生成事件
    void CreateEvent(float playTime,float dtTime);
    
    //外接声明一个方法，用接收外来的事件索引来播放事件
    void PlayEvent(int midiIndex);
    
    //外接一个方法，用于接收外来的事件索引来删除某个事件
    void DeleteEvent(int midiIndex);
    
    
};
