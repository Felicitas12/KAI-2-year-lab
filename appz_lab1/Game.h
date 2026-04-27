#pragma once
#include <vector>

#include "GameEvents.h"
#include "Platform.h"
#include "UserAccount.h"
#include "PlatformInstances.h"

enum class GameGenre { Simulator, Shooter, OnlineAdventure, Browser, Strategy };

inline std::string genreName(GameGenre g) {
    switch (g) {
    case GameGenre::Simulator:       return "Simulator";
    case GameGenre::Shooter:         return "Shooter";
    case GameGenre::OnlineAdventure: return "Online Adventure";
    case GameGenre::Browser:         return "Browser";
    case GameGenre::Strategy:        return "Strategy"; // Додано для демонстрації розширення
    }
    return "Unknown";
}


class Game : public GameEventEmitter {
public:
    Game(std::string name, GameGenre genre, GameRequirements req) : name_(std::move(name)), genre_(genre), req_(req) {}
    virtual ~Game() = default;

    const std::string& getName() const { return name_; }
    GameGenre getGenre() const { return genre_; }
    const GameRequirements& getRequirements() const { return req_; }
    bool isRunning() const { return running_; }

    float getRating() const { return ratingCount_ > 0 ? ratingSum_ / ratingCount_ : 0.f; }
    int getRatingCount() const { return ratingCount_; }
    void addRating(int stars) {
        if (stars >= 1 && stars <= 5) {
            ratingSum_ += static_cast<float>(stars);
            ++ratingCount_;
        }
    }

    bool hasSaves() const { return !saves_.empty(); }
    bool saveState(const std::string& slot) {
        if (!running_) return false;
        saves_.push_back(slot);
        return true;
    }
    bool loadState(const std::string& slot) {
        if (!running_) return false;
        return std::find(saves_.begin(), saves_.end(), slot) != saves_.end();
    }

    virtual bool isInstalled() const = 0;
    virtual bool install(Platform& p) = 0;
    virtual bool start(Platform& p, UserAccount& a) = 0;
    virtual bool stop() = 0;

protected:
    std::string name_;
    GameGenre genre_;
    GameRequirements req_;
    bool running_ = false;
    float ratingSum_ = 0.f;
    int ratingCount_ = 0;
    std::vector<std::string> saves_;
};

class InstallableGame : public Game {
public:
    InstallableGame(std::string name, GameGenre genre, GameRequirements req) : Game(std::move(name), genre, req) {}
    bool isInstalled() const override { return installed_; }
    bool install(Platform& p) override {
        if (installed_ || !req_.hasSpace(p.getHardware())) return false;
        installed_ = true; p.consumeHDD(req_.hddSizeGB);
        return true;
    }
    bool start(Platform& p, UserAccount& acc) override {
        if (!installed_ || running_ || !acc.isLoggedIn() || !req_.metBy(p.getHardware()) || !platformCheck(p)) return false;
        running_ = true; return true;
    }
    bool stop() override {
        if (!running_) return false;
        running_ = false; return true;
    }
protected:
    virtual bool platformCheck(const Platform& p) const {
        if (p.getType() == PlatformType::Console) {
            const auto* c = dynamic_cast<const ConsolePlatform*>(&p);
            return c && c->hasControllers();
        }
        return true;
    }
    bool installed_ = false;
};