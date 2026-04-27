#pragma once

class IStreamable {
public:
    virtual void startStreaming() = 0;
    virtual void stopStreaming() = 0;
    virtual bool isStreaming() const = 0;
    virtual ~IStreamable() = default;
};
