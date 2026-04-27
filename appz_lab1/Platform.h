#pragma once
#include <string>
#include "Hardware.h"

enum class PlatformType { Desktop, Mobile, Console };


class Platform {
protected:
    std::string  name_;
    PlatformType type_;
    HardwareSpec hw_;
public:
    Platform(std::string name, PlatformType type, HardwareSpec hw) : name_(std::move(name)), type_(type), hw_(hw) {}
    virtual ~Platform() = default;

    const std::string& getName() const { return name_; }
    PlatformType getType() const { return type_; }
    const HardwareSpec& getHardware() const { return hw_; }

    void consumeHDD(int gb) { hw_.hddFreeGB -= gb; }
    void releaseHDD(int gb) { hw_.hddFreeGB += gb; }
};
