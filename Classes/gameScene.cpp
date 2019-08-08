/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "gameScene.h"
#include "SimpleAudioEngine.h"

#include <fstream>
#include <locale>
#include <codecvt>


USING_NS_CC;
using namespace std;

Scene* CGame::createScene()
{
    return CGame::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in CGameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool CGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(CGame::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	// add next page menu
	auto nextItem = MenuItemImage::create(
		"ButtonNext.png",
		"ButtonNext.png",
		CC_CALLBACK_1(CGame::menuCloseCallback, this));
	nextItem->setPosition(Vec2(origin.x + visibleSize.width - nextItem->getContentSize().width / 2, origin.y + nextItem->getContentSize().height*1.5));
	auto nextMenu = Menu::create(nextItem, NULL);
	nextMenu->setPosition(Vec2::ZERO);
	this->addChild(nextMenu, 1);

    /////////////////////////////
    // 3. add your codes below..

    // add "CGame" splash screen"
    auto background = Sprite::create("background.png");
    if (background == nullptr)
    {
        problemLoading("'background.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
		background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(background, 0);
    }

	/////////////////////////////
	// 4. add component 4X4
	vector< vector<Sprite*> > arCubes(4, vector<Sprite*>(4));
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			arCubes[i][j] = Sprite::create("smallBlock.png");
			arCubes[i][j]->setPosition(Vec2(visibleSize.width / 2 + origin.x + (i-2)*85, visibleSize.height / 2 + origin.y+(j-2)*75+30));
			//arCubes[i][j]->setPosition(30 + 75 * i, 30 + 65 * j);
			this->addChild(arCubes[i][j], 0);
		}
	}
	// 5th column -- menu
	//vector<Sprite*> ansCubes(4);
	for (i = 0; i < 4; i++) {
		auto ansItem = MenuItemImage::create("ansBlock.png", "ansBlockSel.png", CC_CALLBACK_1(CGame::menuAnswerCallback, this, i));
		
		ansItem->setPosition(Vec2(visibleSize.width / 2 + origin.x + 2 * 85, visibleSize.height / 2 + origin.y + (1 - i) * 75 + 30));
		auto ansMenu = Menu::create(ansItem, NULL);
		ansMenu->setPosition(Vec2::ZERO);
		this->addChild(ansMenu);
	}

	// add Chinese word.
	// read all data
	vector<string> v;
	string fullName = "4word.txt";
	string fullPath = CCFileUtils::getInstance()->fullPathForFilename(fullName);

	v = myUtils.getTextData(fullPath);
	int hi, vi;
	int vPos = -1, hPos = -1;
	//int vPos = 0, hPos = 0;
	while (true) {
		// random pickup 2 line
		hi = cocos2d::RandomHelper::random_int(1, int(v.size()))-1;
		vi = cocos2d::RandomHelper::random_int(1, int(v.size()))-1;

		for (i = 1; i <= 4; i++) {
			string hWord = myUtils.substr(v[hi], i, i);
			for (j = 1; j <= 4; j++) {
				string vWord = myUtils.substr(v[vi], j, j);
				if (hWord.compare(vWord) == 0) {
					vPos = j;
					hPos = i;
					break;
				}
			}
		}
		if (vPos != -1 && hPos != -1) break;
	}
	//generate words.
	for (i = 0; i < 4; i++) {
		int w = i + 1;
		// create label
		// 橫
		if (w != hPos) {
			string hWord = myUtils.substr(v[hi], w, w);
			auto hlabelChi = Label::createWithSystemFont(hWord, "標楷體", 24);
			hlabelChi->enableBold();
			hlabelChi->setPosition(Vec2(visibleSize.width / 2 + origin.x + (i - 2) * 85, visibleSize.height / 2 + origin.y + (2-vPos) * 75 + 30));
			this->addChild(hlabelChi);
		}
		// 直
		if(w != vPos){
			string vWord = myUtils.substr(v[vi], w, w);
			auto vlabelChi = Label::createWithSystemFont(vWord, "標楷體", 24);
			vlabelChi->enableBold();
			vlabelChi->setPosition(Vec2(visibleSize.width / 2 + origin.x + (hPos - 3) * 85, visibleSize.height / 2 + origin.y + (1 - i) * 75 + 30));
			this->addChild(vlabelChi);
		}
	}
	//generate answer.
	string ansWords[4];
	ansWords[0] = myUtils.substr(v[hi], hPos, hPos);
	for (i = 1; i < 4; i++) {
		int ai = cocos2d::RandomHelper::random_int(1, int(v.size()))-1;
		int ri = cocos2d::RandomHelper::random_int(1, 4);
		string rWord = myUtils.substr(v[ai], ri, ri);
		ansWords[i] = rWord;
	}
	//change words
	m_realAns = 0;
	for (i = 0; i < 100; i++) {
		int xi = cocos2d::RandomHelper::random_int(1, 4) - 1;

		string tmpStr = ansWords[xi];
		ansWords[xi] = ansWords[m_realAns];
		ansWords[m_realAns] = tmpStr;
		m_realAns = xi;
	}

	// draw answer.
	for (i = 0; i < 4; i++) {
		auto alabelChi = Label::createWithSystemFont(ansWords[i], "標楷體", 24);
		alabelChi->enableBold();
		alabelChi->setPosition(Vec2(visibleSize.width / 2 + origin.x + 2 * 85, visibleSize.height / 2 + origin.y + (1 - i) * 75 + 30));
		this->addChild(alabelChi);
	}
	
	/*
	//auto labelChi = Label::createWithSystemFont(UTF8FromUTF16(L"靠夭"), "標楷體", 24);
	string word = v[hi] + " - " + v[vi];
	//string word = myUtils.substr(v[hi], 1, 1);// from 1 to 1, not length
	//auto labelChi = Label::createWithSystemFont(v[hi], "標楷體", 24);
	auto labelChi = Label::createWithSystemFont(word, "標楷體", 24);
	labelChi->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 30));
	this->addChild(labelChi);
	*/
    return true;
}


void CGame::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void CGame::menuAnswerCallback(Ref* pSender, int i)
{

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);

	std::stringstream touchDetails;

	if(m_realAns == i)
		touchDetails << myUtils.UTF8FromUTF16(L"猜對了");
	else
		touchDetails << myUtils.UTF8FromUTF16(L"猜錯了");

	MessageBox(touchDetails.str().c_str(), myUtils.UTF8FromUTF16(L"答案").c_str());
}
