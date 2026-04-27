#pragma once

class NetworkService {
    bool connected_ = false;
public:
    void connect() { connected_ = true; }
    void disconnect() { connected_ = false; }
    bool isConnected() const { return connected_; }
};