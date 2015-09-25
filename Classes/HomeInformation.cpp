//
//  HomeInformation.cpp
//  Card13
//
//  Created by Hiroki Ariji on 2015/09/26.
//
//

#include "HomeInformation.h"


USING_NS_CC;
USING_NS_CC_EXT;


Scene* HomeInfoController::createScene()
{
    auto scene = Scene::create();
    auto layer = HomeInfoController::create();
    
    scene->addChild(layer);
    
    return scene;
}

Scene* HomeInfoController::scene()
{
    auto scene = Scene::create();
    auto layer = HomeInfoController::create();
    
    scene->addChild(layer);
    
    return scene;
}

//HelloWorldクラスのレイヤーの初期化処理を行う
bool HomeInfoController::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //シングルタップイベント取得
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(_swallowsTouches);
    
    //イベント関数の割り当て
    listener->onTouchBegan = CC_CALLBACK_2(HomeInfoController::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HomeInfoController::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HomeInfoController::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HomeInfoController::onTouchCancelled, this);
    
    //イベントを追加する
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    initGame();
    
    return true;
}

void HomeInfoController::getMessage(char* result)
{
    const char *post_command;
    
    post_command = "http://127.0.0.1:3000/send_message?type=2";
    
    Get(post_command);
    
    return;
}

void HomeInfoController::initGame()
{
    char message[100];
    getMessage(message);
    Size winSize = Director::getInstance()->getVisibleSize();
    
    auto _bg2 = LayerColor::create(Color4B(0,255,0,128), winSize.width, winSize.height);
    this->addChild(_bg2);
    
    //画面サイズを取得
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //CCSpriteクラスで画像を設定します。
    _sprite1 = Sprite::create("button.png");
    _sprite1->setScale(2.0f);
    _sprite1->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    addChild(_sprite1);
    
    Label *label = Label::createWithSystemFont("TEST", "Marker Felt.ttf", 30);
    
    label->setScale(2.0f);
    label->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(label);
    
    auto move = MoveTo::create(MOVING_TIME, Vec2(visibleSize.width/3, visibleSize.height/3));
    
    //アニメーションの実行
    label->runAction(move);
    
    //update関数の呼び出しを開始
    scheduleUpdate();
    
}


void HomeInfoController::startWebView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto webView = cocos2d::experimental::ui::WebView::create();
    webView->setAnchorPoint(Point(0.5f, 0.5f));
    webView->setContentSize(Size(visibleSize.width * 0.9f, visibleSize.height * 0.8f));
    webView->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2)));
    webView->loadURL("https://www.daiwahouse.co.jp/smp/business/kenchiku/script/regist_kenchiku.asp?ken_toi_kbn=3");
    this->addChild(webView, 1);
    
}

bool HomeInfoController::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void HomeInfoController::onTouchMoved(Touch *touch, Event *unused_event)
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(3.0f,  HelloWorld::scene()));
}


void HomeInfoController::onTouchEnded(Touch *touch, Event *unused_event)
{
    startWebView();
    
    return;
}

void HomeInfoController::onTouchCancelled(Touch *touch, Event *unused_event)
{
    //タップ終了と同じ処理を行う
    onTouchEnded(touch, unused_event);
}

void HomeInfoController::update(float dt)
{
    
}
