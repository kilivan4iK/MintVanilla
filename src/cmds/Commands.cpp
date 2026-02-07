#include <ll/api/memory/Hook.h>
#include <sys/timeb.h>

#include <ll/api/service/Bedrock.h>
#include <mc/world/level/Level.h>

#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>

#include <mc/server/DedicatedServer.h>
#include <mc/server/commands/StopCommand.h>
#include <mc/server/commands/CommandFlag.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>

#include <mc/network/packet/TransferPacket.h>

#include <MintVanilla.h>
#include <Config.h>

const float resetTicks = 20.0;
int ticks = 0;
long timeOld = 0;
long timeNew = 0;

long getTimestamp() {
  timeb t;
  ftime(&t);
  return (long)t.time * 1000 + t.millitm;
}

std::string getTps() {
  double input = 1000.0 * (resetTicks / (timeNew - timeOld));
  auto output = std::to_string(input);
  if (input >= 18.5) {
    return "§a" + output + "§r";
  }
  else if (input > 15.0) {
    return "§6" + output + "§r";
  }
  else return "§c" + output + "§r";
}

LL_TYPE_INSTANCE_HOOK(
  LevelTickHook,
  ll::memory::HookPriority::Normal,
  Level,
  &Level::$tick,
  void
) {
  ticks++;
  if (ticks == resetTicks) {
    ticks = 0;
    timeOld = timeNew;
    timeNew = getTimestamp();
  }
  return origin();
}

namespace mintvanilla::command {

struct RestartParams {
  // int timeout = 20;
  bool reconnect = true;
};

void registerRestartCommand() {
  auto& cmd = ll::command::CommandRegistrar::getInstance(false).getOrCreateCommand(
    "restart",
    "Restart server",
    CommandPermissionLevel::GameDirectors,
    CommandFlagValue::None
  );

  cmd.overload<RestartParams>()
  // .optional("timeout")
  .optional("reconnect").execute(
    [](CommandOrigin const&, CommandOutput&, RestartParams const& param) {
      // int timeout = !param.timeout ? 20 : param.timeout;
      bool reconnect = param.reconnect ? true : false;

      if (reconnect) {
        TransferPacket packet(mintvanilla::MintVanilla::getInstance().getConfig().extras.serverIp, mintvanilla::MintVanilla::getInstance().getConfig().extras.serverPort);
        packet.sendToClients();
      }

      if (StopCommand::mServer()) StopCommand::mServer()->requestServerShutdown();
    }
  );
}

void registerTpsCommand() {
  static ll::memory::HookRegistrar<LevelTickHook> leveltick{};
  
  auto& cmd = ll::command::CommandRegistrar::getInstance(false).getOrCreateCommand(
    "tps",
    "Get server ticks per seconds",
    mintvanilla::MintVanilla::getInstance().getConfig().extras.tpsCommandAny ? CommandPermissionLevel::Any : CommandPermissionLevel::GameDirectors,
    CommandFlagValue::None
  );

  cmd.overload<>().execute(
    [](CommandOrigin const&, CommandOutput& output) {
      std::string outputMessage = mintvanilla::MintVanilla::getInstance().getConfig().extras.tpsCommandFormat;
      outputMessage = outputMessage.replace(outputMessage.find("%tps%"), sizeof("%tps%") - 1, getTps());

      output.success(outputMessage);
    }
  );
}

} // namespace mintvanilla::command
