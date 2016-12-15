
#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

//关于MIDI轨道的解析
#import "ChunkHeader.h"
#import "MTRKChunk.h"
#import "FF5103ChunkEvent.h"



@interface RootViewController()
//轨道头
@property (nonatomic,strong)ChunkHeader *chunkHead;

//一个大的MIDI文件分成多个轨道块，用数组保存这些轨道块
@property (nonatomic,strong)NSArray<MTRKChunk *> *mtrkArray;


//一个MIDI文件在内存中只存在一个NSData对象
@property (nonatomic,strong)NSData *midiData;

//定义一个全局属性记录一下当前MIDI的总时间
@property (nonatomic,assign)float midiAllTime;

//定义一个数组记录一下MIDI文件中所有5103事件的数组
@property (nonatomic,strong)NSArray<FF5103ChunkEvent *> *ff5103Array;



@end



@implementation RootViewController


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
    
    [super loadView];
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    
    // Initialize the GLView attributes
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Initialize the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: (__bridge NSString *)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Set EAGLView as view of RootViewController
    self.view = eaglView;
    
    
    
    
    
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)self.view);
    
    //set the GLView as OpenGLView of the Director
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    
    
    
    //run the cocos2d-x game scene
    app->run();
    
}




// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    //轨道头初始化
    _chunkHead = [ChunkHeader sharedChunkHeaderFrom:kFilePath];
    
    
    //处理playMusic播放类
    _playMusic = [PlayMusic PlayMusicWithChunkHead:_chunkHead
                                    andff5103Array:self.ff5103Array andMTRKArray:self.mtrkArray andMidiAllTime:self.midiAllTime andMidiData:self.midiData];
    //预处理事件1
    [_playMusic CaculateTheEventTime];
    
    //预处理事件2
    [_playMusic DealWithPressKeyEvent];
    
    //[_playMusic PlayMIDIMultiTempMusic];
    
}

//1-当前的midi的总时间
-(float)midiAllTime
{
    if (_midiAllTime == 0)
    {
        //记录一下每个4分音符的时长(不断变化的)
        NSUInteger quartTime = 0;
        
        //遍历MIDI事件中的轨道
        for (NSUInteger i = 0; i < _chunkHead.chunkNum; i++)
        {
            
            //即时计算当前轨道的时间
            float theTime = 0.00000000;
            
            //遍历轨道中的事件(遍历每一个事件)
            //在当前这个轨道中
            for (NSUInteger j = 0; j < self.mtrkArray[i].chunkEventArray.count; j++)
            {
                ChunkEvent *chunkEvent = self.mtrkArray[i].chunkEventArray[j];
                
                
                
                //判断即时的总delta-time是否大于ff5103数组中的最大值
                if (self.ff5103Array.count > 0)
                {
                    //超过了总的5103
                    if (chunkEvent.eventAllDeltaTime > self.ff5103Array[self.ff5103Array.count - 1].eventAllDeltaTime)
                    {
                        quartTime = self.ff5103Array[self.ff5103Array.count - 1].theQuartTime;
                    }
                    else
                    {
                        theTime =  self.ff5103Array[self.ff5103Array.count - 1].eventPlayTime;
                        
                        continue;
                    }
                }
                else
                {
                    quartTime = 500000;
                }
                
                
                //超过的时长
                float theSurDelataTime = 0.00000000;
                
                theSurDelataTime = (float)((float)(chunkEvent.eventAllDeltaTime - self.ff5103Array[self.ff5103Array.count - 1].eventAllDeltaTime)/(float)_chunkHead.tickNum) * quartTime *0.00100 * 0.00100;
                
                theTime = theSurDelataTime + self.ff5103Array[self.ff5103Array.count - 1].eventPlayTime;
                
            }
            
            if (_chunkHead.chunkType == 0)
            {
                _midiAllTime = theTime;
            }
            else if(_chunkHead.chunkType == 1)
            {
                if (_midiAllTime <= theTime)
                {
                    _midiAllTime = theTime;
                }
            }
            else
            {
                _midiAllTime += theTime;
            }
            
        }
        
        NSLog(@"当前MIDI文件的总时间是:%f",_midiAllTime);
        
    }
    
    return _midiAllTime;
}

//2-5103数组
#warning 默认5103的分布不会出现凹形状的
-(NSArray<FF5103ChunkEvent *> *)ff5103Array
{
    if (_ff5103Array == nil)
    {
        
        NSMutableArray<FF5103ChunkEvent *> *ff51mArray = [NSMutableArray array];
        
        //记录一下每个4分音符的时长(不断变化的)
        NSUInteger quartTime = 0;
        
        
        //遍历MIDI事件中的轨道
        for (NSUInteger i = 0; i < _chunkHead.chunkNum; i++)
        {
            
            //即时统计当前轨道中的delta-time(总的delta-time)
            NSUInteger allChunkDeltaTime = 0;
            
            //即时统计总时间
            float theTime = 0.000000;
            
            
            
            //遍历轨道中的事件(遍历每一个事件)
            //在当前这个轨道中
            for (NSUInteger j = 0; j < self.mtrkArray[i].chunkEventArray.count; j++)
            {
                ChunkEvent *chunkEvent = self.mtrkArray[i].chunkEventArray[j];
                
                //即时的总delta-time
                allChunkDeltaTime += chunkEvent.eventDeltaTime;
                
                
                //更新属性值:即时的总delta-time
                chunkEvent.eventAllDeltaTime = allChunkDeltaTime;
                
                
                //即时计算总时间
                //当前事件的时长
                float theChunkEventTime = 0.00000000;
                
                theChunkEventTime = (float)((float)chunkEvent.eventDeltaTime/(float)_chunkHead.tickNum) * quartTime *0.00100 * 0.00100;
                
                //即时的总时长
                theTime += theChunkEventTime;
                
                
                
                //出现5103事件时，4分音符时长发生变化
                if ([chunkEvent isKindOfClass:[FF5103ChunkEvent class]])
                {
                    //更新属性值:即时的总delta-time
                    //chunkEvent.eventAllDeltaTime = allChunkDeltaTime;
                    
                    //更新即时时间属性值
                    chunkEvent.eventPlayTime = theTime;
                    
                    
                    quartTime = ((FF5103ChunkEvent *)chunkEvent).theQuartTime;
                    
                    
                    //NSLog(@"delta-time:%ld之后的4分音符的时长是%ld,%ld之前的总运行时间是%f",allChunkDeltaTime,quartTime,allChunkDeltaTime,theTime);
                    
                    [ff51mArray addObject:(FF5103ChunkEvent *)chunkEvent];
                }
            }
        }
        
        _ff5103Array = ff51mArray.copy;
        
    }
    
    return _ff5103Array;
}

//3-源MIDI数据(转换成NSData之后)
-(NSData *)midiData
{
    if (_midiData == nil)
    {
        _midiData = [NSData dataWithContentsOfFile:@kFilePath];
    }
    
    return _midiData;
}

//4-一个大的MIDI文件分成多个轨道块，用数组保存这些轨道块
-(NSArray<MTRKChunk *> *)mtrkArray
{
    if (_mtrkArray == nil)
    {
        
        if (self.midiData.length <= 23)
        {
            NSLog(@"当前的MIDI文件不完全");
            
            return nil;
        }
        
        //当前轨道块的长度(NSString）
        NSMutableString *mtrkLength = [NSMutableString string];
        //当前轨道的长度(NSUInteger)
        __block NSUInteger length = 0;
        
        //每一个轨道块的长度值起始
        __block NSUInteger lengthStart = 18;
        
        //可变数组
        NSMutableArray<MTRKChunk *> *mMtrkArray = [NSMutableArray array];
        
        [self.midiData enumerateByteRangesUsingBlock:^(const void *bytes,
                                                       NSRange byteRange,
                                                       BOOL *stop) {
            
            //判断MIDI文件
            for (NSUInteger i = 0; i < byteRange.length; ++i)
            {
                
                //转换成NSString来判断值(也可以不转)
                NSString *tempString = [NSString stringWithFormat:@"%02x",((uint8_t*)bytes)[i]];
                
                //轨道块的长度提取
                if (i>=lengthStart + length && i<=lengthStart + 3 + length)
                {
                    [mtrkLength appendString:tempString];
                }
                
                if (i == lengthStart + 4 + length)
                {
                    length = strtoul([mtrkLength UTF8String],0,16);
                    
                    if (length != 0)
                    {
                        lengthStart = i + 4;
                    }
                    
                    //清空轨道快的长度
                    [mtrkLength deleteCharactersInRange:NSMakeRange(0, mtrkLength.length)];
                    
                    //初始化轨道块
                    MTRKChunk *mtrkChunk = [[MTRKChunk alloc]initWithMIDIData:self.midiData andChunkLength:length andLocation:i];
                    
                    
                    
                    
                    [mMtrkArray addObject:mtrkChunk];
                }
                
            }
        }];
        
        _mtrkArray = mMtrkArray.copy;
    }
    
    return _mtrkArray;
}



- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
}




// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
#ifdef __IPHONE_6_0
- (NSUInteger) supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAllButUpsideDown;
}
#endif




- (BOOL) shouldAutorotate
{
    return YES;
}

//转屏幕之后的回调函数
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    NSLog(@"didRotateFromInterfaceOrientation");
    
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}


@end
