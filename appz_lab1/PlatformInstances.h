#pragma once

#include "Hardware.h"
#include "Platform.h"
#include "IStreamable.h"

class DesktopPlatform : public Platform {
public:
    explicit DesktopPlatform(HardwareSpec hw) : Platform("Desktop", PlatformType::Desktop, hw) {}
};

class MobilePlatform : public Platform, public IStreamable {
    bool streaming_ = false;
public:
    explicit MobilePlatform(HardwareSpec hw) : Platform("Mobile", PlatformType::Mobile, hw) {}
    void startStreaming() override { streaming_ = true; }
    void stopStreaming()  override { streaming_ = false; }
    bool isStreaming()    const override { return streaming_; }
};

class ConsolePlatform : public Platform, public IStreamable {
    bool streaming_ = false;
    int  controllers_ = 0;
public:
    ConsolePlatform(HardwareSpec hw, int controllers = 0) : Platform("Console", PlatformType::Console, hw), controllers_(controllers) {}
    void connectController() { ++controllers_; }
    void disconnectController() { if (controllers_ > 0) --controllers_; }
    bool hasControllers()  const { return controllers_ > 0; }

    void startStreaming() override { streaming_ = true; }
    void stopStreaming()  override { streaming_ = false; }
    bool isStreaming()    const override { return streaming_; }
};