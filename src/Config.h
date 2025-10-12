#pragma once

#include "tweaks/tweaks.h"

#include <ll/api/reflection/Dispatcher.h>

using ll::reflection::Dispatcher;

namespace mintvanilla {

struct Config {
  int version = 1;
  struct {
    Dispatcher<bool, allowclientresourcepack::AllowClientResourcePack> allowClientResourcePack = true;
    Dispatcher<bool, fakeseed::FakeSeed> fakeSeed = true;
    Dispatcher<bool, forcetrustedskin::ForceTrustedSkin> forceTrustedSkin = true;
    Dispatcher<bool, hideoperator::HideOperator> hideOperator = true;
  } tweaks{};
  struct {
    bool restartCommand = true;
    bool tpsCommand = true;
  } commands{};
  struct {
    int seed = -777;
    bool tpsCommandAny = true;
    std::string tpsCommandFormat = "[TPS]: %tps%";
    std::string serverIp = "127.0.0.1";
    ushort serverPort = 19132;
  } extras{};
};

} // namespace mintvanilla