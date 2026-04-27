#pragma once

class INetworkRequired {
public:
    virtual bool isNetworkAvailable() const = 0;
    virtual ~INetworkRequired() = default;
};


