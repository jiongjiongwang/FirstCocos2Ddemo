//
//  PlayMusic.m
//  Chunk
//
//  Created by dn210 on 16/11/9.
//  Copyright © 2016年 dn210. All rights reserved.
//

#import "PlayMusic.h"


@interface PlayMusic()


@property (nonatomic,strong)ChunkHeader *chunkHead;

//定义一个数组记录一下MIDI文件中所有5103事件的数组
@property (nonatomic,strong)NSArray<FF5103ChunkEvent *> *ff5103Array;


//一个大的MIDI文件分成多个轨道块，用数组保存这些轨道块
@property (nonatomic,strong)NSArray<MTRKChunk *> *mtrkArray;


//定义一个全局属性记录一下当前MIDI的总时间
@property (nonatomic,assign)float midiAllTime;


//一个MIDI文件在内存中只存在一个NSData对象
@property (nonatomic,strong)NSData *midiData;


//播放音乐的Sampler对象
@property (nonatomic,strong)MIDISampler *sampler;


@property (nonatomic,strong)NSTimer *timer;


//是否开始播放
@property (nonatomic,assign)BOOL isPlay;


//轨道索引数组
@property (nonatomic,strong)NSMutableArray<NSNumber *> *chunkIndexArray;

//播放开始的时刻
@property (nonatomic,strong)NSDate *startTime;


//定义一个可变数组用来存放按下键盘的事件
@property (nonatomic,strong)NSMutableArray<ChunkEvent *> *pressKeyArray;


@end




@implementation PlayMusic


+(instancetype)PlayMusicWithChunkHead:(ChunkHeader *)chunkHead andff5103Array:(NSArray<FF5103ChunkEvent *> *)ff5103Array andMTRKArray:(NSArray<MTRKChunk *> *)mtrkArray
                       andMidiAllTime:(float)midiAllTime
                          andMidiData:(NSData *)midiData
{
    PlayMusic *playOrPause = [[PlayMusic alloc] init];
    
    //轨道头
    playOrPause.chunkHead = chunkHead;
    
    //MIDI中的5103数组
    playOrPause.ff5103Array = ff5103Array;
    
    //一个大的MIDI文件分成多个轨道块，用数组保存这些轨道块
    playOrPause.mtrkArray = mtrkArray;
    
    //定义一个全局属性记录一下当前MIDI的总时间
    playOrPause.midiAllTime = midiAllTime;
    
    //一个MIDI文件在内存中只存在一个NSData对象
    playOrPause.midiData = midiData;
    
    
    playOrPause.sampler = [[MIDISampler alloc] init];
    
    return playOrPause;
}


//懒加载轨道索引数组
-(NSMutableArray<NSNumber *>*)chunkIndexArray
{
    if (_chunkIndexArray == nil)
    {
        
        _chunkIndexArray = [NSMutableArray array];
        
        for (NSUInteger i = 0; i < _chunkHead.chunkNum; i++)
        {
            [_chunkIndexArray addObject:@0];
        }
        
        
    }
    
    return _chunkIndexArray;
}


//懒加载存放按下键盘事件的数组
-(NSMutableArray<ChunkEvent *>*)pressKeyArray
{
    if (_pressKeyArray == nil)
    {
        _pressKeyArray = [NSMutableArray array];
    }
    
    return _pressKeyArray;
}

//钢琴事件数组(随着不同的时间会不断地变化)
-(NSMutableArray<ChunkEvent *> *)tempEventArray
{
    if (_tempEventArray == nil)
    {
        _tempEventArray = [NSMutableArray array];
    }
    
    return _tempEventArray;
}

-(void)PlayMIDIMultiTempMusic
{
    _isPlay = NO;
    
    _timer = [NSTimer scheduledTimerWithTimeInterval:0.001 target:self selector:@selector(TimeGo) userInfo:nil repeats:YES];
}

-(void)TimeGo
{
    
    //时刻之间的差值
    NSTimeInterval secondsInterval = 0;
    
    if (_isPlay == NO)
    {
        //开始的时刻
        _startTime = [NSDate date];
        NSLog(@"开始播放");
    }
    else
    {
        //记录一下当前的时刻
        NSDate *nowTime = [NSDate date];
        
        //时刻之间相减
        secondsInterval= [nowTime timeIntervalSinceDate:_startTime];
    }
    
    //时刻之间差值
    //NSLog(@"secondsInterval=  %0.3f",secondsInterval);
    //当在这个时间差时
    if (secondsInterval > self.midiAllTime)
    {
        [_timer invalidate];
        
        _timer = nil;
        
        NSLog(@"播放结束%f",secondsInterval);
        
        return;
    }

    
    
   
        
    
        NSMutableArray *mEventArray;
    
        
        //生成事件数组
        //传入一个事件范围,返回一个事件数组
        mEventArray = [self GetEventArrayWithTime:secondsInterval andendTime:secondsInterval+0.001 andIndexArray:self.chunkIndexArray];
        
        if (mEventArray.count >= 1)
        {
            
            //播放音乐
            [self PlaySoundWithArray:mEventArray];
            
        }
    
    _isPlay = YES;
}



//预加载1:求出每一个事件的时间
-(void)CaculateTheEventTime
{
    
    //定义一个数组来记录一下每一轨道的索引信息(4分音符范围的数组)
    //所在4分音符的终点
    //终点
    NSUInteger quartChunkIndex[_chunkHead.chunkNum];
    
    memset(quartChunkIndex, 0, sizeof(quartChunkIndex));
    
    
    
    for (NSUInteger k = 0; k < self.ff5103Array.count; k++)
    {
        
        //1-轨道要全部遍历结束
        for (NSUInteger i = 0; i < _chunkHead.chunkNum; i++)
        {
            
            //2-每一个轨道的事件不需要全部遍历
            for (NSUInteger j = quartChunkIndex[i]; j < self.mtrkArray[i].chunkEventArray.count; j++)
            {
                ChunkEvent *chunkEvent = self.mtrkArray[i].chunkEventArray[j];
                
                //5103数组最后一个或当前MIDI文件只有一个5103
                if (k == self.ff5103Array.count -1)
                {
                    
                    //计算每个事件的时刻
                    //即时计算总时间
                    float theTime = 0.00000000;
                    
                    NSUInteger quartTime = self.ff5103Array[k].theQuartTime;
                    
                    
                    theTime = (float)((float)(chunkEvent.eventAllDeltaTime - self.ff5103Array[k].eventAllDeltaTime)/(float)_chunkHead.tickNum) * quartTime *0.00100 * 0.00100 + self.ff5103Array[k].eventPlayTime;
                    
                    //即时的总时长
                    chunkEvent.eventPlayTime = theTime;
                    
                    
                    quartChunkIndex[i] = self.mtrkArray[i].chunkEventArray.count - 1;
                }
                else
                {
                    if (chunkEvent.eventAllDeltaTime > self.ff5103Array[k].eventAllDeltaTime && chunkEvent.eventAllDeltaTime <= self.ff5103Array[k + 1].eventAllDeltaTime)
                    {
                        
                        //计算每个事件的时刻
                        //即时计算总时间
                        float theTime = 0.00000000;
                        
                        NSUInteger quartTime = self.ff5103Array[k].theQuartTime;
                        
                        
                        theTime = (float)((float)(chunkEvent.eventAllDeltaTime - self.ff5103Array[k].eventAllDeltaTime)/(float)_chunkHead.tickNum) * quartTime *0.00100 * 0.00100 + self.ff5103Array[k].eventPlayTime;
                        
                        //即时的总时长
                        chunkEvent.eventPlayTime = theTime;
                        
                    }
                    else if (chunkEvent.eventAllDeltaTime > self.ff5103Array[k + 1].eventAllDeltaTime)
                    {
                        //超过了当前的4分范围了，4分时间要更新
                        quartChunkIndex[i] = j;
                        
                        break;
                    }
                }
            }
        }
    }

    NSLog(@"预处理得出每一个事件的播放时间完毕");
    
}




//封装一个方法:传入一个范围，返回一个数组
-(NSMutableArray<ChunkEvent *> *)GetEventArrayWithTime:(float)startTime andendTime:(float)endTime andIndexArray:(NSMutableArray *)chunkIndex
{
    
    //用一个临时的可变数组来保存当前的事件
    NSMutableArray<ChunkEvent *> *mEventArray = [NSMutableArray array];
    
    
    //1-轨道要全部遍历结束
    for (NSUInteger i = 0; i < _chunkHead.chunkNum; i++)
    {
        
        //根据传入的lowTime设置不同的终点
        NSUInteger endIndex;
        
        endIndex = self.mtrkArray[i].chunkEventArray.count;
        
        //2-每一个轨道的事件不需要全部遍历
        for (NSUInteger j = [chunkIndex[i] integerValue]; j < endIndex; j++)
        {
            ChunkEvent *chunkEvent = self.mtrkArray[i].chunkEventArray[j];
            
            
            
            if (chunkEvent.eventPlayTime >= startTime && chunkEvent.eventPlayTime < endTime)
            {
                 [mEventArray addObject:chunkEvent];
                
                chunkIndex[i] = @(j);
            }
            else if(chunkEvent.eventPlayTime >= endTime)
            {
                chunkIndex[i] = @(j);
                
                break;
            }
            
        }
    }
    
    return mEventArray;
}

//预加载2:事先处理一下MIDI中的所有8，9，和a事件(键盘按键事件)
-(void)DealWithPressKeyEvent2
{
    //遍历所有的轨道
    for (NSUInteger i = 0; i < _chunkHead.chunkNum; i++)
    {
        //2-每一个轨道的事件需要全部遍历
        for (NSUInteger j = 0; j < self.mtrkArray[i].chunkEventArray.count; j++)
        {
             ChunkEvent *chunkEvent = self.mtrkArray[i].chunkEventArray[j];
            
            NSString *firstStatus = [chunkEvent.eventStatus substringToIndex:1];
            
            if ([firstStatus isEqualToString:@"8"])
            {
                
                //遍历pressKeyArray数组
                for (ChunkEvent *event in self.pressKeyArray)
                {
                    //当音符相同时
                    if ([chunkEvent.midiCode isEqualToString:event.midiCode])
                    {
                        
                        //1-得出按下钢琴到释放的持续时间
                        float durationTime = chunkEvent.eventPlayTime - event.eventPlayTime;
                        
                        //2-更新9事件的持续时间
                        event.eventDuration = durationTime;
                        
                        
                        //3-将9事件从数组中移除
                        [self.pressKeyArray removeObject:event];
                        
                        //4-跳出当期的循环
                        break;
                    }
                }
                
            }
            else if ([firstStatus isEqualToString:@"9"])
            {
                
                //1-将当前的按下键盘事件(9事件)存放到数组中
                [self.pressKeyArray addObject:chunkEvent];
                
            }
            else if ([firstStatus isEqualToString:@"a"])
            {
                NSLog(@"%@触摸键盘以后%@,速度是%@,所在位置是%ld,当前事件的开始播放时间是%f",chunkEvent.eventStatus,chunkEvent.midiCode,chunkEvent.midiSpeed,chunkEvent.location,chunkEvent.eventPlayTime);
            }
            
        }
    }
    
    NSLog(@"预处理键盘事件完毕");    
}


-(void)DealWithPressKeyEvent
{
    
    float startTime = 0.000;
    
    //记录最小的持续时间
    __block float minDurationTime = 100.000;
    
    while (startTime < self.midiAllTime)
    {
        
        //1-传入范围得出数组
        NSMutableArray<ChunkEvent *> *mEventArray;
        
        
        //生成事件数组
        //传入一个事件范围,返回一个事件数组
        mEventArray = [self GetEventArrayWithTime:startTime andendTime:startTime+0.001 andIndexArray:self.chunkIndexArray];
        
        if (mEventArray.count >= 1)
        {
            
            //遍历数组
            [mEventArray enumerateObjectsUsingBlock:^(ChunkEvent * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
               
                NSString *firstStatus = [obj.eventStatus substringToIndex:1];
                
                if ([firstStatus isEqualToString:@"8"])
                {
                    
                    //遍历pressKeyArray数组
                    for (ChunkEvent *event in self.pressKeyArray)
                    {
                        //当音符相同时
                        if ([obj.midiCode isEqualToString:event.midiCode])
                        {
                            
                            //1-得出按下钢琴到释放的持续时间
                            float durationTime = obj.eventPlayTime - event.eventPlayTime;
                            
                            //2-更新9事件的持续时间
                            event.eventDuration = durationTime;
                            
                            //3-记录最小的持续时间
                            
                            if (durationTime <= minDurationTime)
                            {
                                minDurationTime = durationTime;
                            }
                            
                            
                            //4-将9事件从数组中移除
                            [self.pressKeyArray removeObject:event];
                            
                            //5-跳出当期的循环
                            break;
                        }
                    }
                    
                }
                else if ([firstStatus isEqualToString:@"9"])
                {
                    
                    //1-将当前的按下键盘事件(9事件)存放到数组中
                    [self.pressKeyArray addObject:obj];
                    
                }
                else if ([firstStatus isEqualToString:@"a"])
                {
                    NSLog(@"%@触摸键盘以后%@,速度是%@,所在位置是%ld,当前事件的开始播放时间是%f",obj.eventStatus,obj.midiCode,obj.midiSpeed,obj.location,obj.eventPlayTime);
                }
                
            }];
            
        }
        startTime += 0.001;
    }
    
    
    //预处理事件之后chunkIndexArray重置为空
    _chunkIndexArray = nil;
    
    NSLog(@"预处理键盘事件完毕");
    
    //NSLog(@"最小的持续时间是%f",minDurationTime);
}

//仅仅是放
//封装一个方法:播放数组事件
-(void)PlaySoundWithArray:(NSMutableArray<ChunkEvent *> *)eventArray
{
    [eventArray enumerateObjectsUsingBlock:^(ChunkEvent * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        //播放音乐的核心代码
        //播放音乐(一个事件一个事件地播放音乐)
        //不播放FF和F0开头事件的音乐
        if (obj.eventStatus.length <= 2)
        {
            
            NSString *firstStatus = [obj.eventStatus substringToIndex:1];
            
            //得出9事件的持续时间
            if ([firstStatus isEqualToString:@"9"])
            {
                NSLog(@"%f秒生成了按键%@,持续时长为%f",obj.eventPlayTime,obj.midiCode,obj.eventDuration);
                //0-判断钢琴事件数组中是否有事件数据
                if (self.tempEventArray.count > 0)
                {
                    //1-取出数组中的第一个数据事件的播放时间
                    float playTime = self.tempEventArray[0].eventPlayTime;
                    
                    //2-判断取出的播放时间与当前的事件时间是否一致
                    if (playTime == obj.eventPlayTime)
                    {
                        //将当前的事件放入数组中
                        //在原有的基础上添加数据
                        [self.tempEventArray addObject:obj];
                    }
                    else
                    {
                        //清空数组后将事件放入数组中
                        [self.tempEventArray removeAllObjects];
                        
                        //新的数据加入到了数组中
                        [self.tempEventArray addObject:obj];
                    }
                    
                }
                else
                {
                    //添加第一个数据进去
                    //新的数据加入到了数组中
                    [self.tempEventArray addObject:obj];
                }
                
            }
            
            
            
#warning 接受到外界的播放信息之后再播放
            //[self PlaySoundWithChunkEvent:obj];
            
            
        }
    }];
}


//封装播放音乐的方法(传入一个事件)
-(void)PlaySoundWithChunkEvent:(ChunkEvent *)chunkEvent
{
    
    //1-事件数组的起始位置
    NSUInteger location;
    
    //2-事件数组的长度
    NSUInteger length;
    
    //判断是不是缺失事件
    if (chunkEvent.isUnFormal)
    {
        location = chunkEvent.location + chunkEvent.deltaTimeLength;
        
        length = chunkEvent.eventLength - chunkEvent.deltaTimeLength + 1;
    }
    else
    {
        location = chunkEvent.location + chunkEvent.deltaTimeLength + 1;
        
        length = chunkEvent.eventLength - chunkEvent.deltaTimeLength;
    }
    
    
    [self sendMIDIMsgWithStatus:chunkEvent.eventStatus
               andEventLocation:location
                         Length:length];
    
}

//播放方法
//1-参数1:事件状态码(NSString)
//2-参数2:当前事件的事件码在NSData中的位置
- (void) sendMIDIMsgWithStatus:(NSString *)dataStr andEventLocation:(NSUInteger)location  Length:(NSUInteger)size
{
    
    Byte statusData = strtoul([dataStr UTF8String],0,16);
    
    [self.midiData enumerateByteRangesUsingBlock:^(const void * _Nonnull bytes, NSRange byteRange, BOOL * _Nonnull stop) {
        
        
        if (size == 3)
        {
            [_sampler MIDIShortMsg:statusData withData1:((uint8_t*)bytes)[location] withData2:((uint8_t*)bytes)[location + 1]];
        }
        else if (size == 2)
        {
            [_sampler MIDIShortMsg:statusData withData1:((uint8_t*)bytes)[location] withData2:0];
        }
        
    }];
}





@end
