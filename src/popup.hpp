#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/NodeIDs.hpp>
#include <Geode/modify/SetIDPopup.hpp>

using namespace geode::prelude;
using namespace geode::node_ids;

class CheckPointPopup : public SetIDPopupDelegate {
    protected:
       std::function<void(int)> m_callback;
    public:
        virtual void setIDPopupClosed(SetIDPopup* ob, int setnum) {
            if (m_callback) {
                m_callback(setnum);
            }
           delete this;
        }

        static bool init(SetIDPopup* ob, int min, int max) {
            CCLabelBMFont* lab = ob->getChildByType<CCLabelBMFont*>(0);
            CCLayer* Layer = ob->getChildByType<CCLayer>(0);
            CCScale9Sprite* bg = Layer->getChildByType<CCScale9Sprite>(0);
            CCMenu* ccmenu = Layer->getChildByType<CCMenu>(0);
            auto winSize = CCDirector::get()->getWinSize();
            bg->setContentWidth(winSize.width / 1.4); // pad the edge
            if (auto close = getChildBySpriteFrameName(ccmenu, "GJ_closeBtn_001.png")) {
                close->setPositionX( (-bg->getScaledContentWidth() / 2) + close->getScaledContentWidth() / 2 - 3 );
            }
            if (auto xd = getChildBySpriteFrameName(ccmenu, "GJ_resetBtn_001.png")) {
                if(CCMenuItemSpriteExtra* spr = typeinfo_cast<CCMenuItemSpriteExtra*>(xd)) {
                    spr->setSprite(CCSprite::createWithSpriteFrameName("GJ_restartCheckBtn_001.png"));
                }
                xd->setPositionX( (bg->getScaledContentWidth() / 2) - xd->getScaledContentWidth() / 2 );
            }
            return true;
        }
        static SetIDPopup* create(int min, int max, std::function<void(int)> callback) {
            auto ret = SetIDPopup::create(max,min,max,"Select a checkpoint","select",true,max,60,false,false);
            if (ret) {
                auto CheckPointdel = new CheckPointPopup;
                CheckPointdel->m_callback = callback;
                ret->m_delegate = CheckPointdel;
                ret->setID("Checkpoint"_spr);
                CheckPointPopup::init(ret, min, max);
                return ret;
            }

            CC_SAFE_DELETE(ret);
            return nullptr;
        };
};

class $modify(SetIDPopup){
    void onCancel(CCObject* x) {
        SetIDPopup::onCancel(x);
        if (this->m_delegate) {
            if(CheckPointPopup* spr = typeinfo_cast<CheckPointPopup*>(this->m_delegate)) {
                delete spr;
            }
        }
    }
};

