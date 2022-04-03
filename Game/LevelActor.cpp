#include "LevelActor.h"

LevelActor::~LevelActor() {}

LevelActor::LevelPtr LevelActor::level() { return _level;  }
void LevelActor::setLevel(LevelPtr level) { _level = level;  }
