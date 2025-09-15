#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include "images.h"
#include "game.h"

#if defined(NDEBUG)
#define ASSERT(nop) (void *)0
#else
#define ASSERT(expr)                          \
    if (!(expr))                              \
    {                                         \
        std::cout << "Assertion is not true:" \
                  << #expr                    \
                  << " in file " << __FILE__  \
                  << " on line " << __LINE__; \
        abort();                              \
    }
#endif

// version
const std::string version = "0.80";

// sizes

const int BOARD_SIZE = 740;
const int DICE_SIZE = 225;
const int KOLOM_SIZE = 90;
const int REST_SIZE = 10;
const int VERTICALLINE_SIZE = 20;
const int SCREEN_WIDTH_SIZE = 1600;
const int SCREEN_HEIGHT_SIZE = 900;
const int STONE_RADIUS = 28;

// correctie single stone

const int SINGLE_STONE = 30;

// correctie thuisvak speler

const int THUIS_VAK_BGS = 6;

// Toon aantal stenen

const int SHOW_STONE = 6;

// Punten MagUithalen

const int MAG_UITHALEN_CORRECTIE = 18;

// enums
enum eImages
{
    Enter,
    Dice1,
    Dice2,
    Dice3,
    Dice4,
    Dice5,
    Dice6,
};

enum eSpeler
{
    Niemand,
    BGSpeler,
    Ai,
};

enum eStatuscode
{
    Spelstart,
    Spelspeel,
    Spelerstart,
    Spelerspeel,
    Spelerstop,
    Spelstop,
};

// tabellen
const std::string TekstUitleg[] = {
    "BACKGAMMON",
    "Backgammon is een bordspel voor 2 personen en wordt gespeeld met 15 witte stenen van Ai en 15 zwarte stenen van Jou en 2 dobbelstenen.",
    "De stenen staan op kegels die genummerd zijn van 1 t/m 24, jij speelt van onderen naar boven (24 . 1) terwijl de Ai andersom speelt.",
    "Begin opstelling van jou: kegel 6: 5, kegel 8: 3, kegel 13: 5, kegel 24: 2.",
    "Begin opstelling van Ai : kegel 19: 5, kegel 17: 3, kegel 12: 5, kegel 1: 2.",
    "DOEL VAN HET SPEL",
    "Het doel is om al jouw stenen in het thuisvak van jezelf te krijgen. Is dat gelukt dan mag je je stenen van daaruit wegspelen. ",
    "Wie dat als eerste voor elkaar heeft gekregen wint het spel.",
    "Stenen worden verplaatst door te gooien met twee dobbelstenen en daarna, volgens de regels, de stenen te verzetten. ",
    "Stenen kunnen onderweg geslagen worden, maar alle geslagen stenen moeten wel weer in het spel worden gebruikt direct daarna. ",
    "Bij het slaan van een steen moet je wel weer helemaal vanaf het andere thuisvak starten met deze steen, waardoor het langer duurt",
    "om het spel als eerste uit te spelen. Je moet gooien en dan hopen op een vrije plek volgens één van de ogen van de dobbelsteen.",
    "REGELS STENEN VERPLAATSEN",
    "Je mag na het gooien stenen naar keuze per aantal per dobbelsteen verplaatsen, maar alleen naar een kegel die nog open is.",
    "Een kegel is open als er minder dan 2 stenen van een tegenstander staan of als er eigen stenen op staan.",
    "Staat er 1 steen van je tegenstander, dan sla je die steen op dat moment. De steen wordt op de middenbalk gezet en die moet weer als",
    "eerste in het spel worden gebracht bij een volgende beurt, door een aantal ogen te gooien waar een vrij vak is om de steen te plaatsen",
    "(gezien vanaf de kant van de tegenstander).",
    "Het aantal gegooide ogen van de dobbelstenen tellen afzonderlijk als getallen voor de verplaatsing. Dat wil zeggen; als je een 5 en een 2",
    "gooit dan mag je 5 + 2 stappen vooruit, als de vakken vrij zijn bij de tussenpassen van 5 en 2 die je dan eigenlijk maakt.",
    "Je mag ook twee stenen verplaatsen, elk volgens het aantal ogen op 1 dobbelsteen.",
    "Als je dubbel gooit dan mag je twee keer het aantal gegooide ogen gebruiken, met dubbel vier mag je dus vier keer een stap van vier ogen",
    "doen met een steen. Je moet altijd de zetten van de twee dobbelstenen doen als dat kan. Kun je maar één dobbelsteen gebruiken, ",
    "dan moet je de hoogst bruikbare kiezen.",
    "WINNEN MET BACKGAMMON",
    "Je wint het spel als je je stenen weet af te spelen vanuit je thuisvak. Dit wordt aangegeven in het spel. (bij de startopstelling staan",
    "er al 5 stenen van jou kleur op kegel 6). Je mag pas beginnen met uitspelen als al jou stenen in je thuisvak staan. Je speelt van daaruit",
    "je stenen van het bord om precies het aantal ogen te gooien wat nodig is om van het bord te komen. Simpelweg, als je een steen op kegel 5",
    "hebt, dan moet je een vijf gooien. stenen op de laatste kegel (kegel 1) speel je weg door een 1 te gooien.",
    "Gooi je met een dobbelsteen een aantal waarvan geen stenen op de kegel staan, dan moet je een normale verplaatsing doen met de stenen die",
    "nog verder weg staan. Alleen in geval van een hoger aantal ogen dan waar nog stenen staan, mag de steen op het hoogste nummer worden",
    "verwijderd.",
    "Het kan voorkomen dat er tijdens het uitspelen een steen wordt geslagen. Deze moet dan net als bij het slaan onderweg weer helemaal vanaf",
    "het begin van het andere thuisveld ingebracht worden via de dobbelsteen en gespeeld worden richting je eigen thuisvak.",
    "Degene die als eerste alle stenen vanaf het eigen thuisvak heeft weggespeelt van het bord heeft het spel gewonnen.",
    "WERKING PROGRAMMA",
    "Klikken op een kegel doe je door het klikken op een van de stenen in de kegel te klikken, bij een lege kegel moet je op de lege circel van",
    "de kegel klikken. Bij een gewone zet moet je eerst de kegel waar de steen staat klikken en daarna op de kegel waar de steen moet gaan staan.",
    "Ook als je meerdere ogen gebruikt voor de zelfde steen moet je eerst de ene zet doen en daarna met dezelfde steen de andere zet maar alleen",
    "als het kan volgens de spelregels. Bij het uitnemen van stenen klik je op de kegel (1..6) waar je steen staat. Bij het terugzetten van",
    "geslagen stenen klik je op de kegel (19/6..24/1) waar de steen moet komen, het 2e cijfer geeft aan het aantal ogen op de dobbelsteen.",
    "Na elke zet die je doet verdwijnt de gebruikte dobbelsteen, als er dubbel wordt gegooid zie je 4 dobbelstenen.",
    "Dit is om aan te geven dat er 4 zetten gedaan kunnen worden indien mogelijk.",
    "Druk of click met de muis op ENTER",
};

const Color KleurUitleg[] = {
    BLUE,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    WHITE,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    WHITE,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    WHITE,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    WHITE,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    GREEN,
};

const std::string SpelerNaam[] = {
    "Niemand",
    "BGSpeler",
    "Ai",
};

const std::string StatuscodeNaam[] = {
    "Spelstart",
    "Spelspeel",
    "Spelerstart",
    "Spelerspeel",
    "Spelerstop",
    "Spelstop",
};

const std::string Kegelnummer[] = {
    "12",
    "11",
    "10",
    " 9",
    " 8",
    " 7",
    " 6",
    " 5",
    " 4",
    " 3",
    " 2",
    " 1",
    "13",
    "14",
    "15",
    "16",
    "17",
    "18",
    "19/6",
    "20/5",
    "21/4",
    "22/3",
    "23/2",
    "24/1",
};

const int Kegelnr[] = {
    12,
    11,
    10,
    9,
    8,
    7,
    6,
    5,
    4,
    3,
    2,
    1,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
};

const int kegelx[] = {
    800,
    1517,
    1391,
    1265,
    1139,
    1013,
    887,
    713,
    587,
    461,
    335,
    209,
    83,
    83,
    209,
    335,
    461,
    587,
    713,
    887,
    1013,
    1139,
    1265,
    1391,
    1517,
};

const int kegely[] = {
    727,
    69,
    69,
    69,
    69,
    69,
    69,
    69,
    69,
    69,
    69,
    69,
    69,
    727,
    727,
    727,
    727,
    727,
    727,
    727,
    727,
    727,
    727,
    727,
    727,
};

const int kegelz[] = {
    -40,
    40,
    40,
    40,
    40,
    40,
    40,
    40,
    40,
    40,
    40,
    40,
    40,
    -40,
    -40,
    -40,
    -40,
    -40,
    -40,
    -40,
    -40,
    -40,
    -40,
    -40,
    -40,
};

const float diameter[] = {48.0, 47.0, 46.0};

const int steenwaarde[] = {0, 1, -1};

// macros

#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define TRL(k, s) (((s) == Ai) ? 25 - (k) : (k))
#define ABS(a) ((a) < 0 ? -(a) : (a))
#define SPL(a) ((a) == 0 ? Niemand : (a) < 0 ? Ai \
                                             : BGSpeler)
#define ONB(a) ((a) >= 1 && (a) <= 24) 

//************************************************************************************************
// main
//************************************************************************************************

/// @brief main program
/// @return result
int main()
{
    std::string title = "BackGammon in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
    // Initialization
    InitWindow(SCREEN_WIDTH_SIZE, SCREEN_HEIGHT_SIZE, title.c_str());
    Images_t images = Images_t();
    Game_t game = Game_t();
    SetTargetFPS(10);

    // main loop
    while (!WindowShouldClose())
    {
        // Update
        // Keypress
        int key = GetKeyPressed();
        if (key > 0)
        {
            // empty buffer
            int keybuf = key;
            while (keybuf > 0)
            {
                keybuf = GetKeyPressed();
            }
            game.KeyPress(key);
        }
        // Mousepress
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int x = GetMouseX();
            int y = GetMouseY();
            game.MousePress(x, y);
        }
        // Actions
        game.Update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.Draw(&images);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context
    return 0;
}

//************************************************************************************************
// Images_t
//************************************************************************************************

/// @brief Constructor
Images_t::Images_t()
{
    image_container[Dice1] = LoadTexture("./resources/1.png");
    image_container[Dice2] = LoadTexture("./resources/2.png");
    image_container[Dice3] = LoadTexture("./resources/3.png");
    image_container[Dice4] = LoadTexture("./resources/4.png");
    image_container[Dice5] = LoadTexture("./resources/5.png");
    image_container[Dice6] = LoadTexture("./resources/6.png");
    image_container[Enter] = LoadTexture("./resources/enterbutton.png");
    image_container[Enter].width = KOLOM_SIZE * 2;
    image_container[Enter].height = KOLOM_SIZE;
    for (int i = 1; i <= 6; ++i)
    {
        image_container[i].width = KOLOM_SIZE;
        image_container[i].height = KOLOM_SIZE;
    }
}

/// @brief destructor
Images_t::~Images_t()
{
    for (int i = 0; i < 7; ++i)
    {
        UnloadTexture(image_container[i]);
    }
}

/// @brief Get an image conatiner
/// @param id number in the array
/// @return image container
Texture2D Images_t::GetImage(int id)
{
    ASSERT(id >= 0 && id < 7);
    return image_container[id];
}

//************************************************************************************************
// Game_t
//************************************************************************************************

/// @brief Initialise the game
void Game_t::InitGame()
{
    InitVlaggen();
    for (int s = BGSpeler; s <= Ai; ++s)
    {
        geslagen[s] = 0;
        uitgenomen[s] = 0;
    }
    spel_continue = false;
    teller = 0;
    // begin opstelling
    for (int i = 1; i <= 24; ++i)
    {
        board[i] = 0;
        if (i < 4)
        {
            ogen[i] = 0;
        }
    }
    // beginopstelling
    board[1] = -2;
    board[6] = 5;
    board[8] = 3;
    board[12] = -5;
    board[13] = 5;
    board[17] = -3;
    board[19] = -5;
    board[24] = 2;
#ifndef NDEBUG
    Zet zet = Zet();
    bestescore = BepaalScore(&zet);
    std::string s = PositieToString();
#endif
}

/// @brief Initialiseer een Zet
/// @param zet Pointer to Zet
void Game_t::InitZet(Zet *zet)
{
    zet->stapteller = 0;
    zet->score = 999999;
}

/// @brief Initialiseer de vlaggen voor de kegels van
void Game_t::InitVlagVan()
{
    for (int i = 0; i <= 24; ++i)
    {
        kegelvan[i] = false;
    }
}

/// @brief Initialiseer de vlaggen voor de kegels naar
void Game_t::InitVlagNaar()
{
    for (int i = 0; i <= 24; ++i)
    {
        kegelnaar[i] = false;
    }
}

/// @brief Initialiseer alle vlaggen
void Game_t::InitVlaggen()
{
    InitVlagVan();
    InitVlagNaar();
}

/// @brief Werp de dobbelstenen
void Game_t::Dice_Roll()
{
    dubbel = false;
#ifdef NDEBUG
    ogen[0] = GetRandomValue(1, 6);
    ogen[1] = GetRandomValue(1, 6);
#else
    ogen[0] = 1;
    ogen[1] = 1;
    ogen[0] = GetRandomValue(1, 6); // zet uit bij een test
    ogen[1] = GetRandomValue(1, 6); // zet uit bij een test
#endif
    if (ogen[1] > ogen[0])
    {
        int tmp = ogen[0];
        ogen[0] = ogen[1];
        ogen[1] = tmp;
    }
    if (ogen[0] == ogen[1])
    {
        dubbel = true;
        ogen[2] = ogen[0];
        ogen[3] = ogen[1];
        if (speler_ad_beurt == BGSpeler)
        {
            tekstinfo = geworpen = "Jij hebt dubbel " + std::to_string(ogen[0]) + " gegooid, kies je zet";
        }
        else
        {
            ogen[0] = -ogen[0];
            ogen[1] = -ogen[1];
            ogen[2] = -ogen[2];
            ogen[3] = -ogen[3];
            tekstinfo = geworpen = "Ai heeft dubbel " + std::to_string(ABS(ogen[0])) + " gegooid, bereken zet(ten)";
        }
    }
    else
    {
        ogen[2] = 0;
        ogen[3] = 0;
        if (speler_ad_beurt == BGSpeler)
        {
            tekstinfo = geworpen = "Jij hebt " + std::to_string(ogen[0]) + " en " + std::to_string(ogen[1]) + " gegooid, kies je zet";
        }
        else
        {
            ogen[0] = -ogen[0];
            ogen[1] = -ogen[1];
            tekstinfo = geworpen = "Ai heeft " + std::to_string(ABS(ogen[0])) + " en " + std::to_string(ABS(ogen[1])) + " gegooid, bereken zet(ten)";
        }
    }
}

/// @brief Do de stappen voor de Ai
/// @param speler eSpeler, speler aan de beurt
void Game_t::Do_Ai_Move()
{
    if (!opening)
    {
        BepaalZetten(Ai);
        if (zetten.size() == 0)
        {
            tekstinfo = "Ai kan niet zetten";
            instructie = "Klik op F6 om verder te gaan ";
            spelstatus = Spelerstop;
            return;
        }
    }
    BesteZet(Ai);
    DoeZet(Ai, &bestezet); // geen score meer nodig
    opening = false;
    gezet += ZetToString(&bestezet);
    tekstinfo = "Ai is klaar met zijn beurt";
    instructie = "Klik op F6 om verder te gaan ";
    spelstatus = Spelerstop;
}

/// @brief Constructor
Game_t::Game_t()
{
    zetten = std::vector<Zet>();
    halfzetten = std::vector<Zet>();
    for (int i = 0; i < 4; ++i)
    {
        stappen[i] = std::vector<Stap>();
    }
    teller = 0;
#ifdef NDEBUG
    speler_ad_beurt = GetRandomValue(BGSpeler, Ai);
    opening = speler_ad_beurt == BGSpeler;
#else
    speler_ad_beurt = Ai;
    opening = false;
#endif
    tekstinfo = "Welkom tot het BackGammon van Peter Veenendaal";
    tekstfout = "";
    spelstatus = Spelstart;
}

/// @brief Destructor
Game_t::~Game_t()
{
    zetten.clear();
    halfzetten.clear();
    for (int i = 0; i < 4; ++i)
    {
        stappen[i].clear();
    }
}

/// @brief handle key press
/// @param key keycode
void Game_t::KeyPress(int key)
{
    if (enter_visible && key == KEY_ENTER)
    {
        enter_visible = false;
        spelstatus = Spelspeel;
    }
    if (spelstatus == Spelerspeel && speler_ad_beurt == BGSpeler && key == KEY_F7)
    {
        kegel_sel_from = -1;
        kegel_sel_to = -1;
        InitVlaggen();
        Zetkegelvan();
        tekstinfo = geworpen;
        tekstfout = "";
        if (geslagen[BGSpeler] > 0)
        {
            instructie = "Click op een geslagen steen om deze te selecteren";
        }
        else
        {
            instructie = "Click op een steen in de kegels met een groene ster om deze te selecteren";
        }
    }
    if (spelstatus == Spelerstop && key == KEY_F6)
    {
        tekstfout = "";
        speler_continue = true;
    }
    if (spelstatus == Spelstop && key == KEY_F5)
    {
        tekstfout = "";
        spel_continue = true;
    }
}

/// @brief handle mouse press
/// @param x mouse column
/// @param y mouse row
void Game_t::MousePress(int x, int y)
{
    // click on enter button
    if (y >= 828 && y <= 894 && x >= 1376 && x <= 1539 && enter_visible)
    {
        KeyPress(KEY_ENTER);
    }
#ifndef NDEBUG
    std::cout << "x=" << x << " y=" << y << std::endl;
#endif
    if (x >= 20 && x < 1580 && y >= 20 && y < 776 && spelstatus == Spelerspeel && speler_ad_beurt == BGSpeler)
    {
        int kegelpos = -1;
        int slagpos = -1;

        if (geslagen[BGSpeler] > 0)
        {
            if (kegel_sel_from == -1)
            {
                int n = MIN(geslagen[BGSpeler], 5);
                if (n == 0)
                {
                    n = 1;
                }
                int minx = kegelx[0] - (int)diameter[1];
                int maxx = kegelx[0] + (int)diameter[1];
                int miny = kegely[0] + kegelz[0] * n - 5;
                int maxy = kegely[0] + (int)diameter[1];
                if (x >= minx && x <= maxx && y >= miny && y <= maxy)
                {
                    kegel_sel_from = 0;
                    tekstinfo = geworpen;
                    instructie = "click op de kegel waar de geslagen steen moet worden terug geplaatst";
                    InitVlagVan();
                    kegelvan[kegel_sel_from] = true;
                    Zetkegelnaar();
                }
            }
            else
            {
                kegelpos = BepaalKegelpositie(x, y, 19, 24);
                bool gevonden = false;
                int posgeslagen = Niemand;
                if (kegelpos > -1)
                {
                    for (auto stap : stappen[0])
                    {
                        if (stap.kegelvan == 0 && stap.kegelnaar == kegelpos)
                        {
                            gevonden = true;
                            posgeslagen = stap.geslagen;
                            break;
                        }
                    }
                }
                if (gevonden)
                {
                    Zet z = Zet();
                    InitZet(&z);
                    MaakStap(0, kegelpos, posgeslagen, 25 - kegelpos, &z);
                    DoeZet(BGSpeler, &z);
                    gezet += ZetToString(&z);
                    PasOgenAan(z.stappen[0].ogen);
                    int retFlag;
                    VervolgZetten(retFlag);
                    if (retFlag == 2)
                    {
                        return;
                    }
                    if (geslagen[BGSpeler] > 0)
                    {
                        instructie = "Click op een geslagen steen om deze te selecteren";
                    }
                    else
                    {
                        instructie = "Click op een steen in de kegels met een groene ster om die te selecteren";
                    }
                }
                else
                {
                    tekstfout = "De aangeklikte kegel " + std::to_string(kegelpos) + " komt niet voor in de lijst van zetten";
                }
            }
        }
        else
        {
            if (MagUithalen(BGSpeler) && kegel_sel_from == -1)
            {
                kegelpos = BepaalKegelpositie(x, y, 1, 6); // alleen de kegels in het thuisvak
                Stap stapvan;
                if (kegelpos >= 1 && kegelpos <= 6)
                {
                    bool gevonden = false;
                    for (auto stap : stappen[0])
                    {
                        if (stap.kegelvan == kegelpos && stap.kegelnaar == 0)
                        {
                            stapvan = stap;
                            gevonden = true;
                            break;
                        }
                    }
                    if (gevonden)
                    {
                        --board[kegelpos];
                        ++uitgenomen[BGSpeler];
                        Zet z = Zet();
                        InitZet(&z);
                        AddStap(stapvan, &z);
                        gezet += ZetToString(&z);
                        PasOgenAan(z.stappen[0].ogen);
                        int retFlag;
                        VervolgZetten(retFlag);
                        return;
                    }
                    else
                    {
                        bool gevonden = false;
                        for (auto stap : stappen[0])
                        {
                            if (stap.kegelvan == kegelpos && stap.kegelnaar > 0)
                            {
                                gevonden = true;
                                break;
                            }
                        }
                        if (!gevonden)
                        {
                            tekstfout = "Op de geselecteerde kegel staan geen stenen van jou";
                            return;
                        }
                    }
                }
            }
            // bepaal kegel
            kegelpos = BepaalKegelpositie(x, y, 1, 24);
            if (ONB(kegelpos))
            {
                if (kegel_sel_from == -1)
                {
                    // kegel van
                    kegel_sel_from = kegelpos;
                    bool gevonden = false;
                    for (auto stap : stappen[0])
                    {
                        if (stap.kegelvan == kegel_sel_from)
                        {
                            gevonden = true;
                            break;
                        }
                    }
                    if (!gevonden)
                    {
                        tekstfout = "De aangeklikte kegel " + std::to_string(kegelpos) + " komt niet voor in de lijst van mogelijke zetten";
                        kegel_sel_from = -1;
                    }
                    else
                    {
                        tekstinfo = "Je hebt een steen op " + std::to_string(kegelpos) + " geselecteed";
                        instructie = "Kies een kegel met gele ster om deze te plaatsen met F7 maak je de keuze ongedaan.";
                        tekstfout = "";
                        InitVlagVan();
                        kegelvan[kegel_sel_from] = true;
                        Zetkegelnaar();
                    }
                }
                else
                {
                    // kegel naar
                    kegel_sel_to = kegelpos;
                    bool gevonden = false;
                    for (auto stap : stappen[0])
                    {
                        if (stap.kegelvan == kegel_sel_from && stap.kegelnaar == kegel_sel_to)
                        {
                            gevonden = true;
                            Zet z = Zet();
                            InitZet(&z);
                            MaakStap(kegel_sel_from, kegel_sel_to, stap.geslagen, stap.ogen, &z);
                            DoeZet(BGSpeler, &z);
                            gezet += ZetToString(&z);
                            PasOgenAan(z.stappen[0].ogen);
                            int retFlag;
                            VervolgZetten(retFlag);
                            if (retFlag == 2)
                            {
                                return;
                            }
                        }
                    }
                    if (!gevonden)
                    {
                        tekstfout = "De aangeklikte kegel " + std::to_string(kegelpos) + " niet voor in de lijst van mogelijke zetten";
                        kegel_sel_to = -1;
                    }
                }
            }
        }
    }
}

/// @brief pas de ogen aan met de gebruikte ogen van de stap
/// @param stapogen ogen gebruikt binnen de stap
void Game_t::PasOgenAan(int stapogen)
{
    if (dubbel)
    {
        for (int j = 0; j < 3; ++j)
        {
            ogen[j] = ogen[j + 1];
        }
        ogen[3] = 0;
    }
    else
    {
        if (ogen[0] == stapogen)
        {
            ogen[0] = ogen[1];
        }
        ogen[1] = 0;
    }
}

/// @brief Maak een backup van de ogen
void Game_t::BewaarOgen()
{
    for (int i = 0; i < 4; ++i)
    {
        keep[i] = ogen[i];
    }
}

/// @brief Herstel de ogen
void Game_t::HerstelOgen()
{
    for (int i = 0; i < 4; ++i)
    {
        ogen[i] = keep[i];
    }
}

/// @brief Vindt vervolgzetten voor de BGSpeler
/// @param retFlag 1 = doorgaan 2 = stop
void Game_t::VervolgZetten(int &retFlag)
{
    retFlag = 1;
    if (ogen[0] == 0)
    {
        // alle zetten gedaan
        InitVlaggen();
        tekstinfo = "Jij kan niet verder zetten";
        instructie = "Click op F6 om verder te gaan";
        spelstatus = Spelerstop;
        retFlag = 2;
        return;
    }
    else
    {
        kegel_sel_from = -1;
        kegel_sel_to = -1;
        InitVlaggen();
        BepaalZetten(BGSpeler);
        if (stappen[0].size() == 0)
        {
            tekstinfo = "Geen vervolgzetten gevonden";
            instructie = "Click op F6 om verder te gaan";
            spelstatus = Spelerstop;
            retFlag = 2;
            return;
        }
        Zetkegelvan();
        tekstinfo = geworpen;
        if (geslagen[BGSpeler] > 0)
        {
            instructie = "Click op een geslagen steen om deze te selecteren";
        }
        else
        {
            instructie = "Click op een steen in de kegels met een groene ster om die te selecteren";
        }
        return;
    }
}

/// @brief Bepaal de kegel waar de mouseclick heeft plaats gevonden
/// @param x Mouse column
/// @param y Mouse row
/// @param van_kegel vanaf kegel die getest wordt
/// @param naar_kegel naar kegel die getest wordt
/// @return
int Game_t::BepaalKegelpositie(int x, int y, int van_kegel, int naar_kegel)
{
    int kegelpos = -1;
    for (int i = van_kegel; i <= naar_kegel; ++i)
    {
        int n = MIN(ABS(board[i]), SHOW_STONE);
        if (n == 0)
        {
            n = 1;
        }
        int minx = kegelx[i] - (int)diameter[1];
        int maxx = kegelx[i] + (int)diameter[1];
        int miny = (i < 13) ? kegely[i] - (int)diameter[1] : kegely[i] + kegelz[i] * n - 5;
        int maxy = (i < 13) ? kegely[i] + kegelz[i] * n + 5 : kegely[i] + (int)diameter[1];
        if (x >= minx && x <= maxx && y >= miny && y <= maxy)
        {
            kegelpos = i;
            break;
        }
    }
    return kegelpos;
}

/// @brief Update actions
void Game_t::Update()
{
    ++teller;
    if (teller % 10 != 0 || enter_visible)
    {
        return;
    }
    if (spelstatus == Spelstart)
    {
        InitGame();
        enter_visible = true;
        return;
    }
    if (spelstatus == Spelspeel)
    {
        spelstatus = Spelerstart;
        return;
    }
    if (spelstatus == Spelerstart)
    {
        gezet = "Gezet: ";
        InitVlaggen();
        speler_continue = false;
        ++speler_ad_beurt;
        if (speler_ad_beurt > Ai)
        {
            speler_ad_beurt = BGSpeler;
        }
        Dice_Roll();
        if (speler_ad_beurt != BGSpeler)
        {
            tekstfout = "";
        }
        else
        {
            kegel_sel_from = -1;
            kegel_sel_to = -1;
            BepaalZetten(BGSpeler);
            if (stappen[0].size() == 0)
            {
                tekstinfo = "Jij kan niet zetten";
                instructie = "Click op F6 om verder te gaan";
                spelstatus = Spelerstop;
                return;
            }
            Zetkegelvan();
            if (geslagen[BGSpeler] > 0)
            {
                instructie = "Click op een geslagen steen om deze te selecteren";
            }
            else
            {
                instructie = "Click op een steen in de kegels met een groene ster om die te selecteren";
            }
        }
        spelstatus = Spelerspeel;
        return;
    }
    if (spelstatus == Spelerspeel)
    {
        if (speler_ad_beurt != BGSpeler)
        {
            Do_Ai_Move();
        }
        return;
    }
    if (spelstatus == Spelerstop)
    {
        if (speler_continue)
        {
            if (ABS(uitgenomen[speler_ad_beurt]) == 15)
            {
                speler_winnaar = speler_ad_beurt;
                spelstatus = Spelstop;
            }
            else
            {
                spelstatus = Spelerstart;
            }
        }
        return;
    }
    if (spelstatus == Spelstop)
    {
        if (speler_winnaar > Niemand)
        {
            tekstinfo = "Spel is afgelopen, " + SpelerNaam[speler_winnaar] + " is de winnaar...";
            instructie = "Click op F5 om opnieuw te starten";
            tekstfout = "";
        }
        if (spel_continue)
        {
            spelstatus = Spelstart;
        }
        return;
    }
}

/// @brief draw the screen
void Game_t::Draw(Images_t *img)
{
    Color brd = {101, 67, 33, 255};
    Color inn = {250, 250, 210, 255};
    Color alm = {205, 127, 50, 255};
    Color amb = {255, 191, 0, 255};
    Vector2 v1, v2, v3;
    std::string text;

    if (spelstatus == Spelstart)
    {
        ClearBackground(DARKGRAY);
        for (int i = 0; i < 44; ++i)
        {
            DrawText(
                TekstUitleg[i].c_str(),
                REST_SIZE,
                VERTICALLINE_SIZE * i + REST_SIZE,
                20,
                KleurUitleg[i]);
        }
        DrawTexture(
            img->GetImage(Enter),
            SCREEN_WIDTH_SIZE - REST_SIZE - KOLOM_SIZE * 2,
            SCREEN_HEIGHT_SIZE - REST_SIZE - KOLOM_SIZE,
            RAYWHITE);
        return;
    }
    enter_visible = false;
    ClearBackground(WHITE);
    // Draw board
    DrawRectangle(0, 0, 1600, 800, brd);
    DrawRectangle(20, 20, 756, 756, inn);
    DrawRectangle(824, 20, 756, 756, inn);
    for (int i = 0; i < 6; i++)
    {
        // kegel 12 . kegel 7
        v3.x = 20 + 126 * i;
        v3.y = 20;
        v2.x = 20 + 126 * (i + 1);
        v2.y = 20;
        v1.x = 20 + 63 + 126 * i;
        v1.y = 20 + 358;
        if (i % 2 == 0)
        {
            DrawTriangle(v1, v2, v3, alm);
        }
        else
        {
            DrawTriangle(v1, v2, v3, amb);
        }
        DrawText(
            Kegelnummer[i].c_str(),
            20 + 53 + 126 * i,
            0,
            20,
            RAYWHITE);
        if (kegelvan[Kegelnr[i]])
        {
            DrawText(
                "*",
                20 + 13 + 126 * i,
                0,
                30,
                GREEN);
        }
        if (kegelnaar[Kegelnr[i]])
        {
            DrawText(
                "*",
                20 + 13 + 126 * i,
                0,
                30,
                YELLOW);
        }
        // kegel 6 . kegel 1
        v3.x = 824 + 126 * i;
        v3.y = 20;
        v2.x = 824 + 126 * (i + 1);
        v2.y = 20;
        v1.x = 824 + 63 + 126 * i;
        v1.y = 20 + 358;
        if (i % 2 == 0)
        {
            DrawTriangle(v1, v2, v3, alm);
        }
        else
        {
            DrawTriangle(v1, v2, v3, amb);
        }
        DrawText(
            Kegelnummer[i + 6].c_str(),
            824 + 53 + 126 * i,
            0,
            20,
            RAYWHITE);
        if (kegelvan[Kegelnr[i + 6]])
        {
            DrawText(
                "*",
                824 + 13 + 126 * i,
                0,
                30,
                GREEN);
        }
        if (kegelnaar[Kegelnr[i + 6]])
        {
            DrawText(
                "*",
                824 + 13 + 126 * i,
                0,
                30,
                YELLOW);
        }
        // kegel 13 . kegel 18
        v1.x = 20 + 126 * i;
        v1.y = 776;
        v2.x = 20 + 126 * (i + 1);
        v2.y = 776;
        v3.x = 20 + 63 + 126 * i;
        v3.y = 776 - 358;
        if (i % 2 == 0)
        {
            DrawTriangle(v1, v2, v3, amb);
        }
        else
        {
            DrawTriangle(v1, v2, v3, alm);
        }
        DrawText(
            Kegelnummer[i + 12].c_str(),
            20 + 53 + 126 * i,
            778,
            20,
            RAYWHITE);
        if (kegelvan[Kegelnr[i + 12]])
        {
            DrawText(
                "*",
                20 + 13 + 126 * i,
                778,
                30,
                GREEN);
        }
        if (kegelnaar[Kegelnr[i + 12]])
        {
            DrawText(
                "*",
                20 + 13 + 126 * i,
                778,
                30,
                YELLOW);
        }
        // kegel 19 . kegel 24
        v1.x = 824 + 126 * i;
        v1.y = 776;
        v2.x = 824 + 126 * (i + 1);
        v2.y = 776;
        v3.x = 824 + 63 + 126 * i;
        v3.y = 776 - 358;
        if (i % 2 == 0)
        {
            DrawTriangle(v1, v2, v3, amb);
        }
        else
        {
            DrawTriangle(v1, v2, v3, alm);
        }
        DrawText(
            Kegelnummer[i + 18].c_str(),
            824 + 53 + 126 * i,
            778,
            20,
            RAYWHITE);
        if (kegelvan[Kegelnr[i + 18]])
        {
            DrawText(
                "*",
                824 + 13 + 126 * i,
                778,
                30,
                GREEN);
        }
        if (kegelnaar[Kegelnr[i + 18]])
        {
            DrawText(
                "*",
                824 + 13 + 126 * i,
                778,
                30,
                YELLOW);
        }
    }
    if (kegelvan[0])
    {
        DrawText(
            "*",
            800,
            778,
            30,
            GREEN);
    }
    // Draw stones
    for (int i = 1; i <= 24; ++i)
    {
        if (board[i] == 0)
        {
            if (i % 2 == 0)
            {
                DrawStone(kegelx[i], kegely[i], 0, 1, alm, LIGHTGRAY);
            }
            else
            {
                DrawStone(kegelx[i], kegely[i], 0, 1, amb, DARKGRAY);
            }
            continue;
        }
        int s = SPL(board[i]);
        if (s == BGSpeler)
        {
            DrawStone(kegelx[i], kegely[i], kegelz[i], board[i], BLACK, GREEN);
        }
        else
        {
            DrawStone(kegelx[i], kegely[i], kegelz[i], ABS(board[i]), WHITE, RED);
        }
    }
    // Draw captured stones
    if (geslagen[BGSpeler] > 0)
    {
        DrawStone(800, 727, kegelz[24], geslagen[BGSpeler], BLACK, GREEN);
    }
    if (geslagen[Ai] < 0)
    {
        DrawStone(800, 69, kegelz[1], ABS(geslagen[Ai]), WHITE, RED);
    }
    // Draw taken out stones
    if (uitgenomen[BGSpeler] > 0)
    {
        DrawText(
            "Uit:",
            kegelx[22],
            kegely[22] + 120,
            20,
            BLUE);
        DrawStone(kegelx[23], kegely[23] + 123, 0, -1, BLACK, GREEN);
        text = std::to_string(uitgenomen[BGSpeler]);
        DrawText(
            text.c_str(),
            kegelx[23] - 10,
            kegely[23] + 120,
            20,
            WHITE);
    }
    if (uitgenomen[Ai] < 0)
    {
        DrawText(
            "Uit:",
            kegelx[22],
            kegely[22] + 120,
            20,
            BLUE);
        DrawStone(kegelx[24], kegely[24] + 123, 0, -1, WHITE, RED);
        text = std::to_string(ABS(uitgenomen[Ai]));
        DrawText(
            text.c_str(),
            kegelx[24] - 10,
            kegely[24] + 120,
            20,
            BLACK);
    }
    if (gezet != "")
    {
        DrawText(
            gezet.c_str(),
            REST_SIZE + KOLOM_SIZE * 4,
            REST_SIZE * 5 + 9 * KOLOM_SIZE,
            20,
            BLACK);
    }
    if (tekstinfo != "")
    {
        DrawText(
            tekstinfo.c_str(),
            REST_SIZE + KOLOM_SIZE * 4,
            REST_SIZE + 9 * KOLOM_SIZE,
            20,
            BLACK);
    }
    if (instructie != "")
    {
        DrawText(
            instructie.c_str(),
            REST_SIZE + KOLOM_SIZE * 4,
            REST_SIZE * 3 + 9 * KOLOM_SIZE,
            20,
            BLACK);
    }
    if (tekstfout != "")
    {
        DrawText(
            tekstfout.c_str(),
            REST_SIZE + KOLOM_SIZE * 4,
            REST_SIZE * 7 + 9 * KOLOM_SIZE,
            20,
            RED);
    }
    for (int i = 0; i < 4; ++i)
    {
        if (ABS(ogen[i]) > 0)
        {
            DrawTexture(
                img->GetImage(ABS(ogen[i])),
                REST_SIZE + KOLOM_SIZE * i,
                SCREEN_HEIGHT_SIZE - KOLOM_SIZE - REST_SIZE,
                WHITE);
        }
    }
}

/// @brief Draw a stone
/// @param x center position x
/// @param y center position y
/// @param z distance between 2 stones
/// @param n number of stones
/// @param c1 Color stone
/// @param c2 Coler edge of the stone
void Game_t::DrawStone(int x, int y, int z, int n, Color c1, Color c2)
{
    int max = MIN(ABS(n), SHOW_STONE);

    for (int j = 0; j < max; ++j)
    {
        DrawCircle(
            x,
            y + z * j,
            diameter[0],
            c1);
        for (int i = 0; i < 3; ++i)
        {
            DrawCircleLines(
                x,
                y + z * j,
                diameter[i],
                c2);
        }
    }
    if (n > 0) // uitgenomen is -1 en dan is ns niet nodig
    {
        if ((c1.a == BLACK.a && c1.b == BLACK.b && c1.g == BLACK.g && c1.r == BLACK.r) ||
            (c1.a == WHITE.a && c1.b == WHITE.b && c1.g == WHITE.g && c1.r == WHITE.r))
        {
            std::string ns = std::to_string(n);
            DrawText(ns.c_str(), x, y + z * (max - 1), 20, c2);
        }
    }
}

/// @brief bepaal of stenen uitgenomen kunnen worden
/// @param speler Ai of BGSpeler
/// @return true or false
bool Game_t::MagUithalen(int speler)
{
    if (geslagen[speler] != 0)
    {
        // geslagen stenen moeten eerst weer in het spel worden gebracht
        return false;
    }
    int n = ABS(uitgenomen[speler]);
    for (int i = 1; i <= 6; ++i)
    {
        int k = TRL(i, speler);
        int s = SPL(board[k]);
        if (s == speler)
        {
            n += ABS(board[k]);
        }
    }
    return n == 15;
}

/// @brief Bepaal alle mogelijke zetten,
/// @param speler Ai of BGSpeler
void Game_t::BepaalZetten(int speler)
{
    if (opening && speler == Ai)
    {
        return;
    }
    zetten.clear();
    halfzetten.clear();
    BepaalStappen(speler, 0);
    if (speler == Ai && stappen[0].size() > 0)
    {
        BepaalVervolgStappen(speler);
    }
#ifndef NDEBUG
    if (speler == Ai)
    {
        std::cout << SpelerNaam[speler] << " kan " << zetten.size() << " zetten doen." << std::endl;
        int t = 0;
        for (auto zet : zetten)
        {
            ++t;
            std::cout << t << ") " << ZetToString(&zet) << std::endl;
        }
    }
#endif
}

/// @brief vul de stappen(list)[index]
/// @param speler Ai of BGSpeler
/// @param index 0..3
void Game_t::BepaalStappen(int speler, int index)
{
    stappen[index].clear();
    if (geslagen[speler] != 0)
    {
        BepaalStappenTeruggezet(speler, index);
    }
    else
    {
        if (MagUithalen(speler))
        {
            BepaalStappenUitgenomen(speler, index);
        }
        BepaalStappenVerplaatst(speler, index);
    }
#ifndef NDEBUG
    Zet zet = Zet();
    std::cout << SpelerNaam[speler] << " kan " << stappen[index].size() << " stappen[ " << index << "] doen." << std::endl;
    int t = 0;
    for (auto stap : stappen[index])
    {
        ++t;
        InitZet(&zet);
        AddStap(stap, &zet);
        std::cout << t << ") " << ZetToString(&zet) << std::endl;
    }
#endif
}

/// @brief vul de overige stappen(list) en combineer deze tot de zetten(list)
/// @param speler Ai of BGSpeler
void Game_t::BepaalVervolgStappen(int speler)
{
    Zet zet = Zet();
#ifndef NDEBUG
    std::string sf, st;
#endif

    BewaarOgen();

    if (dubbel)
    {
        for (auto stapA : stappen[0])
        {
            InitZet(&zet);
            AddStap(stapA, &zet);
            PasOgenAan(stapA.ogen);
#ifndef NDEBUG
            sf = PositieToString();
#endif
            DoeZet(speler, &zet);
            BepaalStappen(speler, 1);
            NeemZetTerug(speler, &zet);
            HerstelOgen();
#ifndef NDEBUG
            st = PositieToString();
            ASSERT(sf == st);
#endif
            if (stappen[1].size() > 0)
            {
                for (auto stapB : stappen[1])
                {
                    InitZet(&zet);
                    AddStap(stapA, &zet);
                    AddStap(stapB, &zet);
                    PasOgenAan(stapA.ogen);
                    PasOgenAan(stapB.ogen);
#ifndef NDEBUG
                    sf = PositieToString();
#endif
                    DoeZet(speler, &zet);
                    BepaalStappen(speler, 2);
                    NeemZetTerug(speler, &zet);
                    HerstelOgen();
#ifndef NDEBUG
                    st = PositieToString();
                    ASSERT(sf == st);
#endif
                    if (stappen[2].size() > 0)
                    {
                        for (auto stapC : stappen[2])
                        {
                            InitZet(&zet);
                            AddStap(stapA, &zet);
                            AddStap(stapB, &zet);
                            AddStap(stapC, &zet);
                            PasOgenAan(stapA.ogen);
                            PasOgenAan(stapB.ogen);
                            PasOgenAan(stapC.ogen);
#ifndef NDEBUG
                            sf = PositieToString();
#endif
                            DoeZet(speler, &zet);
                            BepaalStappen(speler, 3);
                            NeemZetTerug(speler, &zet);
                            HerstelOgen();
#ifndef NDEBUG
                            st = PositieToString();
                            ASSERT(sf == st);
#endif
                            if (stappen[3].size() > 0)
                            {
                                for (auto stapD : stappen[3])
                                {
                                    InitZet(&zet);
                                    AddStap(stapA, &zet);
                                    AddStap(stapB, &zet);
                                    AddStap(stapC, &zet);
                                    AddStap(stapD, &zet);
#ifndef NDEBUG
                                    sf = PositieToString();
#endif
                                    DoeZet(speler, &zet); // voor de score
                                    NeemZetTerug(speler, &zet);
                                    HerstelOgen();
#ifndef NDEBUG
                                    st = PositieToString();
                                    ASSERT(sf == st);
#endif
                                    zetten.push_back(zet);
                                }
                            }
                            else
                            {
                                halfzetten.push_back(zet);
                            }
                        }
                    }
                    else
                    {
                        halfzetten.push_back(zet);
                    }
                }
            }
            else
            {
                halfzetten.push_back(zet);
            }
        }
    }
    else
    {
        for (auto stapA : stappen[0])
        {
            InitZet(&zet);
            AddStap(stapA, &zet);
            PasOgenAan(stapA.ogen);
#ifndef NDEBUG
            sf = PositieToString();
#endif
            DoeZet(speler, &zet);
            BepaalStappen(speler, 1);
            NeemZetTerug(speler, &zet);
            HerstelOgen();
#ifndef NDEBUG
            st = PositieToString();
            ASSERT(sf == st);
#endif
            if (stappen[1].size() > 0)
            {
                for (auto stapB : stappen[1])
                {
                    InitZet(&zet);
                    AddStap(stapA, &zet);
                    AddStap(stapB, &zet);
                    // omdat dit de tweede stap is hoeven de ogen niet meer worden aangepast en later teruggezet
#ifndef NDEBUG
                    sf = PositieToString();
#endif
                    DoeZet(speler, &zet); // voor de score
                    NeemZetTerug(speler, &zet);
#ifndef NDEBUG
                    st = PositieToString();
                    ASSERT(sf == st);
#endif
                    zetten.push_back(zet);
                }
            }
            else
            {
                if (stapA.ogen == ogen[0])
                {
                    // alleen de hoogste dobbelsteen vastleggen
                    halfzetten.push_back(zet);
                }
            }
        }
    }
    // gewone zetten niet gevonden dan de list vullen met half afgeronde zetten als deze er zijn
    if (zetten.size() == 0)
    {
        if (halfzetten.size() > 0)
        {
            for (auto z : halfzetten)
            {
                zetten.push_back(z);
            }
        }
    }
}

/// @brief Bepaal de eerste stappen van een verplaatsing en plaats deze in een stappenlist[index]
/// @param speler Ai of Speler
void Game_t::BepaalStappenVerplaatst(int speler, int index)
{
    int opp = Niemand, van = 0, van2 = 0, naar = 0, naar2 = 0;
    Stap stap;
    Zet zet = Zet();
    int dobbel = TelDobbelstenen();

    if (dobbel == 0)
    {
        // geen zet mogelijk;
        return;
    }

    opp = 3 - speler; // oponent
    for (int i = 1; i <= 24; ++i)
    {
        int k = TRL(i, speler); // k = kegel i voor de Speler, 25 - i voor de Ai
        int sv = SPL(board[k]);
        if (sv == speler)
        {
            van = k;
            if (dubbel)
            {
                naar = van - ogen[0];
                if (ONB(naar))
                {
                    int sn = SPL(board[naar]);
                    int spl = KanSlaan(naar, opp) ? opp : Niemand;
                    if (sn == Niemand || sn == speler || spl == opp)
                    {
                        InitZet(&zet);
                        MaakStap(van, naar, spl, ogen[0], &zet);
                        stappen[index].push_back(zet.stappen[0]);
                    }
                }
            }
            else
            {
                for (int j = 0; j < dobbel; ++j)
                {
                    naar = van - ogen[j];
                    if (ONB(naar))
                    {
                        int sn = SPL(board[naar]);
                        int spl = KanSlaan(naar, opp) ? opp : Niemand;
                        if (sn == Niemand || sn == speler || spl == opp)
                        {
                            InitZet(&zet);
                            MaakStap(van, naar, spl, ogen[j], &zet);
                            stappen[index].push_back(zet.stappen[0]);
                        }
                    }
                }
            }
        }
    }
}

/// @brief Bepaal de eerste stappen van een terugzetting en plaats deze in een stappenlist[index]
/// @param speler Ai of BGSpeler
/// @param index 0..3
void Game_t::BepaalStappenTeruggezet(int speler, int index)
{
    int opp = Niemand, n = 0;
    Zet zet = Zet();
    int dobbel = TelDobbelstenen();

    if (dobbel == 0)
    {
        // geen zet mogelijk;
        return;
    }

    opp = 3 - speler; // oponent
    if (dubbel)
    {
        int o = 25 - ABS(ogen[0]);
        int k = TRL(o, speler);
        int s = SPL(board[k]);
        int spl = KanSlaan(k, opp) ? opp : Niemand;
        if (s == Niemand || s == speler || spl == opp)
        {
            InitZet(&zet);
            MaakStap(0, k, spl, ogen[0], &zet);
            stappen[index].push_back(zet.stappen[0]);
        }
    }
    else
    {
        for (int i = 0; i < dobbel; ++i)
        {
            int o = 25 - ABS(ogen[i]);
            int k = TRL(o, speler);
            int s = SPL(board[k]);
            int spl = KanSlaan(k, opp) ? opp : Niemand;
            if (s == Niemand || s == speler || spl == opp)
            {
                InitZet(&zet);
                MaakStap(0, k, spl, ogen[i], &zet);
                stappen[index].push_back(zet.stappen[0]);
            }
        }
    }
}

/// @brief Bepaal de eerste stappen van een uitneming en plaats deze in een stappenlist[index]
/// @param speler Ai of BGSpeler
/// @param index 0..3
void Game_t::BepaalStappenUitgenomen(int speler, int index)
{
    Zet zet = Zet();
    int dobbel = TelDobbelstenen();
    int maxkegel = 0, minkegel = 0;

    if (dobbel == 0)
    {
        // geen zet mogelijk;
        return;
    }

    // BGSpeler vul maxkegel, Ai vul minkegel
    for (int i = 6; i >= 1; --i)
    {
        int s = SPL(board[i]);
        if (s == BGSpeler && maxkegel == 0)
        {
            maxkegel = i;
        }
        int k = TRL(i, Ai);
        s = SPL(board[k]);
        if (s == Ai && minkegel == 0)
        {
            minkegel = k;
        }
    }

    for (int i = 0; i < dobbel; ++i)
    {
        int k = TRL(ABS(ogen[i]), speler);
        int s = SPL(board[k]);
        if (s == speler)
        {
            InitZet(&zet);
            MaakStap(k, 0, Niemand, ogen[i], &zet);
            stappen[index].push_back(zet.stappen[0]);
        }
        else
        {
            if (speler == BGSpeler)
            {
                if (k > maxkegel)
                {
                    InitZet(&zet);
                    MaakStap(maxkegel, 0, Niemand, ogen[i], &zet);
                    stappen[index].push_back(zet.stappen[0]);
                }
            }
            else
            {
                if (k < minkegel)
                {
                    InitZet(&zet);
                    MaakStap(minkegel, 0, Niemand, ogen[i], &zet);
                    stappen[index].push_back(zet.stappen[0]);
                }
            }
        }
    }
}

/// @brief Tel de dobbelstenen die gebruikt kunnen worden
/// @return aantal
int Game_t::TelDobbelstenen()
{
    int dobbel = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (ogen[i] != 0)
        {
            ++dobbel;
        }
    }
    return dobbel;
}

/// @brief Kan de steen op positie k in de kegel worden geslagen?
/// @param k absolute positie
/// @param opp tegenstander
/// @return true of false
bool Game_t::KanSlaan(int k, int opp)
{
    return ABS(board[k]) == 1 && SPL(board[k]) == opp;
}

/// @brief Voeg een Stap toe aan een Zet
/// @param van Kegel van, indien 0 dan komt de steen vanuit de geslagen stapel
/// @param naar Kegel naar, indien 0 dan is de steen uitgenomen
/// @param geslagen Steen die eventueel geslagen wordt
/// @param zet Pointer naar Zet
void Game_t::MaakStap(int van, int naar, int geslagen, int ogen, Zet *zet)
{
    Stap s;

    s.kegelvan = van;
    s.kegelnaar = naar;
    s.geslagen = geslagen;
    s.ogen = ogen;

    AddStap(s, zet);
}

/// @brief Voeg een stap toe aan de zet
/// @param stap een berekende stap
/// @param zet Pointer naar Zet
void Game_t::AddStap(Stap stap, Zet *zet)
{
    zet->stappen[zet->stapteller] = stap;
    ++zet->stapteller;
}

/// @brief Voer de zet uit
/// @param speler Ai of Speler
/// @parem cz Pointer naar beste zet of zoek zet
void Game_t::DoeZet(int speler, Zet *zet)
{
    int steen, slagsteen;
    int opp = 3 - speler;

    for (int i = 0; i < zet->stapteller; ++i)
    {
        Stap stap = zet->stappen[i];
        if (stap.kegelnaar == 0)
        {
            // uit het spel genomen
            board[stap.kegelvan] -= steenwaarde[speler];
            uitgenomen[speler] += steenwaarde[speler];
        }
        else if (stap.kegelvan == 0)
        {
            // geslagen steen teruggezet
            geslagen[speler] -= steenwaarde[speler];
            if (stap.geslagen == opp)
            {
                geslagen[opp] += steenwaarde[opp];
                board[stap.kegelnaar] -= steenwaarde[opp];
            }
            board[stap.kegelnaar] += steenwaarde[speler];
        }
        else
        {
            // gewone zet
            board[stap.kegelvan] -= steenwaarde[speler];
            if (stap.geslagen == opp)
            {
                geslagen[opp] += steenwaarde[opp];
                board[stap.kegelnaar] -= steenwaarde[opp];
            }
            board[stap.kegelnaar] += steenwaarde[speler];
        }
    }
    if (speler == Ai)
    {
        zet->score = BepaalScore(zet);
    }
}

/// @brief Maak de uitgevoerde zet weer ongedaan
/// @param speler Ai of BGSpeler
/// @param zet pointer naar Zet
void Game_t::NeemZetTerug(int speler, Zet *zet)
{
    int steen, slagsteen;
    int opp = 3 - speler;

    for (int i = zet->stapteller - 1; i >= 0; --i)
    {
        Stap stap = zet->stappen[i];
        if (stap.kegelnaar == 0)
        {
            // uit het spel genomen teruggedraaid
            board[stap.kegelvan] += steenwaarde[speler];
            uitgenomen[speler] -= steenwaarde[speler];
        }
        else if (stap.kegelvan == 0)
        {
            // geslagen steen teruggezet teruggedraaid
            geslagen[speler] += steenwaarde[speler];
            if (stap.geslagen == opp)
            {
                geslagen[opp] -= steenwaarde[opp];
                board[stap.kegelnaar] += steenwaarde[opp];
            }
            board[stap.kegelnaar] -= steenwaarde[speler];
        }
        else
        {
            // gewone zet teruggedraaid
            board[stap.kegelvan] += steenwaarde[speler];
            if (stap.geslagen == opp)
            {
                geslagen[opp] -= steenwaarde[opp];
                board[stap.kegelnaar] += steenwaarde[opp];
            }
            board[stap.kegelnaar] -= steenwaarde[speler];
        }
    }
}

/// @brief Zoek de beste zet voor de Ai
/// @param speler Ai of Speler
void Game_t::BesteZet(int speler)
{
    bestescore = 999999;
    int dobbel = 0;
    int size = zetten.size();
    int sn;
    Zet zet;

    if (opening)
    {
        InitZet(&zet);

        if (ogen[0] == -1 && ogen[1] == -1)
        {
            sn = (KanSlaan(25 - 5, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 6, 25 - 5, sn, -1, &zet);
            MaakStap(25 - 6, 25 - 5, Niemand, -1, &zet);
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 23, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 23, sn, -1, &zet);
                MaakStap(25 - 24, 25 - 23, Niemand, -1, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 7, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 8, 25 - 7, sn, -1, &zet);
                MaakStap(25 - 8, 25 - 7, Niemand, -1, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -2 && ogen[1] == -1)
        {
            sn = (KanSlaan(25 - 11, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 13, 25 - 11, sn, -2, &zet);
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 23, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 23, sn, -1, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 5, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 6, 25 - 5, sn, -1, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -3 && ogen[1] == -1)
        {
            sn = (KanSlaan(25 - 5, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 8, 25 - 5, sn, -3, &zet);
            MaakStap(25 - 6, 25 - 5, Niemand, -1, &zet);
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -4 && ogen[1] == -1)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
                sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 9, 25 - 8, sn, -1, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
                sn = (KanSlaan(25 - 23, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 23, sn, -1, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -5 && ogen[1] == -1)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 8, sn, -5, &zet);
                sn = (KanSlaan(25 - 5, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 6, 25 - 5, sn, -1, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 8, sn, -5, &zet);
                sn = (KanSlaan(25 - 23, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 23, sn, -1, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -6 && ogen[1] == -1)
        {
            sn = (KanSlaan(25 - 7, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 13, 25 - 7, sn, -6, &zet);
            MaakStap(25 - 8, 25 - 7, Niemand, -1, &zet);
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -2 && ogen[1] == -2)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 11, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 11, sn, -2, &zet);
                MaakStap(25 - 13, 25 - 11, Niemand, -2, &zet);
                sn = (KanSlaan(25 - 22, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 22, sn, -2, &zet);
                MaakStap(25 - 24, 25 - 22, Niemand, -2, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 4, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 6, 25 - 4, sn, -2, &zet);
                MaakStap(25 - 6, 25 - 4, Niemand, -2, &zet);
                MaakStap(25 - 6, 25 - 4, Niemand, -2, &zet);
                sn = (KanSlaan(25 - 6, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 8, 25 - 6, sn, -2, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -3 && ogen[1] == -2)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 10, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 10, sn, -3, &zet);
                sn = (KanSlaan(25 - 11, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 11, sn, -2, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 11, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 11, sn, -2, &zet);
                sn = (KanSlaan(25 - 21, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 21, sn, -3, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -4 && ogen[1] == -2)
        {
            sn = (KanSlaan(25 - 4, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 8, 25 - 4, sn, -4, &zet);
            MaakStap(25 - 6, 25 - 4, Niemand, -2, &zet);
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -5 && ogen[1] == -2)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 8, sn, -5, &zet);
                sn = (KanSlaan(25 - 11, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 11, sn, -2, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 8, sn, -5, &zet);
                sn = (KanSlaan(25 - 22, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 22, sn, -2, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -6 && ogen[1] == -2)
        {
            sn = (KanSlaan(25 - 11, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 13, 25 - 11, sn, -2, &zet);
            sn = (KanSlaan(25 - 18, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 24, 25 - 18, sn, -6, &zet);
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -3 && ogen[1] == -3)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 10, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 10, sn, -3, &zet);
                MaakStap(25 - 13, 25 - 10, Niemand, -3, &zet);
                sn = (KanSlaan(25 - 21, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 21, sn, -3, &zet);
                MaakStap(25 - 24, 25 - 21, Niemand, -3, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 10, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 10, sn, -3, &zet);
                MaakStap(25 - 13, 25 - 10, Niemand, -3, &zet);
                sn = (KanSlaan(25 - 7, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 10, 25 - 7, sn, -3, &zet);
                MaakStap(25 - 10, 25 - 7, Niemand, -3, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -4 && ogen[1] == -3)
        {
            int x = GetRandomValue(1, 3);
            if (x == 3)
            {
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
                sn = (KanSlaan(25 - 21, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 21, sn, -3, &zet);
            }
            else if (x == 2)
            {
                sn = (KanSlaan(25 - 10, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 10, sn, -3, &zet);
                sn = (KanSlaan(25 - 20, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 20, sn, -4, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 10, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 10, sn, -3, &zet);
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -5 && ogen[1] == -3)
        {
            sn = (KanSlaan(25 - 3, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 8, 25 - 3, sn, -5, &zet);
            MaakStap(25 - 6, 25 - 3, Niemand, -3, &zet);
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -6 && ogen[1] == -3)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 18, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 18, sn, -6, &zet);
                sn = (KanSlaan(25 - 15, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 18, 25 - 15, sn, -3, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 18, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 18, sn, -6, &zet);
                sn = (KanSlaan(25 - 10, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 10, sn, -3, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -4 && ogen[1] == -4)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
                MaakStap(25 - 13, 25 - 9, Niemand, -4, &zet);
                sn = (KanSlaan(25 - 20, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 20, sn, -4, &zet);
                MaakStap(25 - 24, 25 - 20, Niemand, -4, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
                MaakStap(25 - 13, 25 - Niemand, sn, -4, &zet);
                sn = (KanSlaan(25 - 5, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 9, 25 - 5, sn, -4, &zet);
                MaakStap(25 - 9, 25 - 5, Niemand, -4, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -5 && ogen[1] == -4)
        {
            if (GetRandomValue(1, 2) == 2)
            {
                sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 8, sn, -5, &zet);
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 8, sn, -5, &zet);
                sn = (KanSlaan(25 - 20, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 20, sn, -4, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -6 && ogen[1] == -4)
        {
            int x = GetRandomValue(1, 3);
            if (x == 3)
            {
                sn = (KanSlaan(25 - 18, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 18, sn, -6, &zet);
                sn = (KanSlaan(25 - 9, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 13, 25 - 9, sn, -4, &zet);
            }
            else if (x == 2)
            {
                sn = (KanSlaan(25 - 18, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 24, 25 - 18, sn, -6, &zet);
                sn = (KanSlaan(25 - 14, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 18, 25 - 14, sn, -4, &zet);
            }
            else
            {
                sn = (KanSlaan(25 - 2, BGSpeler)) ? BGSpeler : Niemand;
                MaakStap(25 - 8, 25 - 2, sn, -6, &zet);
                MaakStap(25 - 6, 25 - 2, Niemand, -4, &zet);
            }
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -5 && ogen[1] == -5)
        {
            sn = (KanSlaan(25 - 8, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 13, 25 - 8, sn, -5, &zet);
            MaakStap(25 - 13, 25 - 8, Niemand, -5, &zet);
            sn = (KanSlaan(25 - 3, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 8, 25 - 3, sn, -5, &zet);
            MaakStap(25 - 8, 25 - 3, Niemand, -5, &zet);
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -6 && ogen[1] == -5)
        {
            sn = (KanSlaan(25 - 18, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 24, 25 - 18, sn, -6, &zet);
            sn = (KanSlaan(25 - 13, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 18, 25 - 13, sn, -5, &zet);
            bestezet = zet;
            return;
        }
        else if (ogen[0] == -6 && ogen[1] == -6)
        {
            sn = (KanSlaan(25 - 7, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 13, 25 - 7, sn, -6, &zet);
            MaakStap(25 - 13, 25 - 7, Niemand, -6, &zet);
            sn = (KanSlaan(25 - 18, BGSpeler)) ? BGSpeler : Niemand;
            MaakStap(25 - 24, 25 - 18, sn, -6, &zet);
            MaakStap(25 - 24, 25 - 18, Niemand, -6, &zet);
            bestezet = zet;
            return;
        }
    }

    // zoek beste zet
    for (auto zet : zetten)
    {
        if (zet.score < bestescore || zet.score == bestescore && GetRandomValue(1, 10) < 6)
        {
            bestezet = zet;
            bestescore = zet.score;
        }
    }
}

/// @brief Bepaal de score van de uitgevoerde zet (hoe lager hoe beter)
/// @param cz De zoek zet die onderzocht wordt
/// @return Score
int Game_t::BepaalScore(Zet *zet)
{
    int n, score = 0, pips_ai = 0, pips_spl = 0, min_speler = 1;

    if (geslagen[BGSpeler] == 0)
    {
        for (int i = 1; i <= 24; ++i)
        {
            int k = TRL(i, Ai);
            int s = SPL(board[k]);
            if (s == BGSpeler)
            {
                min_speler = k;
                break;
            }
        }
    }

    for (int i = 1; i <= 24; ++i)
    {
        int k = TRL(i, Ai);
        int s = SPL(board[k]);
        if (s == Ai)
        {
            n = ABS(board[k]);
            pips_ai += i * n;
            // correctie voor een enkele steen
            if (n == 1 && k < min_speler)
            {
                score += SINGLE_STONE;
            }
            if (k >= 1 && k <= 6)
            {
                score += THUIS_VAK_BGS * n;
            }
        }
        else if (s == BGSpeler)
        {
            n = board[k];
            pips_spl += i * n;
        }
    }

    score += pips_ai;

    // correcte voor een geslagen stenen van de speler
    for (int i = 0; i < zet->stapteller; ++i)
    {
        Stap stap = zet->stappen[i];
        if (stap.geslagen == BGSpeler)
        {
            score -= SINGLE_STONE;
        }
        if (stap.kegelnaar == 0)
        {
            score -= MAG_UITHALEN_CORRECTIE;
        }
    }

#ifndef NDEBUG
    std::cout << "pips Ai: " << pips_ai << " pips speler: " << pips_spl << " score Ai: " << score << " bestescore: " << bestescore << ZetToString(zet) << std::endl;
#endif
    return score;
}

/// @brief Translate zet naar string
/// @param cz Pointer to Zet
/// @return string
std::string Game_t::ZetToString(Zet *zet)
{
    std::string str = "";

    if (zet->stapteller > 1)
    {
        str = "( ";
    }
    for (int i = 0; i < zet->stapteller; ++i)
    {
        if (zet->stappen[i].kegelvan == 0)
        {
            str += "ges-" + std::to_string(zet->stappen[i].kegelnaar) + " ";
        }
        else if (zet->stappen[i].kegelnaar == 0)
        {
            str += std::to_string(zet->stappen[i].kegelvan) + "-uit ";
        }
        else
        {
            if (zet->stappen[i].geslagen != Niemand)
            {
                str += std::to_string(zet->stappen[i].kegelvan) + "x" + std::to_string(zet->stappen[i].kegelnaar) + " ";
            }
            else
            {
                str += std::to_string(zet->stappen[i].kegelvan) + "-" + std::to_string(zet->stappen[i].kegelnaar) + " ";
            }
        }
    }
    if (zet->stapteller > 1)
    {
        str += ")";
    }
    return str;
}

/// @brief Vertaal de huidige positie naar een string
/// @return positie als string
std::string Game_t::PositieToString()
{
    std::string str = "";

    for (int i = 1; i <= 24; ++i)
    {
        if (board[i] == 0)
        {
            str += "0";
        }
        else
        {
            int n = ABS(board[i]);
            int s = SPL(board[i]);
            if (s == BGSpeler)
            {
                str += "S";
            }
            else
            {
                str += "A";
            }
            str += std::to_string(n);
        }
    }
    str += "|";
    if (geslagen[BGSpeler] == 0)
    {
        str += "GS0";
    }
    else
    {
        str += "GS";
        str += std::to_string(geslagen[BGSpeler]);
    }
    if (geslagen[Ai] == 0)
    {
        str += "GA0";
    }
    else
    {
        str += "GA";
        str += std::to_string(ABS(geslagen[Ai]));
    }
    str += "|";
    if (uitgenomen[BGSpeler] == 0)
    {
        str += "US0";
    }
    else
    {
        str += "US";
        str += std::to_string(uitgenomen[BGSpeler]);
    }
    if (uitgenomen[Ai] == 0)
    {
        str += "UA0";
    }
    else
    {
        str += "UA";
        str += std::to_string(ABS(uitgenomen[Ai]));
    }
#ifndef NDEBUG
    std::cout << "Positie: " << str << std::endl;
#endif
    return str;
}

/// @brief Zet de vlaggen voor de kegel naar
void Game_t::Zetkegelnaar()
{
    InitVlagNaar();
    if (geslagen[BGSpeler] > 0)
    {
        for (auto stap : stappen[0])
        {
            if (stap.kegelvan == 0 && stap.kegelnaar >= 19 && stap.kegelnaar <= 24)
            {
                kegelnaar[stap.kegelnaar] = true;
            }
        }
    }
    else
    {
        // gewone zet
        for (auto stap : stappen[0])
        {
            if (stap.kegelvan == kegel_sel_from)
            {
                kegelnaar[stap.kegelnaar] = true;
            }
        }
    }
}

/// @brief Vul de vlaggen voor de kegel van
void Game_t::Zetkegelvan()
{
    InitVlagVan();
    if (geslagen[BGSpeler] > 0)
    {
        kegelvan[0] = true;
    }
    else
    {
        // gewone zet
        for (auto stap : stappen[0])
        {
            kegelvan[stap.kegelvan] = true;
        }
    }
}

// EOF