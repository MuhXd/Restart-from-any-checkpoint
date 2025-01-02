#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "popup.hpp"
class $modify(hihihihihihi,PlayLayer){
	struct Fields {
		CheckpointObject* overrideCheckPoint;
	};
	void loadFromCheckpoint(CheckpointObject* val) {
		if (this->m_fields->overrideCheckPoint) {
			val = this->m_fields->overrideCheckPoint;
			this->m_fields->overrideCheckPoint = nullptr;
		};
		return PlayLayer::loadFromCheckpoint(val);
	};
};
class $modify(endLayer,EndLevelLayer){
	struct Fields {
		bool m_xddddd;
	};
	void onRestartCheckpointCustom(CCObject* x, CheckpointObject* p) {
		this->m_fields->m_xddddd = true;
		if ((this->m_playLayer)) { 
			reinterpret_cast<hihihihihihi*>(this->m_playLayer)->m_fields->overrideCheckPoint = p;
			EndLevelLayer::onRestartCheckpoint(x);
		}
	};
	void onRestartCheckpoint(CCObject* x) {
		if (this->m_fields->m_xddddd) {
			this->m_fields->m_xddddd = false;
			return EndLevelLayer::onRestartCheckpoint(x);
		}
		PlayLayer*pl = this->m_playLayer;
		if (!pl) {
			return EndLevelLayer::onRestartCheckpoint(x);
		};
		

		int min = 1;
		int max = pl->m_checkpointArray->count();
		auto Checkpoint = CheckPointPopup::create(min,max,[=](int selected){
			if (max == selected) {
				this->m_fields->m_xddddd = true;
				return EndLevelLayer::onRestartCheckpoint(x);
			};
			auto checkpoint = pl->m_checkpointArray->objectAtIndex(selected-1);
			if (checkpoint) {
				if (Mod::get()->getSettingValue<bool>("rm-root")) {
					int maxx = max-1;
					while (maxx > (selected-1)) {
						maxx-=1;
						CheckpointObject* c = (CheckpointObject*) pl->m_checkpointArray->lastObject();
						pl->m_checkpointArray->removeLastObject(false);
						if (c) {
							c->m_physicalCheckpointObject->destroyObject();
						};
					};
				}
				pl->m_checkpointArray->removeObject(checkpoint,false);
				pl->m_checkpointArray->addObject(checkpoint);
				CheckpointObject* p = typeinfo_cast<CheckpointObject*>(checkpoint);
				endLayer::onRestartCheckpointCustom(x,p);
			}
		});
        Checkpoint->show();
	}
};