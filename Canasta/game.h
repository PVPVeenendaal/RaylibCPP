#pragma once

#include "stapel.h"
#include "images.h"
#include <string>

class Game_t
{
private:
    // data
    Stapel_t *team_uitgelegd[2][12] =
        {
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
             nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
             nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}};
    Stapel_t *spelers[4] = {nullptr, nullptr, nullptr, nullptr}; // kaarten in de hand
    Stapel_t *trekstapel = nullptr;
    Stapel_t *aflegstapel = nullptr;
    Stapel_t *kaarttypen[15] = {nullptr, nullptr, nullptr, nullptr, nullptr,
                                nullptr, nullptr, nullptr, nullptr, nullptr,
                                nullptr, nullptr, nullptr, nullptr, nullptr}; // speler_ad_beurt
    int totaal_punten[2] = {0, 0};
    int spelstatus = 0;
    int teller = 0;
    int deler = 0;
    int speler_ad_beurt = 0;
    int speler_winnaar = 0;
    int team_winnaar = 0;
    int afleg_kaart = 0;
    int afleg_kaart_actie = 0;
    int trek_kaart = -1;
    bool aflegstapel_oppakken = false;
    bool enter_visible = false;
    bool ronde_continue = false;
    bool ronde_geteld = false;
    bool spel_continue = false;
    int overzicht[2][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    std::string tekstinfo = "Welkom tot het Canasta van Peter Veenendaal";
    std::string tekstfout = "";
    
    // methods
    // init methods
    void InitGame();
    void InitRonde();
    // team functions
    int GetMinimumPunten(int team);
    bool HeeftCanastaGelegd(int team);
    bool HeeftUitgelegd(int team);
    int TeamUitlegPunten(bool inclusief, int team);

    // game functions
    void VulDeck();
    void SchudDeck();
    void DeelDeck();
    bool Trekkaart(int ontvanger, bool show);

    // speler functions
    void Do_Ai_Move(int speler);
    void Do_Ai_Werpeenkaart(int speler);
    void Do_Ai_Uitleggen(int speler);
    void Do_Ai_Trekkaart(int speler);
    void UpdateAflegstapel(int speler);
    void UpdateTeam(int speler, int col);
    bool MagOppakken(int speler);
    bool WilOppakken(int speler);
    int BepaalUitlegWaarde(int speler, int ak);
    bool KanUitleggen(int speler, int col);
    bool KanAllesUitleggen(int speler);
    void LegAllesUit(int speler);
    bool LegEenStapelUit(int speler);
    void LegExtraJokersUit(int speler, bool test);
    void VulTypeKaarten(int speler);
    int ZoekDrieKaart(int speler);
    int ZoekTweeKaart(int speler); // met 1 joker
    void SetKaarten(int speler, int u);
    int KanCanastaWordenGemaakt(int speler);
    void KaartenBijleggen(int speler, int ak);
    void CanastaUitleggen(int speler);
    void StappenUitleggen(int speler, int ak);
    void ExtraKaartenUitleggen(int speler);
    void KaartenUitleggen(int speler);

public:
    // Constructor
    Game_t();
    // Destructor
    ~Game_t();
    // main functions
    void KeyPress(int key);
    void MousePress(int x, int y);
    void Update();
    void Draw(Images_t *img);
}; // end class Game_t
