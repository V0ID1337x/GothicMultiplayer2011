
/*
MIT License

Copyright (c) 2022 Gothic Multiplayer Team.

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

#include <string>

#include "CHeroClass.h"
#include "CLanguage.h"
#include "CObservation.h"
#include "CPlayer.h"
#include "CSpawnPoint.h"
#include "CSyncFuncs.h"
#include "HooksManager.h"
#include "Network.h"
#include "VoiceCapture.h"
#include "VoicePlayback.h"
#include "WorldBuilder\load.h"

#define WARN(x) MessageBoxA(NULL, x, x, 0)

enum FILE_REQ { CLASS_FILE = 1, SPAWN_FILE = 2, WB_FILE = 3, NULL_SIZE = 255 };

struct MD5Sum {
  BYTE data[16];
};

union STime {
  int time;
  struct {
    unsigned short day;
    unsigned char hour, min;
  };
};

class CGmpClient : public CSyncFuncs {
public:
  CGmpClient(const char* ip, CLanguage* ptr);
  ~CGmpClient(void);
  void HandleNetwork(void);
  bool IsConnected(void);
  bool Connect(void);
  void PrepareToJoin(void);
  void JoinGame(BYTE selected_class);
  zSTRING& GetLastError(void);
  void SendDropItem(short Instance, short amount);
  void SendTakeItem(short Instance);
  void SendCastSpell(oCNpc* Target, short SpellId);
  void SendMessage(const char* msg);
  void SendWhisper(const char* player_name, const char* msg);
  void SendRemoteMessage(const char* msg);
  void SendVoice();
  void UpdatePlayerStats(short anim);
  void SendHPDiff(size_t who, short diff);
  void SyncGameTime(void);
  void Disconnect(void);
  void DownloadWBFile(void);
  void DownloadClassFile(void);
  void DownloadSpawnpointsFile(void);
  MD5Sum* GetMD5(LPBYTE data, DWORD size);
  BYTE AreDefaultItems(void);  // 0 - yes| 1 - no
  void RestoreHealth(void);

  CHeroClass* classmgr;
  CSpawnPoint* spawnpoint;
  VoiceCapture* voiceCapture;
  VoicePlayback* voicePlayback;
  vector<CPlayer*> player;
  std::vector<Info> VobsWorldBuilderMap;
  int HeroLastHp;
  zSTRING map;
  bool IsAdminOrModerator;
  bool IgnoreFirstTimeMessage;
  bool IsInGame;
  unsigned char game_mode;
  short mp_restore;
  int DropItemsAllowed;
  int ForceHideMap;
  CObservation::ObseravtionMode ObserveMode;
  CLanguage* lang;
  Network* network;
  BOOL IsReadyToJoin;

private:
  int clientPort;
  std::string clientHost;
  BYTE tmp8[2];
  time_t last_mp_regen;

  std::string GetServerAddresForHTTPDownloader();
};

/*
Streszczenie działania:
Całą robotę z połączeniem i innymi bajerami sieciowymi będzie zajmowała się metoda HandleNetwork;
HandleNetwork będzie wywoływał odpowiednie metody zależnie od warstwy na której będzie pracował.
Warstwy:
0 - ładujemy podstawe dane z serwera i wybieramy klasę postaci
0a - wysyłamy crc mapy(może za to dostaniemy bana :D)
0b - pobieramy liste klas
0c - pobieramy liste spawnpointow
1 - po wstępnych ustawieniach dołączamy do gry
2 - zarzynamy się nawzajem/robimy sesje rpg/whatever
*/
