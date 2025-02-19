

/*
MIT License

Copyright (c) 2022 Gothic Multiplayer Team (pampi, skejt23, mecio)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <string.h>
#include <memory>
#include <string>
#include <vector>
#include <ctime>
#include <future>
#include "config.h"
#include "CClasses.h"
#include "CPermissions.h"
#include "znet_server.h"
#include "Script.h"

#define DEFAULT_PORT 0xDEAD
#define DEFAULT_ADMIN_PORT 0x404

class CLog;
class GothicClock;

enum CONFIG_FLAGS{
	QUICK_POTS		= 0x01,
	DROP_ITEMS		= 0x02,
	HIDE_MAP		= 0x04,
	MANA_REGENERATION	= 0x08,
};

struct Packet {
	// Not owning.
  unsigned char* data = nullptr;
  std::uint32_t length = 0;
	Net::PlayerId id;
};

class CGmpServ : public Net::PacketHandler
{
public:
	enum PL_FLAGS{
		PL_UNCONCIOUS = 0x01,   //00000001
		PL_BURN = 0x02, //00000010
	};
	enum FILE_REQ{
		CLASS_FILE	= 1,
		SPAWN_FILE	= 2,
		WB_FILE		= 3,
		NULL_SIZE	= 255
	};
	struct sPlayer {
		Net::PlayerId id;
		std::string name;
		unsigned char char_class, flags, head, skin, body, walkstyle, figth_pos, spellhand, headstate, has_admin,
				admin_passwd, moderator_passwd, is_ingame, passed_crc_test, mute;
		SModerator *moderator;
		short health, mana;
		float pos[3];
		float nrot[3];
		// miejce na obrót głowy
		time_t tod;  // time of death
		unsigned short left_hand, right_hand, armor, rangedeq, meleeeq, animation;
	};

public:
	CGmpServ(int argc, char **argv);

	~CGmpServ();
	void AddToPublicListHTTP();
	bool Receive();
	bool HandlePacket(Net::PlayerId playerId, unsigned char* data, std::uint32_t size);
	void Run();
	bool Init();
	void SaveBanList(void);
	bool IsPublic(void);
	void DoRespawns(void);
	void SendSpamMessage(void);
	void RegenerationHPMP(void);
private:
	void HTTPServerThread(std::int32_t port);
	void DeleteFromPlayerList(Net::PlayerId guid);
	sPlayer* FindPlayer(const char* nickname);
	void LoadBanList(void);
	void HandleCastSpell(Packet p, bool Target);
	void HandleDropItem(Packet p);
	void HandleTakeItem(Packet p);
	void HandleVoice(Packet p);
	void SomeoneJoinGame(Packet p);
	void HandlePlayerUpdate(Packet p);
	void MakeHPDiff(Packet p);
	void HandleNormalMsg(Packet p);
	void HandleWhisp(Packet p);
	void HandleRMConsole(Packet p);
	void HandleGameInfo(Packet p);
	void HandleMapNameReq(Packet p);
	void SendDisconnectionInfo(uint64_t disconnected_id);
	void SendDeathInfo(uint64_t deadman);
	void SendRespawnInfo(uint64_t luckyguy);
	void SendGameInfo(Net::PlayerId guid);
	size_t FindIDOnList(uint64_t guid);

	std::vector<std::string> ban_list;
	CClassManager *classmgr;
    Script * script;
	time_t last_stand_timer;
	time_t regen_time;
	
	unsigned char GetPacketIdentifier(const Packet& p);
	int serverPort;
	int arg_count;
	char **arg_vec;
	unsigned short maxConnections;
	time_t spam_time;
	std::vector<sPlayer> players;
	bool allow_modification = false;
	std::string loop_msg;
	Config config_;
	std::unique_ptr<GothicClock> clock_;
	std::future<void> http_thread_future_;
	std::future<void> public_list_http_thread_future_;
};
