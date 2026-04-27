#pragma once
#include <vector>
#include <string>

enum class GameEvent { Installed, Uninstalled, Started, Stopped, Saved, Loaded, RatingChanged };

struct GameEventData {
	GameEvent   type;
	std::string gameName;
	std::string details;
};

class IGameEventObserver {
public:
	virtual void onGameEvent(const GameEventData& data) = 0;
	virtual ~IGameEventObserver() = default;
};

class GameEventEmitter {
	std::vector<IGameEventObserver*> observers_;
protected:
	void emit(GameEvent type, const std::string& gameName, const std::string& details = "") {
		GameEventData d{ type, gameName, details };
		for (auto* o : observers_) o->onGameEvent(d);
	}
public:
	void subscribe(IGameEventObserver* o) { observers_.push_back(o); }
	void unsubscribe(IGameEventObserver* o) {
		observers_.erase(std::remove(observers_.begin(), observers_.end(), o), observers_.end());
	}
};