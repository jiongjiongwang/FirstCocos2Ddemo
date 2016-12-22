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
    
    
};
