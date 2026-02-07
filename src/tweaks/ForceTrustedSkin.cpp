#include "tweaks.h"

#include <ll/api/memory/Hook.h>

#include <mc/world/actor/player/Player.h>
#include <mc/world/actor/player/SerializedSkinImpl.h>
#include <mc/world/actor/player/SerializedSkinRef.h>
#include <mc/world/actor/player/TrustedSkinFlag.h>

#include <MintVanilla.h>
#include <Config.h>

namespace mintvanilla::forcetrustedskin {

LL_TYPE_INSTANCE_HOOK(
  SerializedSkinImplHook,
  ll::memory::HookPriority::Normal,
  Player,
  &Player::updateSkin,
  void,
  SerializedSkinRef const& skin,
  int clientSubID
) {
  skin.mSkinImpl->mObject.mIsTrustedSkin = TrustedSkinFlag::True;
  return origin(skin, clientSubID);
}

struct ForceTrustedSkin::Impl {
  ll::memory::HookRegistrar<SerializedSkinImplHook> r;
};

void ForceTrustedSkin::call(bool enable) {
  if (enable) {
    if (!impl) impl = std::make_unique<Impl>();
  } else {
    impl.reset();
  }
}

ForceTrustedSkin::ForceTrustedSkin()  = default;
ForceTrustedSkin::~ForceTrustedSkin() = default;

} // namespace mintvanilla::forcetrustedskin

// LL_TYPE_INSTANCE_HOOK(
//   PlayerListPacketHook,
//   ll::memory::HookPriority::Normal,
//   PlayerListPacket,
//   &PlayerListPacket::$write,
//   void,
//   BinaryStream& bs
// ) {
//   this->mEntries->at(0).mSkin->mSkinImpl->mObject.mIsTrustedSkin = TrustedSkinFlag::True;
//   return origin(bs);
// }

// LL_AUTO_TYPE_INSTANCE_HOOK(
//   PlayerSkinPacketHook,
//   ll::memory::HookPriority::Normal,
//   SerializedSkinImpl,
//   &SerializedSkinImpl::$write,
//   void,
//   BinaryStream& bs
// ) {
//   this->mSkin->mSkinImpl->mObject.mIsTrustedSkin = TrustedSkinFlag::True;
//   return origin(bs);
// }
