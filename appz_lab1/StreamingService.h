#pragma once
#include <string>
#include "PlatformInstances.h"

class StreamingService {
public:
    std::string startStream(Platform& p) {
        auto* s = dynamic_cast<IStreamable*>(&p);
        if (!s) return "÷€ платформа (Desktop) не п≥дтримуЇ трансл€ц≥ю.";
        s->startStreaming(); return "";
    }
};