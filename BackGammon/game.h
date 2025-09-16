#pragma once

#include "raylib.h"
#include "images.h"
#include <string>
#include <vector>

struct Stap // per stap per steen per dobbelsteen
{
    int kegelvan;  // indien 0 dan komt de steen vanuit de geslagen stapel
    int kegelnaar; // indien 0 dan is de steen uitgenomen
    int geslagen;  // geslagen steen of 0 (Niemand)
    int ogen;      // gebruikte aantal ogen op een dobbelsteen
}; // end of Stap

struct Zet // 1..4 stappen
{    
    Stap stappen[4]; // 1 tot 4 stappen
    int stapteller; // elke stap komt overeen met een dobbelsteen
    int score;
};

struct BewaarZet
{
    int zetnr;
    Zet zetten[4];
};

class Game_t
{
private:
    // data
    int board[25] =
        {
            0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0};
    int geslagen[3] =
        {
            0,
            0,
            0};
    int uitgenomen[3] =
        {
            0,
            0,
            0};
    std::vector<Zet> zetten;
    std::vector<Zet> halfzetten;       
    std::vector<Stap> stappen[4];
    BewaarZet bewaar; 
    Zet bestezet = Zet();
    bool aigezet = false;
    int bestescore = 0; // hoe lager hoe beter
    int ogen[4];        // houdt rekening met dubbel gooien
    int keep[4];        // backup van ogen
    bool dubbel = false;
    bool opening = false;
    int spelstatus = 0;
    int teller = 0;
    int speler_ad_beurt = 0;
    int speler_winnaar = 0;
    bool enter_visible = false;
    bool speler_continue = false;
    bool spel_continue = false;
    int kegel_sel_from = -1;
    int kegel_sel_to = -1;
    std::string tekstinfo = "Welkom tot het BackGammon van Peter Veenendaal";
    std::string tekstfout = "";
    std::string instructie = "";
    std::string geworpen = "";
    bool kegelvan[25];
    bool kegelnaar[25];

    // methodss
    // init methods
    void InitGame();
    void InitZet(Zet *zet);
    void InitVlagVan();
    void InitVlagNaar();
    void InitVlaggen();

    // speler functions
    void Dice_Roll(); // Werp met de dobbelstenen
    void Do_Ai_Move(); // Zoek en doe de beste zet voor de Ai
    void DrawStone(int x, int y, int z, int n, Color c1, Color c2); // teken een meer stenen
    bool MagUithalen(int speler); // Bepaal of er stenen uitgehaald mogen worden
    void BepaalZetten(int speler); // vul de zetten(list)
    void BepaalStappen(int speler, int index); // vul de stappen(list)[index]
    void BepaalVervolgStappen(int speler); // vul de overige stappen(list) en combineer deze tot de zetten(list)
    void BepaalStappenVerplaatst(int speler, int index); // vul de stappen(list)[index] met verplaatse stenen
    void BepaalStappenTeruggezet(int speler, int index); // vul de stappen(list)[index] met teruggezette stenen
    void BepaalStappenUitgenomen(int speler, int index); // vul de stappen(list)[index] met uitgenomen stenen
    bool KanSlaan(int k, int opp); // Kan de steen op kegel k worden geslagen?
    int TelDobbelstenen(); // Tel de dobbelstenen die gebruikt kunnen worden
    void MaakStap(int van, int naar, int geslagen, int ogen, Zet *zet); // Maak een nieuwe stap
    void AddStap(Stap stap, Zet *zet);
    void DoeZet(int speler, Zet *zet); // kunnen 1 .. 4 stappen zijn
    void NeemZetTerug(int speler, Zet *zet); // maak een uitgevoerde zet ongedaan
    void BesteZet(int speler); // Zoek de beste zet voor de speler
    void PasOgenAan(int stapogen); // pas de ogen aan met de gebruikte ogen van de stap
    void BewaarOgen(); // Maak een backup van de ogen 
    void HerstelOgen(); // Herstel de ogen
    int BepaalScore(Zet *zet); // bepaal de score na het uitvoeren van een zet
    std::string ZetToString(Zet *zet); // Vertaal een zet naar een string
    std::string PositieToString(); // Vertaal de current positie naar een string
    void Zetkegelvan(); // Zet de vlaagen voor de kegels waarvan kan worden gezet
    void Zetkegelnaar(); // Zet de vlaggen voor de kegels waarnaar gezet kunnen worden
    int BepaalKegelpositie(int x, int y, int van_kegel, int naar_kegel); // Bepaal de kegel waar de mouseclick heeft plaats gevonden
    void VervolgZetten(int &retFlag); // Vindt vervolgzetten voor de Speler

public:
    // Constructor
    Game_t();
    // Destructor
    ~Game_t();
    // main functions
    void KeyPress(int key); // Handle the key press
    void MousePress(int x, int y); // Handle the mouse press
    void Update(); // Update the gui
    void Draw(Images_t *img); // Draw the gui
}; // end class Game_t
