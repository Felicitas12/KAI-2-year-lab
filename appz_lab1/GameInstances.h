#pragma once
#include <string>

#include "Game.h"
#include "Hardware.h"
#include "INetworkRequired.h"
#include "NetworkService.h"
#include "Platform.h"
#include "UserAccount.h"



class SimulatorGame : public InstallableGame {
public: SimulatorGame(std::string name, GameRequirements req) : InstallableGame(std::move(name), GameGenre::Simulator, req) {}
};

class ShooterGame : public InstallableGame {
public: ShooterGame(std::string name, GameRequirements req) : InstallableGame(std::move(name), GameGenre::Shooter, req) {}
};

class StrategyGame : public InstallableGame {
public: StrategyGame(std::string name, GameRequirements req) : InstallableGame(std::move(name), GameGenre::Strategy, req) {}
};

class OnlineAdventureGame : public InstallableGame, public INetworkRequired {
    NetworkService& network_;
public:
    OnlineAdventureGame(std::string name, GameRequirements req, NetworkService& net) : InstallableGame(std::move(name), GameGenre::OnlineAdventure, req), network_(net) {}
    bool isNetworkAvailable() const override { return network_.isConnected(); }
    bool start(Platform& p, UserAccount& acc) override {
        if (!network_.isConnected()) return false;
        return InstallableGame::start(p, acc);
    }
};

class BrowserGame : public Game {
public:
    BrowserGame(std::string name, GameRequirements req) : Game(std::move(name), GameGenre::Browser, req) {}

    bool isInstalled() const override { return true; }
    bool install(Platform&)  override { return true; }
    bool uninstall(Platform&) override { return false; }

    bool start(Platform& p, UserAccount& acc) override {
        if (running_) return false;
        if (!acc.isLoggedIn()) return false;
        if (!req_.metBy(p.getHardware())) return false;
        running_ = true;
        emit(GameEvent::Started, name_);
        return true;
    }

    bool stop() override {
        if (!running_) return false;
        running_ = false;
        emit(GameEvent::Stopped, name_);
        return true;
    }
};
