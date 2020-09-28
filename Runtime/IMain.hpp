#pragma once

#include "Runtime/RetroTypes.hpp"

#include "amuse/amuse.hpp"
#include "boo2/audiodev/IAudioVoiceEngine.hpp"
#include "hecl/Runtime.hpp"

namespace hecl {
class CVarManager;
} // namespace hecl

namespace urde {
class CStopwatch;
enum class EGameplayResult { None, Win, Lose, Playing };

enum class EFlowState {
  None,
  WinBad,
  WinGood,
  WinBest,
  LoseGame,
  Default,
  StateSetter,
};

class IMain {
public:
  virtual ~IMain() = default;
  virtual void Init(const hecl::Runtime::FileStoreManager& storeMgr, hecl::CVarManager* cvarMgr,
                    boo2::IAudioVoiceEngine* voiceEngine, amuse::IBackendVoiceAllocator& backend) = 0;
  virtual void Draw() = 0;
  virtual bool Proc() = 0;
  virtual void Shutdown() = 0;
  virtual EFlowState GetFlowState() const = 0;
  virtual void SetFlowState(EFlowState) = 0;
  virtual size_t GetExpectedIdSize() const = 0;
  virtual void WarmupShaders() = 0;
};
} // namespace urde
