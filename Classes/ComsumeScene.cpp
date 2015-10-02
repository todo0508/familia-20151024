//
//  ComsumeScene.cpp
//  Card13
//
//  Created by Hiroki Ariji on 2015/10/02.
//
//

#include "ComsumeScene.hpp"
#include <random>


USING_NS_CC;
USING_NS_CC_EXT;


Scene* ComsumeController::scene()
{
    auto scene = Scene::create();
    auto layer = ComsumeController::create();
    
    scene->addChild(layer);
    
    return scene;
}

//HelloWorldクラスのレイヤーの初期化処理を行う
bool ComsumeController::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //シングルタップイベント取得
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(_swallowsTouches);
    
    //イベント関数の割り当て
    listener->onTouchBegan = CC_CALLBACK_2(ComsumeController::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ComsumeController::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ComsumeController::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ComsumeController::onTouchCancelled, this);
    
    //イベントを追加する
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    initGame();
    
    return true;
}

void ComsumeController::powerSavingMode(char* result)
{
    const char *post_command;
    post_command = "http://127.0.0.1:3000/api/send_message?type=54";
    Post(post_command);
    return;
}

void ComsumeController::initGame()
{
    char message[100];
    Size winSize = Director::getInstance()->getVisibleSize();
    auto _bg2 = LayerColor::create(Color4B(0,128,128,128), winSize.width, winSize.height);
    this->addChild(_bg2);
    /*
     if(getAutoDoorStatus(message)){
     _doorStatus = 1;
     _sprite1 = Sprite::create("normal_door.png");
     _sprite1->setScale(1.0f);
     _sprite1->setPosition(Vec2(winSize.width/2, winSize.height/2));
     }else{
     */
    _doorStatus = 0;
    _sprite1 = Sprite::create("normal_door_close.png");
    _sprite1->setScale(1.0f);
    _sprite1->setPosition(Vec2(winSize.width/2, winSize.height/2));
    //    }
    
    addChild(_sprite1);
    _sprite2 = Sprite::create("house-key.png");
    _sprite2->setScale(1.0f);
    _sprite2->setPosition(Vec2(winSize.width/2-150, winSize.height/2));
    addChild(_sprite2);
    
    Label *label = Label::createWithSystemFont("電力消費量の予測", "Marker Felt.ttf", 30);
    label->setScale(2.0f);
    label->setPosition(Vec2(winSize.width/2, winSize.height*3/4));
    this->addChild(label);
    
    //    platform::NativeBridge::executeNative();
    
    //update関数の呼び出しを開始
    scheduleUpdate();
    
}

void ComsumeController::playEffect()
{
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music/blackout_harp1.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/blackout_harp1.mp3");
    //    CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect("music/blackout_harp1.mp3");
}

void ComsumeController::showSPrite()
{
    char message[100];
    Size winSize = Director::getInstance()->getVisibleSize();
    powerSavingMode(message);
    
    this->removeChild(_sprite1);
    if(_doorStatus == 0){
        _sprite1 = Sprite::create("normal_door.png");
        _sprite1->setScale(1.0f);
        _sprite1->setPosition(Vec2(winSize.width/2, winSize.height/2));
        _doorStatus = 1;
    }else{
        _sprite1 = Sprite::create("normal_door_close.png");
        _sprite1->setScale(1.0f);
        _sprite1->setPosition(Vec2(winSize.width/2, winSize.height/2));
        _doorStatus = 0;
    }
    addChild(_sprite1);
    
    
    // 別スレッドを生成して引数を渡して実行する
    auto t = std::thread([this] (int n) {
        for (int i = 0; i < 100; i++) {
            mtx.lock();
            log("%d", n + i);
            mtx.unlock();
        }
        
        // 処理が一通り終わったのでメインスレッドに戻してメソッドを呼ぶ
        auto scheduler = Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread(CC_CALLBACK_0(ComsumeController::dispatchThreadCallbacks, this));
    }, 1000);
    
    // スレッドの管理を手放す
    // スレッドの処理を待つ場合はt.join()かstd::asyncを使う
    t.detach();
}

void ComsumeController::dispatchThreadCallbacks()
{
    // std::lock_guardはunlockをスコープから抜ける時に自動的にやってくれる
    std::lock_guard<std::mutex> lock(mtx);
//    CCDirector::sharedDirector()->replaceScene(TransitionFadeTR::create(6.0f,  HelloWorld::scene()));
    Director::getInstance()->replaceScene(TransitionMoveInB::create(3.0f,HelloWorld::scene()));
}

void ComsumeController::startWebView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto webView = cocos2d::experimental::ui::WebView::create();
    
    webView->setAnchorPoint(Point(0.5f, 0.5f));
    webView->setContentSize(Size(visibleSize.width * 0.5f, visibleSize.height * 0.75f));
    webView->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2)));
    webView->loadURL("https://www.daiwahouse.co.jp/smp/business/kenchiku/script/regist_kenchiku.asp?ken_toi_kbn=3");
    this->addChild(webView, 1);
}

bool ComsumeController::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void ComsumeController::onTouchMoved(Touch *touch, Event *unused_event)
{
    CCDirector::sharedDirector()->replaceScene(TransitionFade::create(3.0f,  HelloWorld::scene(), ccc3(0, 0, 0)));
}


void ComsumeController::onTouchEnded(Touch *touch, Event *unused_event)
{
    playEffect();
    
    showSPrite();
    
    return;
}

void ComsumeController::onTouchCancelled(Touch *touch, Event *unused_event)
{
    //タップ終了と同じ処理を行う
    onTouchEnded(touch, unused_event);
}

void ComsumeController::update(float dt)
{
    
}