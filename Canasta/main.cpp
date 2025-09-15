#include "raylib.h"
#include <iostream>
#include <string>
#include "images.h"
#include "stapel.h"
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
const std::string version = "1.01";

// sizes

const int SQUARE_SIZE = 90;
const int SQUARE_HALF_SIZE = 45;
const int CARD_WIDTH_SIZE = 72;
const int CARD_HALF_WIDTH_SIZE = 36;
const int CARD_HEIGHT_SIZE = 88;
const int CARD_HALF_HEIGHT_SIZE = 44;
const int VERTICALLINE_SIZE = 19;
const int CARDREST_SIZE = 9;
const int SCREEN_WIDTH_SIZE = SQUARE_SIZE * 17 + CARDREST_SIZE * 2;
const int SCREEN_HEIGHT_SIZE = SQUARE_SIZE * 10 + CARDREST_SIZE * 2;

// enums
enum eTeam
{
    Wij,
    Zij,
};

enum eSpeler
{
    Noord,
    Oost,
    Zuid,
    West,
};

enum eStatuscode
{
    Spelstart,
    Spelspeel,
    Schudden,
    Delen,
    Rondestart,
    Rondespeel,
    Spelerstart,
    Spelerspeel,
    Spelertrekkaart,
    Speleruitleg,
    Spelerwerp,
    Spelerstop,
    Rondestop,
    Spelstop,
};

enum eKaart
{
    Ba,
    Zj,
    Rj,
    K2,
    H2,
    S2,
    R2,
    H3,
    R3,
    Ka,
    Ha,
    Sa,
    Ra,
    Kh,
    Hh,
    Sh,
    Rh,
    Kv,
    Hv,
    Sv,
    Rv,
    Kb,
    Hb,
    Sb,
    Rb,
    Kt,
    Ht,
    St,
    Rt,
    K9,
    H9,
    S9,
    R9,
    K8,
    H8,
    S8,
    R8,
    K7,
    H7,
    S7,
    R7,
    K6,
    H6,
    S6,
    R6,
    K5,
    H5,
    S5,
    R5,
    K4,
    H4,
    S4,
    R4,
    K3,
    S3,
};

enum eKaartKleuren
{
    Klaveren,
    Harten,
    Schoppen,
    Ruiten,
    Joker,
};

enum eKaartTypen
{
    Rode3,
    Aas,
    Heer,
    Vrouw,
    Boer,
    Tien,
    Negen,
    Acht,
    Zeven,
    Zes,
    Vijf,
    Vier,
    Zwarte3,
    Kl_Joker,
    Gr_Joker,
};

enum eCanasta
{
    Geen,
    Onecht,
    Echt,
};

enum eAflegActie
{
    Geen_actie,
    Twee_kaart_uitleg,
    Plus_joker_uitleg,
    Kaart_aanleg,
};

// tabellen

const int Teamid[] = {
    Wij,
    Zij,
    Wij,
    Zij,
};

const int Vriendid[] = {
    Zuid,
    West,
    Noord,
    Oost,
};

const int Puntenkaart[] = {
    -1,             // Achtergrond
    50, 50,         // Gr_Joker
    20, 20, 20, 20, // Kl_Joker
    100, 100,       // Rode3
    20, 20, 20, 20, // Aas
    10, 10, 10, 10, // Heer
    10, 10, 10, 10, // Vrouw
    10, 10, 10, 10, // Boer
    10, 10, 10, 10, // Tienen
    10, 10, 10, 10, // Negen
    10, 10, 10, 10, // Acht
    5, 5, 5, 5,     // Zeven
    5, 5, 5, 5,     // Zes
    5, 5, 5, 5,     // Vijf
    5, 5, 5, 5,     // Vier
    5, 5            // Zwarte3
};

const int Uitlegkaart[] = {
    -1,                                     // Achtergrond
    Gr_Joker, Gr_Joker,                     // Gr_Joker
    Kl_Joker, Kl_Joker, Kl_Joker, Kl_Joker, // Kl_Joker
    Rode3, Rode3,                           // Rode3
    Aas, Aas, Aas, Aas,                     // Aas
    Heer, Heer, Heer, Heer,                 // Heer
    Vrouw, Vrouw, Vrouw, Vrouw,             // Vrouw
    Boer, Boer, Boer, Boer,                 // Boer
    Tien, Tien, Tien, Tien,                 // Tien
    Negen, Negen, Negen, Negen,             // Negen
    Acht, Acht, Acht, Acht,                 // Acht
    Zeven, Zeven, Zeven, Zeven,             // Zeven
    Zes, Zes, Zes, Zes,                     // Zes,
    Vijf, Vijf, Vijf, Vijf,                 // Vijf
    Vier, Vier, Vier, Vier,                 // Vier
    Zwarte3, Zwarte3                        // Zwarte3
};

const int KleurKaart[] = {
    -1,                                 // Achtergrond
    Joker, Joker,                       // Gr_Joker
    Klaveren, Harten, Schoppen, Ruiten, // Kl_Joker
    Harten, Ruiten,                     // Rode3
    Klaveren, Harten, Schoppen, Ruiten, // Aas
    Klaveren, Harten, Schoppen, Ruiten, // Heer
    Klaveren, Harten, Schoppen, Ruiten, // Vrouw
    Klaveren, Harten, Schoppen, Ruiten, // Boer
    Klaveren, Harten, Schoppen, Ruiten, // Tien
    Klaveren, Harten, Schoppen, Ruiten, // Negen
    Klaveren, Harten, Schoppen, Ruiten, // Acht
    Klaveren, Harten, Schoppen, Ruiten, // Zeven
    Klaveren, Harten, Schoppen, Ruiten, // Zes
    Klaveren, Harten, Schoppen, Ruiten, // Vijf
    Klaveren, Harten, Schoppen, Ruiten, // Vier
    Klaveren, Schoppen                  // Zwarte3
};

const int CanastaPunten[] = {
    0,
    300,
    500,
};

const std::string TekstUitleg[] = {
    "Canasta een kaartspel voor 4 personen (noord, oost, zuid en west) die twee teams vormen: wij (noord en zuid) en zij (oost en west).",
    "DOEL VAN HET SPEL",
    "Het doel van Canasta is om de meeste punten te scoren door setjes kaarten van dezelfde waarde te vormen en deze op tafel te spelen.",
    "Een canasta is een set van zeven kaarten van dezelfde waarde, en het spel draait om het maken van zoveel mogelijk punten door sets en canasta's",
    "te vormen. Het team dat als eerste een bepaald aantal punten bereikt (5000 punten) wint het spel.",
    "Er wordt gespeeld met 2 sets van 52 kaarten (104 kaarten) + 4 jokers, bij elke ronde krijgt een speler na het schudden 11 kaarten in de hand, ",
    "de rest van de kaarten wordt als trekstapel op tafel gelegd, de bovenste kaart wordt omgedraaid als aflegstapel naast de trekstapel op tafel ",
    "gelegd, is dit een joker, twee of rode drie dan wordt een volgende kaart omgedraaid net zolang dat het geen joker, twee of rode drie is.",
    "WAARDE VAN DE KAARTEN",
    "- Jokers: 50 punten(wild) - Tweeen: 20 punten(wild) ook wel kleine jokers genoemd - Azen: 20 punten - 8 t/m Koning: 10 punten - 4 t/m 7: 5 punten",
    "- Zwarte drieen 5 punten(kan niet worden gebruikt om sets te maken, enkel voor blokkades in de aflegstapel)",
    "SPECIALE KAARTEN",
    "- Jokers en Tweeen : Dit zijn de wilde kaarten in Canasta. Ze kunnen worden gebruikt om elke kaartwaarde aan te vullen en sets sneller te vormen.",
    "  Er moeten altijd meer kaarten dan wilde kaarten zijn in een set of canasta.",
    "- Rode drieen : Als je een rode drie trekt of ontvangt, moet je deze onmiddellijk op tafel leggen en een vervangende kaart trekken.",
    "  Rode drieen geven aan het einde van de ronde 100 punten als je team minstens een set heeft gemaakt (3 of meer kaarten) anders 100 strafpunten.",
    "- Zwarte drieen : Deze kaarten kunnen niet worden gebruikt om sets te maken, maar kunnen op de aflegstapel worden gelegd om te voorkomen dat",
    "  de tegenstander de stapel oppakt.",
    "SPELVERLOOP CANASTA",
    "Het spel bestaat uit beurten waarin de spelers proberen sets en canasta's te vormen om punten te scoren, elke beurt bestaat uit drie stappen : ",
    "- 1. Kaart trekken, de speler begint zijn of haar beurt door een kaart te trekken. De speler heeft twee opties:",
    "-- een kaart trekken van de trekstapel.",
    "-- de hele aflegstapel opnemen, als de aflegstapel niet geblokkeerd is met een wilde kaart dan mag het worden opgepakt als er op tafel al sets",
    "   liggen van die kaart, is de aftrekstapel geblokkeerd dan mag het alleen als er nieuwe set kan worden gemaakt zonder wilde kaarten.",
    "- 2. Sets vormen en neerleggen",
    " Het doel van Canasta is om setjes van minstens drie kaarten van dezelfde waarde te vormen en op tafel te leggen. De sets kunnen bestaan uit ",
    " gewone kaarten en wilde kaarten(jokers of tweeen), maar er zijn regels : ",
    " Zuivere set: Alleen kaarten van dezelfde waarde zonder wilde kaarten.",
    " Gemengde set: Een set waarin een of meer wilde kaarten zijn (er moeten altijd meer kaarten dan wilde kaarten zijn).",
    " Zuivere canasta: Zeven kaarten zonder jokers of tweeen Gemengde canasta : Zeven kaarten met een tot 3 jokers en of tweeen.",
    " De speler die als eerste een of meer sets uitlegt moet voldoen aan de minimale score: - 50 (0..1499) - 90 (1500..2999) 120 (3000..) punten.",
    "- 3. Kaart afleggen",
    " Aan het einde van de beurt moet de speler een kaart afleggen op de aflegstapel, tenzij hij/zij alle kaarten uitspeelt om uit te gaan.",
    " De bovenste kaart van de aflegstapel bepaalt wat de volgende speler kan doen.",
    "UITGAAN EN DE RONDE EINDIGEN",
    "Een ronde eindigt wanneer een speler alle kaarten uit zijn hand heeft gespeeld. Dit kan alleen als het team minstens een canasta heeft gemaakt.",
    "PUNTENTELLING CANASTA",
    "Na het einde van elke ronde worden de punten van alle kaarten geteld. De punten worden als volgt verdeeld :",
    "- Sets en canasta's: Het aantal punten van de kaarten in de sets wordt bij elkaar opgeteld.'",
    "- Zuivere canasta: 500 punten extra.",
    "- Gemengde canasta: 300 punten extra.",
    "- Rode 3: 100 punten per rode 3, 4 Rode drieen: 400 punten extra. (uitzondering: geen set gemaakt dan 100 strafpunten voor elke rode 3)",
    "- Uitgaan: Een team krijgt 100 extra punten als een speler alle kaarten uitspeelt.",
    "  Daarnaast worden alle kaarten die nog in de handen van de spelers zijn, als strafpunten afgetrokken van de totaalscore",
    "WINST VAN HET SPEL",
    "Het spel eindigt wanneer een team 5000 punten heeft behaald. Het team met de meeste punten wint het spel.",
    "Druk of click met de muis op ENTER",
};

const Color KleurUitleg[] = {
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
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    WHITE,
    YELLOW,
    WHITE,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    YELLOW,
    WHITE,
    YELLOW,
    GREEN,
};

const std::string KaartUitlegLetter[] = {
    "r3",
    "A",
    "K",
    "Q",
    "J",
    "10",
    "9",
    "8",
    "7",
    "6",
    "5",
    "4",
};

const std::string TeamNaam[] = {
    "Wij",
    "Zij",
};

const std::string SpelerNaam[] = {
    "Noord",
    "Oost",
    "Zuid",
    "West",
};

const std::string StatuscodeNaam[] = {
    "Spelstart",
    "Spelspeel",
    "Schudden",
    "Delen",
    "Rondestart",
    "Rondespeel",
    "Spelerstart",
    "Spelerspeel",
    "Spelertrekkaart",
    "Speleruitleg",
    "Spelerwerp",
    "Spelerstop",
    "Rondestop",
    "Spelstop",
};

const std::string KaartNaam[] = {
    "Ba",                   // Achtergrond
    "Jj", "Jj",             // Grote Joker
    "K2", "K2", "S2", "R2", // Kleine Joker
    "H3", "R3",             // Rode 3
    "Ka", "Ha", "Sa", "Ra", // Aas
    "Kh", "Hh", "Sh", "Rh", // Heer
    "Kv", "Hv", "Sv", "Rv", // Vrouw
    "Kb", "Hb", "Sb", "Rb", // Boer
    "Kt", "Ht", "St", "Rt", // Tien
    "K9", "H9", "S9", "R9", // Negen
    "K8", "H8", "S8", "R8", // Acht
    "K7", "H7", "S7", "R7", // Zeven
    "K6", "H6", "S6", "R6", // Zes
    "K5", "H5", "S5", "R5", // Vijf
    "K4", "H4", "S4", "R4", // Vier
    "K3", "s3",             // Zwarte 3
};

const std::string KaartKleurNaam[] = {
    "Klaveren",
    "Harten",
    "Schoppen",
    "Ruiten",
    "Joker",
};

const std::string KaartUitlegNaam[] = {
    "Rode3",
    "Aas",
    "Heer",
    "Vrouw",
    "Boer",
    "Tien",
    "Negen",
    "Acht",
    "Zeven",
    "Zes",
    "Vijf",
    "Vier",
    "Zwarte3",
    "Kl_Joker",
    "Gr_Joker",
};

const int ZwarteKaarten[] = {
    K3,
    Ka,
    Kh,
    Kv,
    Kb,
    Kt,
    K9,
    K8,
    K7,
    K6,
    K5,
    K4,
};

const int RodeKaarten[] = {
    H3,
    Ha,
    Hh,
    Hv,
    Hb,
    Ht,
    H9,
    H8,
    H7,
    H6,
    H5,
    H4,
};

// macros

#define ABS(k) ((k) < 0 ? -(k) : (k))
#define SEL(k) ((k) < 0) // -k =true, k = false
#define PUNTEN(k) (Puntenkaart[ABS(k)])
#define KLEUR(k) (KleurKaart[ABS(k)])
#define UITLEG(k) (Uitlegkaart[ABS(k)])
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

// types

const int speeltafel = 55;
const int dir = 56;
const int enterbutton = 57;

//************************************************************************************************
// main 
//************************************************************************************************

/// @brief main program
/// @return result
int main()
{
    std::string title = "Canasta in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
    // Initialization
    InitWindow(SCREEN_WIDTH_SIZE, SCREEN_HEIGHT_SIZE, title.c_str());
    Images_t *images = new Images_t();
    ASSERT(images != nullptr);
    Game_t *game = new Game_t();
    ASSERT(game != nullptr);
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
	        while(keybuf>0){
            	keybuf = GetKeyPressed();
        	}
			game->KeyPress(key);
		}
        // Mousepress
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int x = GetMouseX();
            int y = GetMouseY();
            game->MousePress(x, y);
        }
        // Actions
        game->Update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game->Draw(images);
        EndDrawing();
    }

    // De-Initialization
    delete game;
    delete images;
    CloseWindow(); // Close window and OpenGL context
    return 0;
}

//************************************************************************************************
// Images_t
//************************************************************************************************

/// @brief Constructor
Images_t::Images_t()
{
    image_container[speeltafel] = LoadTexture("./resources/speeltafel.png");
    image_container[speeltafel].width = SCREEN_WIDTH_SIZE;
    image_container[speeltafel].height = SCREEN_HEIGHT_SIZE;
    image_container[K2] = LoadTexture("./resources/k2.png");
    image_container[K3] = LoadTexture("./resources/k3.png");
    image_container[K4] = LoadTexture("./resources/k4.png");
    image_container[K5] = LoadTexture("./resources/k5.png");
    image_container[K6] = LoadTexture("./resources/k6.png");
    image_container[K7] = LoadTexture("./resources/k7.png");
    image_container[K8] = LoadTexture("./resources/k8.png");
    image_container[K9] = LoadTexture("./resources/k9.png");
    image_container[Kt] = LoadTexture("./resources/kt.png");
    image_container[Kb] = LoadTexture("./resources/kb.png");
    image_container[Kv] = LoadTexture("./resources/kv.png");
    image_container[Kh] = LoadTexture("./resources/kk.png");
    image_container[Ka] = LoadTexture("./resources/ka.png");
    image_container[H2] = LoadTexture("./resources/h2.png");
    image_container[H3] = LoadTexture("./resources/h3.png");
    image_container[H4] = LoadTexture("./resources/h4.png");
    image_container[H5] = LoadTexture("./resources/h5.png");
    image_container[H6] = LoadTexture("./resources/h6.png");
    image_container[H7] = LoadTexture("./resources/h7.png");
    image_container[H8] = LoadTexture("./resources/h8.png");
    image_container[H9] = LoadTexture("./resources/h9.png");
    image_container[Ht] = LoadTexture("./resources/ht.png");
    image_container[Hb] = LoadTexture("./resources/hb.png");
    image_container[Hv] = LoadTexture("./resources/hv.png");
    image_container[Hh] = LoadTexture("./resources/hk.png");
    image_container[Ha] = LoadTexture("./resources/ha.png");
    image_container[S2] = LoadTexture("./resources/s2.png");
    image_container[S3] = LoadTexture("./resources/s3.png");
    image_container[S4] = LoadTexture("./resources/s4.png");
    image_container[S5] = LoadTexture("./resources/s5.png");
    image_container[S6] = LoadTexture("./resources/s6.png");
    image_container[S7] = LoadTexture("./resources/s7.png");
    image_container[S8] = LoadTexture("./resources/s8.png");
    image_container[S9] = LoadTexture("./resources/s9.png");
    image_container[St] = LoadTexture("./resources/st.png");
    image_container[Sb] = LoadTexture("./resources/sb.png");
    image_container[Sv] = LoadTexture("./resources/sv.png");
    image_container[Sh] = LoadTexture("./resources/sk.png");
    image_container[Sa] = LoadTexture("./resources/sa.png");
    image_container[R2] = LoadTexture("./resources/r2.png");
    image_container[R3] = LoadTexture("./resources/r3.png");
    image_container[R4] = LoadTexture("./resources/r4.png");
    image_container[R5] = LoadTexture("./resources/r5.png");
    image_container[R6] = LoadTexture("./resources/r6.png");
    image_container[R7] = LoadTexture("./resources/r7.png");
    image_container[R8] = LoadTexture("./resources/r8.png");
    image_container[R9] = LoadTexture("./resources/r9.png");
    image_container[Rt] = LoadTexture("./resources/rt.png");
    image_container[Rb] = LoadTexture("./resources/rb.png");
    image_container[Rv] = LoadTexture("./resources/rv.png");
    image_container[Rh] = LoadTexture("./resources/rk.png");
    image_container[Ra] = LoadTexture("./resources/ra.png");
    image_container[Rj] = LoadTexture("./resources/rj.png");
    image_container[Zj] = LoadTexture("./resources/zj.png");
    image_container[Ba] = LoadTexture("./resources/ba.png");
    for (int i = 0; i < 55; ++i)
    {
        image_container[i].width = CARD_WIDTH_SIZE;
        image_container[i].height = CARD_HEIGHT_SIZE;
    }
    image_container[enterbutton] = LoadTexture("./resources/enterbutton.png");
    image_container[enterbutton].width = 2 * SQUARE_SIZE;
    image_container[enterbutton].height = SQUARE_SIZE;
    image_container[dir] = LoadTexture("./resources/dir.png");
    image_container[dir].width = SQUARE_SIZE;
    image_container[dir].height = SQUARE_SIZE;
}

/// @brief destructor
Images_t::~Images_t()
{
    for (int i = 0; i < 58; ++i)
    {
        UnloadTexture(image_container[i]);
    }
}

/// @brief Get an image conatiner
/// @param id number in the array
/// @return image container
Texture2D Images_t::GetImage(int id)
{
    ASSERT(id >= 0 && id <= 57);
    return image_container[id];
}

//************************************************************************************************
// Stapel_t
//************************************************************************************************

/// @brief Grow the capacity off the array by 2
void Stapel_t::growArray()
{
    int *temp = new int[capacity * 2];
    ASSERT(temp != nullptr);
    capacity = capacity * 2;
    for (int i = 0; i < size; ++i)
    {
        temp[i] = data[i];
    }
    delete[] data;
    data = temp;
}

/// @brief Default constructor with size 1
Stapel_t::Stapel_t()
{
    capacity = 1;
    size = 0;
    data = new int[capacity];
    ASSERT(data != nullptr);
}

/// @brief Constructor with size capacity
/// @param capacity default size
Stapel_t::Stapel_t(int capacity)
{
    this->capacity = capacity;
    size = 0;
    data = new int[capacity];
    ASSERT(data != nullptr);
}

/// @brief Destructor
Stapel_t::~Stapel_t()
{
    delete data;
    capacity = 0;
    size = 0;
}

/// @brief Get the size of the stapel
/// @return size
int Stapel_t::GetSize() { return size; }

/// @brief Is de stapel leeg?
/// @return true of false
bool Stapel_t::IsEmpty()
{
    return size == 0;
}

/// @brief Clear the stapel
void Stapel_t::Clear() { size = 0; }

/// @brief Get the container size from the stapel
/// @return container size
int Stapel_t::GetCapacity() { return capacity; }

/// @brief Get element on index in the stapel
/// @param index 0..size - 1
/// @return element or 0 => (index < 0 or index >= size)
int Stapel_t::GetElementAt(int index) { return index >= 0 && index < size ? data[index] : 0; }

/// @brief Set a new value to an element on index in the stapel
/// @param index 0..size - 1
/// @param value new value
void Stapel_t::SetElementAt(int index, int value)
{
    if (index >= 0 && index < size)
    {
        data[index] = value;
    }
}

/// @brief Add element to the stapel
/// @param value value of the element
void Stapel_t::PushBack(int value)
{
    if (size == capacity)
    {
        growArray();
    }
    data[size] = value;
    ++size;
}

/// @brief Get the last pushed value from the stapel without removing it
/// @return value or 0 => (size = 0)
int Stapel_t::Peek() { return size > 0 ? data[size - 1] : 0; }

/// @brief Get the last pushed value from the stapel and remove it
/// @return value or 0 => (size == 0)
int Stapel_t::PopBack()
{
    if (size > 0)
    {
        int value = Peek();
        data[size - 1] = 0;
        --size;
        return value;
    }
    return 0;
}

/// @brief Get the index of a value in the stapel
/// @param key sleutelwaarde
/// @return index or -1
int Stapel_t::Search(int key)
{
    for (int i = 0; i < size; ++i)
    {
        if (data[i] == key)
        {
            return i;
        }
    }
    return -1;
}

/// @brief Delete element on the index in the stapel
/// @param index 0..size-1
void Stapel_t::DeleteAt(int index)
{
    if (index >= 0 && index < size)
    {
        for (int i = index; i < size; ++i)
        {
            data[i] = data[i + 1];
        }
        data[size - 1] = 0;
        --size;
    }
}

/// @brief Shuffle the stapel
void Stapel_t::Shuffle()
{
    if (size <= 1)
    {
        return;
    }
    for (int i = 0; i < size; ++i)
    {
        int j = GetRandomValue(0, size - 1);
        int k = data[i];
        data[i] = data[j];
        data[j] = k;
    }
}

/// @brief Sort the stapel
void Stapel_t::Sort()
{
    if (size <= 1)
    {
        return;
    }
    for (int i = 0; i < size - 1; ++i)
    {
        int minidx = i;
        for (int j = i; j < size; ++j)
        {
            if (ABS(data[j]) < ABS(data[minidx]))
            {
                minidx = j;
            }
        }
        if (i != minidx)
        {
            int k = data[i];
            data[i] = data[minidx];
            data[minidx] = k;
        }
    }
}

/// @brief Tel de punten from the stapel
/// @return punten
int Stapel_t::PuntenStapel()
{
    int punten = 0;
    for (int i = 0; i < size; ++i)
    {
        int k = data[i];
        punten += PUNTEN(k);
    }
    return punten;
}

/// @brief Count the jokers in the stapel
/// @return aantal
int Stapel_t::JokersStapel()
{
    int aantal = 0;
    for (int i = 0; i < size; ++i)
    {
        int k = data[i];
        if (UITLEG(k) == Gr_Joker || UITLEG(k) == Kl_Joker)
        {
            ++aantal;
        }
    }
    return aantal;
}

/// @brief Tel de geselecteerde kaarten
/// @return aantal
int Stapel_t::SelectedStapel()
{
    int aantal = 0;
    for (int i = 0; i < size; ++i)
    {
        int k = data[i];
        if (SEL(k))
        {
            ++aantal;
        }
    }
    return aantal;
}

/// @brief Tel de geselcteerde jokers
/// @return aantal
int Stapel_t::JokersSelectedStapel()
{
    int aantal = 0;
    for (int i = 0; i < size; ++i)
    {
        int k = data[i];
        int u = UITLEG(k);
        if (SEL(k) && (u == Gr_Joker || u == Kl_Joker))
        {
            ++aantal;
        }
    }
    return aantal;
}

/// @brief Haal de select vlag van alle kaarten
void Stapel_t::UnselectAll()
{
    for (int i = 0; i < size; ++i)
    {
        int k = GetElementAt(i);
        if (SEL(k))
        {
            SetElementAt(i, ABS(k));
        }
    }
}

/// @brief Get the canasta version of the stapel
/// @return eCanasta
int Stapel_t::CanastaStapel()
{
    if (size >= 7)
    {
        return JokersStapel() > 0 ? Onecht : Echt;
    }
    return Geen;
}

/// @brief Is the stapel blocked
/// @return true or false
bool Stapel_t::IsBlocked()
{
    for (int i = 0; i < size; ++i)
    {
        int k = data[i];
        if (UITLEG(k) == Gr_Joker || UITLEG(k) == Kl_Joker || UITLEG(k) == Rode3)
        {
            return true;
        }
    }
    return false;
}

//************************************************************************************************
// Game_t
//************************************************************************************************

/// @brief Initialise the game
void Game_t::InitGame()
{
    for (int i = Noord; i <= West; ++i)
    {
        spelers[i]->Clear();
    }
    trekstapel->Clear();
    aflegstapel->Clear();
    for (int i = Rode3; i <= Gr_Joker; ++i)
    {
        kaarttypen[i]->Clear();
        if (i <= Vier)
        {
            team_uitgelegd[Wij][i]->Clear();
            team_uitgelegd[Zij][i]->Clear();
        }
    }
    spel_continue = false;
    team_winnaar = -1;
    totaal_punten[Wij] = 0;
    totaal_punten[Zij] = 0;
    tekstinfo = "Een nieuw spel wordt gestart";
    tekstfout = "";
    trek_kaart = -1;
    teller = 0;
}

/// @brief Initialize ronde
void Game_t::InitRonde()
{
    for (int i = Noord; i <= West; ++i)
    {
        spelers[i]->Clear();
    }
    trekstapel->Clear();
    aflegstapel->Clear();
    for (int i = Rode3; i <= Gr_Joker; ++i)
    {
        kaarttypen[i]->Clear();
        if (i <= Vier)
        {
            team_uitgelegd[Wij][i]->Clear();
            team_uitgelegd[Zij][i]->Clear();
        }
    }
    ++deler;
    if (deler > West)
    {
        deler = Noord;
    }
    speler_ad_beurt = deler;
    speler_winnaar = -1;
    ronde_continue = false;
    ronde_geteld = false;
    tekstinfo = "Een nieuwe ronde wordt gestart";
    tekstfout = "";
    trek_kaart = -1;
}

/// @brief Get minimum uitleg punten voor een team
/// @return 50, 90 or 120
int Game_t::GetMinimumPunten(int team)
{
    if (totaal_punten[team] >= 3000)
    {
        return 120;
    }
    else if (totaal_punten[team] >= 1500)
    {
        return 90;
    }
    return 50;
}

/// @brief Heeft het team een canasta uitgelegd of niet
/// @param team eTeam
/// @return true or false
bool Game_t::HeeftCanastaGelegd(int team)
{
    for (int i = Aas; i <= Vier; ++i) // rode 3 telt niet mee
    {
        if (team_uitgelegd[team][i]->CanastaStapel() > Geen)
        {
            return true;
        }
    }
    return false;
}

/// @brief Heeft het team een of meer sets uitgelegd
/// @param team eTeam
/// @return true or false
bool Game_t::HeeftUitgelegd(int team)
{
    for (int i = Aas; i <= Vier; ++i) // rode 3 telt niet mee
    {
        if (!team_uitgelegd[team][i]->IsEmpty())
        {
            return true;
        }
    }
    return false;
}

/// @brief Get the punten from the uitlegstapels
/// @param inclusief when true get bonus, when false not
/// @param team eTeam
/// @return punten
int Game_t::TeamUitlegPunten(bool inclusief, int team)
{
    int punten = 0;

    if (inclusief)
    {
        for (int s = 0; s < 5; ++s)
        {
            overzicht[team][s] = 0;
        }
    }

    for (int i = Rode3; i <= Vier; ++i)
    {
        int p = team_uitgelegd[team][i]->PuntenStapel();
        if (i == Rode3)
        {
            if (inclusief)
            {
                if (HeeftUitgelegd(team))
                {
                    punten += p;
                    overzicht[team][0] += p;
                    if (team_uitgelegd[team][Rode3]->GetSize() == 4)
                    {
                        punten += 400;
                        overzicht[team][1] += 400;
                    }
                }
                else
                {
                    punten -= p;
                    overzicht[team][0] -= p;
                }
            }
        }
        else
        {
            punten += p;
            if (inclusief)
            {
                overzicht[team][0] += p;
                int c = CanastaPunten[team_uitgelegd[team][i]->CanastaStapel()];
                punten += c;
                overzicht[team][2] += c;
            }
        }
    }
    if (inclusief && spelstatus >= Rondestop)
    {
        for (int s = Noord; s <= West; ++s)
        {
            int p = spelers[s]->PuntenStapel();
            if (Teamid[s] == team)
            {
                if (speler_winnaar == s)
                {
                    overzicht[team][3] = 100;
                    punten += 100;
                }
                punten -= p;
                overzicht[team][4] -= p;
            }
        }
    }
    return punten;
}

/// @brief Vul de trekstapel
void Game_t::VulDeck()
{
    for (int i = Zj; i <= S3; ++i)
    {
        trekstapel->PushBack(i);
        trekstapel->PushBack(i);
    }
    tekstinfo = SpelerNaam[deler] + " pakt de kaarten op ";
}

/// @brief Schud de trekstapel
void Game_t::SchudDeck()
{
    for (int n = 0; n < 3; ++n)
    {
        trekstapel->Shuffle();
    }
    tekstinfo = SpelerNaam[deler] + " schudt de kaarten ";
}

/// @brief  Verdeel de eerste kaarten
void Game_t::DeelDeck()
{
    int ontvanger = deler;
    for (int i = 0; i < 44; ++i) // 4 * 11
    {
        ++ontvanger;
        if (ontvanger > West)
        {
            ontvanger = Noord;
        }
        Trekkaart(ontvanger, false);
    }
    Trekkaart(4, false);
    for (int i = Noord; i <= West; ++i)
    {
        spelers[i]->Sort();
    }
    tekstinfo = SpelerNaam[deler] + " verdeelt de kaarten ";
}

/// @brief trek een kaart van de trekstapel
/// @param ontvanger eSpeler of aflegstapel ( == 4)
/// @param show show text or not
/// @return true is success, false = no more cards
bool Game_t::Trekkaart(int ontvanger, bool show)
{
    if (trekstapel->IsEmpty())
    {
        return false;
    } // trekkaarten zijn op
    int k = trekstapel->PopBack(), team;
    ASSERT(k > 0);
    if (ontvanger < 4)
    {
        team = Teamid[ontvanger];
        while (UITLEG(k) == Rode3)
        {
            team_uitgelegd[team][Rode3]->PushBack(k);
            // trekkaarten zijn op ?
            if (trekstapel->IsEmpty())
            {
                return false;
            }
            k = trekstapel->PopBack();
            ASSERT(k > 0);
        }
        if (show)
        {
            trek_kaart = ABS(k);
        }
        spelers[ontvanger]->PushBack(ABS(k));
    }
    else
    {
        while (UITLEG(k) == Rode3 || UITLEG(k) == Gr_Joker || UITLEG(k) == Kl_Joker)
        {
            aflegstapel->PushBack(k);
            // trekkaarten zijn op ?
            if (trekstapel->IsEmpty())
            {
                return false;
            }
            k = trekstapel->PopBack();
            ASSERT(k > 0);
        }
        aflegstapel->PushBack(k);
    }
    return true;
}

/// @brief Do de stappen voor de Ai
/// @param speler eSpeler, speler aan de beurt
void Game_t::Do_Ai_Move(int speler)
{
    int team = Teamid[speler_ad_beurt];
    if (spelstatus == Spelertrekkaart)
    {
        Do_Ai_Trekkaart(speler);
    }
    else if (spelstatus == Speleruitleg)
    {
        Do_Ai_Uitleggen(speler);
    }
    else if (spelstatus == Spelerwerp)
    {
        Do_Ai_Werpeenkaart(speler);        
    }
}

void Game_t::Do_Ai_Werpeenkaart(int speler)
{
    int team = Teamid[speler];
    Stapel_t *waarden = new Stapel_t(14);
    VulTypeKaarten(speler_ad_beurt);
    int jokers = kaarttypen[Kl_Joker]->GetSize() + kaarttypen[Gr_Joker]->GetSize();
    bool blocked = aflegstapel->IsBlocked();
    int k, u, val, size, osize;

    for (int i = 0; i < spelers[speler]->GetSize(); ++i)
    {
        k = spelers[speler_ad_beurt]->GetElementAt(i);
        u = UITLEG(k);
        val = PUNTEN(k); 
        if (u == Zwarte3 || u == Kl_Joker || u == Gr_Joker)
        {
            waarden->PushBack(val - 2); 
        }
        else
        {
            size = kaarttypen[u]->GetSize();
            osize = team_uitgelegd[team ^ 1][u]->GetSize();
            if (size > 3)
            {
                waarden->PushBack(val);
            }
            else if (size == 3)
            {
                waarden->PushBack(val * 3 + 20);
            }
            else if (size == 2)
            {
                if (jokers > 0)
                {
                    waarden->PushBack(val * 2 + 20);
                }
                else
                {
                    if (blocked)
                    {
                        waarden->PushBack(val - 1);
                    }
                    else 
                    {
                        if (osize > 0)
                        {
                            waarden->PushBack(val * osize);
                        }
                        else
                        {
                            waarden->PushBack(val);
                        }
                    }
                }
            }
            else if (size == 1)
            {
                if (blocked)
                {
                    waarden->PushBack(val - 1);
                }
                else if (osize > 0)
                {
                    waarden->PushBack(val * osize);
                }
                else
                {
                    waarden->PushBack(val);
                }
            }
        }
    }
    ASSERT(waarden->GetSize() == spelers[speler]->GetSize());
    int minidx = 0;
    int minval = waarden->GetElementAt(minidx);
    for (int i = 1; i < waarden->GetSize(); ++i)
    {
        val = waarden->GetElementAt(i);
        if (val < minval)
        {
            minval = val;
            minidx = i;
        }
        else if (val == minval)
        {
            if (GetRandomValue(0, 9) < 5)
            {
                minval = val;
                minidx = i;
            }
        }
    }
    k = spelers[speler]->GetElementAt(minidx);
    spelers[speler]->DeleteAt(minidx);
    aflegstapel->PushBack(ABS(k));
    if (spelers[speler]->IsEmpty())
    {
        speler_winnaar = speler;
        spelstatus = Rondestop;
    }
    else
    {
        spelstatus = Spelerstop;
    }
    delete waarden;
}

/// @brief Ai legt uit
/// @param speler eSpeler, speler aan de beurt
void Game_t::Do_Ai_Uitleggen(int speler)
{
    KaartenUitleggen(speler);
    if (spelers[speler]->IsEmpty())
    {
        speler_winnaar = speler_ad_beurt;
        spelstatus = Rondestop;
    }
    else
    {
        spelstatus = Spelerwerp;
    }
}

/// @brief Ai trekt een kaart
/// @param speler eSpeler, speler aan de beurt
void Game_t::Do_Ai_Trekkaart(int speler)
{
    tekstfout = "";
#ifdef NDEBUG
    bool show = false;
#else
    bool show = true;
#endif
    // aflegstapel oppakkem
    if (MagOppakken(speler))
    {
        if (WilOppakken(speler))
        {
            spelstatus = Speleruitleg;
            tekstinfo = SpelerNaam[speler] + " pakt de aflegstapel op";
            return;
        }
        else
        {
            afleg_kaart = 0;
            afleg_kaart_actie = Geen_actie;
            spelers[speler]->UnselectAll();
        }
    }
    // kaart trekken
    if (!Trekkaart(speler, show))
    {
        tekstfout = "de kaarten die opgepakt kunnen zijn op, einde ronde zonder winnaar!";
        spelstatus = Rondestop;
        return;
    };
    spelers[speler]->Sort();
    spelstatus = Speleruitleg;
}

/// @brief Mag een speler de aflegstapel oppakken?
/// @param speler eSpeler
/// @return true or false
bool Game_t::MagOppakken(int speler)
{
    int team = Teamid[speler];
    int k = aflegstapel->Peek();
    int u = UITLEG(k);
    bool blocked = aflegstapel->IsBlocked();
    
    VulTypeKaarten(speler);
    
    int jokers = kaarttypen[Gr_Joker]->GetSize() + kaarttypen[Kl_Joker]->GetSize();
    int size = kaarttypen[u]->GetSize();
    afleg_kaart_actie = Geen_actie;
    bool voldoende_punten = HeeftUitgelegd(team) ? BepaalUitlegWaarde(speler, ABS(k)) + TeamUitlegPunten(false, team) >= GetMinimumPunten(team)
                                                 : BepaalUitlegWaarde(speler, ABS(k)) >= GetMinimumPunten(team);
    

    // if (speler == Zuid)
    //{
    //     int vriend = Noord;
    // }

    // Bij een zwarte drie of een joker mag de stapel niet worden opgepakt
    if (u == Zwarte3 || u == Gr_Joker || u == Kl_Joker)
    {
        return false;
    }
    if (blocked)
    {
        // stapel is geblokkeerd
        if (size < 2)
        {
            if (speler == Zuid)
            {
                tekstfout = "De aflegstapel is geblokkeerd, je moet minstens 2 kaarten hebben van de zelfde serie!";
            }
            return false;
        }
        if (voldoende_punten)
        {
            afleg_kaart_actie = Twee_kaart_uitleg;
        }
        else
        {
            if (speler == Zuid)
            {
                tekstfout = "Je hebt onvoldoende punten in de hand plus deze kaart om uit te leggen!";
            }
            return false;
        }
    }
    else
    {
        if (size >= 2)
        {
            if (!team_uitgelegd[team][u]->IsEmpty())
            {
                afleg_kaart_actie = Kaart_aanleg;
            }
            else
            {
                if (voldoende_punten)
                {
                    afleg_kaart_actie = Twee_kaart_uitleg;
                }
                else
                {
                    if (speler == Zuid)
                    {
                        tekstfout = "Je hebt onvoldoende punten in de hand plus deze kaart om uit te leggen!";
                    }
                    return false;
                }
            }
        }
        else if (size == 1)
        {
            if (!team_uitgelegd[team][u]->IsEmpty())
            {
                afleg_kaart_actie = Kaart_aanleg;
            }
            else
            {
                if (voldoende_punten && jokers > 0)
                {
                    afleg_kaart_actie = Plus_joker_uitleg;
                }
                else
                {
                    if (speler == Zuid)
                    {
                        tekstfout = "Je hebt onvoldoende punten en of jokers in de hand plus deze kaart om uit te leggen!";
                    }
                    return false;
                }
            }
        }
        else
        {
            if (!HeeftUitgelegd(team))
            {
                if (speler == Zuid)
                {
                    tekstfout = "Je moet minstens 1 kaart van de zelfde serie met een joker hebben!";
                }
                return false;
            }
            else
            {
                if (team_uitgelegd[team][u]->IsEmpty())
                {
                    if (speler == Zuid)
                    {
                        tekstfout = "De aflegstapel niet geblokkeerd maar er is met deze kaart nog niet uitgelegd!";
                    }
                    return false;
                }
                else
                {
                    afleg_kaart_actie = Kaart_aanleg;
                }
            }
        }
    }
    if (afleg_kaart_actie > Geen_actie)
    {
        afleg_kaart = ABS(k);
        aflegstapel_oppakken = true;
        spelers[speler]->UnselectAll();
        for (int i = 0; i < spelers[speler]->GetSize(); ++i)
        {
            k = spelers[speler]->GetElementAt(i);
            if (UITLEG(k) == u)
            {
                spelers[speler]->SetElementAt(i, -k);
            }
        }
    }
    return true;
}

/// @brief Bepaal of de ai de aflegstapel wil oppakken
/// @param speler eSpeler
/// @return true of false
bool Game_t::WilOppakken(int speler)
{
    int team = Teamid[speler];
    if (afleg_kaart_actie == Twee_kaart_uitleg || afleg_kaart_actie == Plus_joker_uitleg)
    {
        return true;
    }
    else
    {
        int k = aflegstapel->Peek();
        int u = UITLEG(k);
        if (team_uitgelegd[team][u]->GetSize() >= 3)
        {
            return true;
        }
    }
    return false;
}

/// @brief Bepaal de punten in de hand die uitgelegd kunnen worden
/// @param speler eSpeler
/// @param ak bovenste kaart van de aflegstapel of 0
/// @return mogelijke uitleg punten
int Game_t::BepaalUitlegWaarde(int speler, int ak)
{
    int totaal = 0;

    VulTypeKaarten(speler);
    // is er op de aflegkaarten geklikt ?
    if (ak != 0)
    {
        int au = UITLEG(ak);
        kaarttypen[au]->PushBack(ak);
    }
    int kl_j = kaarttypen[Kl_Joker]->GetSize(); // tel aantal grote jokers
    int gr_j = kaarttypen[Gr_Joker]->GetSize(); // tel aantal kleine jokers
    int m_j = 0;                                // tel aantal extra jokers die mee worden uitgelegd
    for (int i = Aas; i <= Vier; ++i)
    {
        int pnt = kaarttypen[i]->PuntenStapel(); // tel het aantal punten van de stapel
        int cnt = kaarttypen[i]->GetSize();      // tel het aantal kaarten van de stapel
        if (cnt >= 3)
        {
            totaal += pnt;
            m_j += MIN(cnt - 1, 3); // noit meer dan 3 jokers die mee kunnen worden uitgelegd
        }
        else if (cnt == 2 && gr_j + kl_j > 0) // bij 2 kaarten moet altijd een joker worden uitgelegd
        {
            if (gr_j > 0)
            {
                totaal += pnt + 50; // plus Grote Joker
                --gr_j;
            }
            else if (kl_j > 0)
            {
                totaal += pnt + 20; // plus Kleine Joker
                --kl_j;
            }
        }
    }
    // Zijn er nog jokers die mee kunnen worden uitgelegd ?
    if (m_j > 0 && gr_j + kl_j > 0)
    {
        while (m_j > 0 && gr_j + kl_j > 0)
        {
            if (gr_j > 0)
            {
                totaal += 50; // plus grote joker
                --gr_j;
            }
            else if (kl_j > 0)
            {
                totaal += 20; // plus kleine joker
                --kl_j;
            }
            --m_j;
        }
    }
    return totaal;
}

/// @brief Test of kaarten kunnen wordn uitgelegd of niet
/// @param speler eSpeler
/// @param col Aas .. Vier
/// @return true of false
bool Game_t::KanUitleggen(int speler, int col)
{
    int team = Teamid[speler];
    VulTypeKaarten(speler);
    if (!HeeftUitgelegd(team))
    {
        int uitlegwaarde = BepaalUitlegWaarde(speler, afleg_kaart);
        if (uitlegwaarde < GetMinimumPunten(team))
        {
            if (speler == Zuid)
            {
                tekstfout = "Je hebt niet genoeg punten in de hand om te kunnen uitleggen!";
            }
            return false;
        }
    }
    int plus = 0;
    if (afleg_kaart != 0)
    {
        if (UITLEG(afleg_kaart) != col)
        {
            if (speler == Zuid)
            {
                tekstfout = "De kaart van de aflegstapel en de gekozen column komen niet overeen!";
            }
            return false;
        }
        else
        {
            plus = 1;
        }
    }
    int kaarten = plus;
    int jokers = 0;
    if (!team_uitgelegd[team][col]->IsEmpty())
    {
        jokers += team_uitgelegd[team][col]->JokersStapel();
        kaarten += team_uitgelegd[team][col]->GetSize();
    }
    for (int i = 0; i < spelers[speler]->GetSize(); ++i)
    {
        int k = spelers[speler]->GetElementAt(i);
        if (SEL(k))
        {
            int u = UITLEG(k);
            if (u == Gr_Joker || u == Kl_Joker)
            {
                ++jokers;
                ++kaarten;
                continue;
            }
            else if (u == col)
            {
                ++kaarten;
                continue;
            }
            if (speler == Zuid)
            {
                tekstfout = "De geslecteerde kaart(en) en de gekozen column komen niet overeen!";
            }
            return false;
        }
    }
    if (kaarten < 3)
    {
        if (speler == Zuid)
        {
            tekstfout = "Er zijn te weinig kaarten geselecteerd om te mogen uitleggen!";
        }
        return false;
    }
    kaarten -= jokers;
    if (jokers > 3 || jokers >= kaarten)
    {
        if (speler == Zuid)
        {
            tekstfout = "Er worden meer jokers uitgelegd dan toegestaan!";
        }
        return false;
    }
    bool is_canasta = kaarten + jokers >= 7 || HeeftCanastaGelegd(team);
    kaarten = spelers[speler]->SelectedStapel();
    int rest = is_canasta ? 0 : 2;
    if (!is_canasta && spelers[speler]->GetSize() - kaarten < rest)
    {
        if (speler == Zuid)
        {
            tekstfout = "Er blijven te weinig kaarten over bij deze uitleg!";
        }
        return false;
    }
    return true;
}

/// @brief Kan de speler al zijn kaarten uitleggen?
/// @param speler eSpeler
/// @return true of false
bool Game_t::KanAllesUitleggen(int speler)
{
    bool result = false;
    VulTypeKaarten(speler);
    int zwarte3 = kaarttypen[Zwarte3]->GetSize();
    if (zwarte3 >= 2)
    {
        // er blijft na het wegwerpen van een kaart toch nog een zwarte drie over => false
        return result;
    }
    int spelerjokers = kaarttypen[Gr_Joker]->GetSize() + kaarttypen[Kl_Joker]->GetSize();
    int spelersize = spelers[speler]->GetSize();
    for (int u = Aas; u <= Vier; ++u)
    {
        int sizehand = kaarttypen[u]->GetSize();
        if (sizehand >= 3)
        {
            spelersize -= sizehand;
        }
        else if (sizehand == 2 && spelerjokers > 0)
        {
            spelersize -= sizehand - 1;
            --spelerjokers;
        }
    }
    if (spelersize - spelerjokers > 1)
    {
        // er blijvan te veel niet jokers over zelf na het wegwerpen  van een kaart => false
        return result;
    }
    if (spelerjokers > 0)
    {
        int team = Teamid[speler];
        // test of er nog jokers uitgelegd kunnen worden
        // eerste loop: sluit aan bij stapels die al een of meer jokers hebben maar minder dan 3
        for (int u = Aas; u <= Vier; u++)
        {
            int size = team_uitgelegd[team][u]->GetSize();
            int jokers = team_uitgelegd[team][u]->JokersStapel();
            int magjokers = 0;
            if (size > 0 && jokers > 0 && jokers < 3)
            {
                magjokers = MIN(size - 1, 3) - jokers;
                while (magjokers > 0 && spelerjokers > 0)
                {
                    --spelersize;
                    --spelerjokers;
                }
            }
        }
        // tweede loop: sluit aan bij stapels die nog geen jokers hebben
        for (int u = Aas; u <= Vier; u++)
        {
            int size = team_uitgelegd[team][u]->GetSize();
            int jokers = team_uitgelegd[team][u]->JokersStapel();
            int magjokers = 0;
            if (size > 0 && jokers == 0)
            {
                magjokers = MIN(size - 1, 3);
                while (magjokers > 0 && spelerjokers > 0)
                {
                    --spelersize;
                    --spelerjokers;
                }
            }
        }
    }
    if (spelersize <= 1)
    {
        result = true;
    }
    return result;
}

/// @brief Speler legt al zijn kaarten uit
/// @param speler eSpeler
void Game_t::LegAllesUit(int speler)
{
    int team = Teamid[speler];
    
    do
    {
        VulTypeKaarten(speler);
    } while (LegEenStapelUit(speler));
    // Zijn er nog jokers uit te leggen
    if (kaarttypen[Gr_Joker]->GetSize() + kaarttypen[Kl_Joker]->GetSize() > 0)
    {
        LegExtraJokersUit(speler, false);
    }
}

/// @brief Leg een stapel uit
/// @param speler eSpeler
/// @return true of false
bool Game_t::LegEenStapelUit(int speler)
{
    VulTypeKaarten(speler);
    int gr_jokers = kaarttypen[Gr_Joker]->GetSize();
    int kl_jokers = kaarttypen[Kl_Joker]->GetSize();
    int col = ZoekDrieKaart(speler);
    if (col > 0)
    {
        SetKaarten(speler, col);
        if (KanUitleggen(speler, col))
        {
            UpdateTeam(speler, col);
            tekstinfo = SpelerNaam[speler] + " heeft kaarten uitgelegd!";
        }
        return true;
    }
    col = ZoekTweeKaart(speler);
    if (col > 0)
    {
        SetKaarten(speler, col);
        if (gr_jokers > 0)
        {
            SetKaarten(speler, Gr_Joker);
        }
        else if (kl_jokers > 0)
        {
            SetKaarten(speler, Kl_Joker);
        }
        if (KanUitleggen(speler, col))
        {
            UpdateTeam(speler, col);
            tekstinfo = SpelerNaam[speler] + " heeft kaarten uitgelegd!";
        }
        return true;
    }
    return false;
}

/// @brief Leg extra jokers uit
/// @param speler eSpeler
/// @param test true stop als aan de eis is voldaan, false leg alle jokers uit
void Game_t::LegExtraJokersUit(int speler, bool test)
{
    int team = Teamid[speler];
    int plusjokers = 0;
    int gr_jokers = 0;
    int kl_jokers = 0;
    // eerste loop: leg jokers bij stapels met een of meer jokers
    for (int u = Aas; u <= Vier; ++u)
    {
        int size = team_uitgelegd[team][u]->GetSize();
        int jokers = team_uitgelegd[team][u]->JokersStapel();
        if (size > 0 && jokers > 0 && jokers < 3)
        {
            VulTypeKaarten(speler);
            gr_jokers = kaarttypen[Gr_Joker]->GetSize();
            kl_jokers = kaarttypen[Kl_Joker]->GetSize();
            spelers[speler]->UnselectAll();
            plusjokers = MIN(size - 1, 3) - jokers;
            while (plusjokers > 0 && gr_jokers + kl_jokers > 0)
            {
                if (gr_jokers > 0)
                {
                    SetKaarten(speler, Gr_Joker);
                    --gr_jokers;
                    --plusjokers;
                    if (KanUitleggen(speler, u))
                    {
                        UpdateTeam(speler, u);
                        tekstinfo = SpelerNaam[speler] + " heeft kaart(en) bijgelegd!";
                    }
                }
                else if (kl_jokers > 0)
                {
                    SetKaarten(speler, Kl_Joker);
                    --kl_jokers;
                    --plusjokers;
                    if (KanUitleggen(speler, u))
                    {
                        UpdateTeam(speler, u);
                        tekstinfo = SpelerNaam[speler] + " heeft kaart(en) bijgelegd!";
                    }
                }
                if (test && TeamUitlegPunten(false, team) >= GetMinimumPunten(team))
                {
                    // test == true en genoeg punten uitgelegd => stop while
                    break;
                }
            }
        }
        if (test && TeamUitlegPunten(false, team) >= GetMinimumPunten(team))
        {
            // test == true en genoeg punten uitgelegd => stop for
            break;
        }
    }
    if (test && TeamUitlegPunten(false, team) >= GetMinimumPunten(team))
    {
        // test == true en genoeg punten uitgelegd => stop function
        return;
    }
    // tweede loop: leg joker(s) bij goede stapels
    for (int u = Aas; u <= Vier; ++u)
    {
        int size = team_uitgelegd[team][u]->GetSize();
        int jokers = team_uitgelegd[team][u]->JokersStapel();
        if (size > 0 && jokers == 0)
        {
            VulTypeKaarten(speler);
            gr_jokers = kaarttypen[Gr_Joker]->GetSize();
            kl_jokers = kaarttypen[Kl_Joker]->GetSize();
            plusjokers = MIN(size - 1, 3);
            while (plusjokers > 0 && gr_jokers + kl_jokers > 0)
            {
                if (gr_jokers > 0)
                {
                    SetKaarten(speler, Gr_Joker);
                    --gr_jokers;
                    --plusjokers;
                    if (KanUitleggen(speler, u))
                    {
                        UpdateTeam(speler, u);
                        tekstinfo = SpelerNaam[speler] + " heeft kaart(en) bijgelegd!";
                    }
                }
                else if (kl_jokers > 0)
                {
                    SetKaarten(speler, Kl_Joker);
                    --kl_jokers;
                    --plusjokers;
                    if (KanUitleggen(speler, u))
                    {
                        UpdateTeam(speler, u);
                        tekstinfo = SpelerNaam[speler] + " heeft kaart(en) bijgelegd!";
                    }
                }
                if (test && TeamUitlegPunten(false, team) >= GetMinimumPunten(team))
                {
                    // test == true en genoeg punten uitgelegd => stop while
                    break;
                }
            }
        }
        if (test && TeamUitlegPunten(false, team) >= GetMinimumPunten(team))
        {
            // test == true en genoeg punten uitgelegd => stop for
            break;
        }
    }
}

/// @brief Vul kaarttype array's met kaarten uit de hand
/// @param speler eSpeler
void Game_t::VulTypeKaarten(int speler)
{
    for (int i = Rode3; i <= Gr_Joker; ++i)
    {
        kaarttypen[i]->Clear();
    }
    for (int i = 0; i < spelers[speler]->GetSize(); ++i)
    {
        int k = spelers[speler]->GetElementAt(i);
        ASSERT(k != 0);
        int u = UITLEG(k);
        kaarttypen[u]->PushBack(k);
    }
}

/// @brief Zoek een drie kaart of meer in de hand
/// @param speler eSpeler
/// @return kolom of 0 als niet gevonden
int Game_t::ZoekDrieKaart(int speler)
{
    int col = 0;
    
    VulTypeKaarten(speler);
    for (int u = Aas; u <= Vier; ++u)
    {
        if (kaarttypen[u]->GetSize() >= 3)
        {
            col = u;
            break;
        }
    }
    return col;
}

/// @brief Zoek een twee kaart in de hand, er moet dan minstens nog een joker in de hand zijn
/// @param speler eSpeler
/// @return kolom of 0 als niet gevonden
int Game_t::ZoekTweeKaart(int speler)
{
    int col = 0;
    
    VulTypeKaarten(speler);
    if (kaarttypen[Gr_Joker]->IsEmpty() && kaarttypen[Kl_Joker]->IsEmpty())
    {
        return col;
    }
    for (int u = Aas; u <= Vier; ++u)
    {
        if (kaarttypen[u]->GetSize() == 2)
        {
            col = u;
            break;
        }
    }
    return col;
}

/// @brief Zet selected kaarten in de hand per kaarttype
/// @param speler
/// @param u eKaartTypen
void Game_t::SetKaarten(int speler, int u)
{
    for (int i = 0; i < spelers[speler]->GetSize(); ++i)
    {
        int k = ABS(spelers[speler]->GetElementAt(i));
        if (UITLEG(k) == u)
        {
            spelers[speler]->SetElementAt(i, -k);
            if (u == Gr_Joker || u == Kl_Joker)
            {
                // jokers worden per stuk geactiveerd
                break;
            }
        }
    }
}

/// @brief Controleer of er een canasta kan worden gemaakt met jokers in de hand
/// @param speler eSpeler
/// @return Kolom of 0 als het niet wordt gevonden
int Game_t::KanCanastaWordenGemaakt(int speler)
{
    int team = Teamid[speler];
    int col = 0;
    
    VulTypeKaarten(speler);
    int cntjokers = kaarttypen[Gr_Joker]->GetSize() + kaarttypen[Kl_Joker]->GetSize();
    for (int u = Aas; u <= Vier; ++u)
    {
        int size = team_uitgelegd[team][u]->GetSize();
        int jokers = team_uitgelegd[team][u]->JokersStapel();
        if (size == 6 && jokers <= 2 && cntjokers >= 1)
        {
            col = u;
            break;
        }
        if (size == 5 && jokers <= 1 && cntjokers >= 2)
        {
            col = u;
            break;
        }
        if (size == 4 && jokers == 0 && cntjokers >= 3)
        {
            col = u;
            break;
        }
    }
    return col;
}

/// @brief Controleer of er kaarten kunnen worden bijgelegd
/// @param speler eSpeler
/// @param ak aflegkaart
void Game_t::KaartenBijleggen(int speler, int ak)
{
    int team = Teamid[speler];
    
    VulTypeKaarten(speler);
    if (ak > 0)
    {
        aflegstapel->PopBack();
        int u = UITLEG(ak);
        team_uitgelegd[team][u]->PushBack(ak);
        return;
    }
    for (int u = Aas; u <= Vier; ++u)
    {
        int s1 = kaarttypen[u]->GetSize();
        int s2 = team_uitgelegd[team][u]->GetSize();
        if (s1 > 0 && s2 > 0)
        {
            spelers[speler]->UnselectAll();
            for (int i = 0; i < spelers[speler]->GetSize(); ++i)
            {
                int k = spelers[speler]->GetElementAt(i);
                if (UITLEG(k) == u)
                {
                    spelers[speler]->SetElementAt(i, -k);
                }
            }
            if (KanUitleggen(speler, u))
            {
                UpdateTeam(speler, u);
                tekstinfo = SpelerNaam[speler] + " heeft kaart(en) bijgelegd!";
            }
        }
    }
}

/// @brief Test of er een canasta gemaakt kan worden, zo ja leg deze dan uit
/// @param speler eSpeler
void Game_t::CanastaUitleggen(int speler)
{
    int team = Teamid[speler];
    
    VulTypeKaarten(speler);
    int kl_jokers = kaarttypen[Kl_Joker]->GetSize();
    int gr_jokers = kaarttypen[Gr_Joker]->GetSize();
    if (kl_jokers + gr_jokers > 0)
    {
        int col = KanCanastaWordenGemaakt(speler);
        if (col != 0)
        {
            int size = team_uitgelegd[team][col]->GetSize();
            while (size < 7)
            {
                spelers[speler]->UnselectAll();
                if (kl_jokers > 0)
                {
                    SetKaarten(speler, Kl_Joker);
                    --kl_jokers;
                }
                else if (gr_jokers > 0)
                {
                    SetKaarten(speler, Gr_Joker);
                    --gr_jokers;
                }
                if (KanUitleggen(speler, col))
                {
                    UpdateTeam(speler, col);
                    tekstinfo = SpelerNaam[speler] + " heeft canasta gemaakt!";
                }
                ++size;
            }
        }
    }
}

/// @brief Uitleggen in stappen
/// @param speler eSpeler
/// @param ak afleg_kaart
void Game_t::StappenUitleggen(int speler, int ak)
{
    int team = Teamid[speler], u;

    if (ak > 0)
    {
        u = UITLEG(ak);
        aflegstapel->PopBack();
        spelers[speler]->PushBack(ak);
        VulTypeKaarten(speler);
        spelers[speler]->UnselectAll();
        if (afleg_kaart_actie == Twee_kaart_uitleg)
        {
            SetKaarten(speler, u);
        }
        if (afleg_kaart_actie == Plus_joker_uitleg)
        {
            SetKaarten(speler, u);
            if (!kaarttypen[Gr_Joker]->IsEmpty())
            {
                SetKaarten(speler, Gr_Joker);
            }
            else if (!kaarttypen[Kl_Joker]->IsEmpty())
            {
                SetKaarten(speler, Kl_Joker);
            }
        }
        if (KanUitleggen(speler, u))
        {
            UpdateTeam(speler, u);
            tekstinfo = SpelerNaam[speler] + " heeft kaart(en) uitgelegd!";
        }
        return;
    }
    // test of er genoeg punten zijn om uitgelegd te kunnen worden
    if (BepaalUitlegWaarde(speler, ak) + TeamUitlegPunten(false, team) >= GetMinimumPunten(team))
    {
        do
        {
            VulTypeKaarten(speler);
            if (!LegEenStapelUit(speler))
            {
                break;
            }
        } while (TeamUitlegPunten(false, team) < GetMinimumPunten(team));
        if (TeamUitlegPunten(false, team) < GetMinimumPunten(team))
        {
            // Nog te weinig punten uitgelegd, extra jokers bijleggen
            ASSERT(kaarttypen[Gr_Joker]->GetSize() + kaarttypen[Kl_Joker]->GetSize() > 0);
            LegExtraJokersUit(speler, true);
        }
    }
}

/// @brief Onderzoek of alles uitgelegd kan worden, als true leg dan alles uit
///        als false onderzoek of er extra stapel uitgelegd kan worden, als true leg dan een stapel uit
/// @param speler eSpeler
void Game_t::ExtraKaartenUitleggen(int speler)
{
    int team = Teamid[speler];
    int vriend = Vriendid[speler];
    if (spelers[vriend]->GetSize() <= 5)
    {
        if (KanAllesUitleggen(speler))
        {
            LegAllesUit(speler);
        }
    }
    // leg een stapel uit als er nog genoeg kaarten inde hand zijn
    if (spelers[speler]->GetSize() >= 8 || HeeftCanastaGelegd(team ^ 1))
    {
        LegEenStapelUit(speler);
    }
}

/// @brief Controleer of er kaarten kunnen worden uitgelegd
/// @param speler eSpeler
void Game_t::KaartenUitleggen(int speler)
{
    int team = Teamid[speler];

    // kaart van de aflegstapel opgepakt
    if (afleg_kaart > 0)
    {
        switch (afleg_kaart_actie)
        {
        case Kaart_aanleg:
        {
            KaartenBijleggen(speler, afleg_kaart);
            UpdateAflegstapel(speler);
            afleg_kaart = 0;
            break;
        }
        case Twee_kaart_uitleg:
        case Plus_joker_uitleg:
        {
            StappenUitleggen(speler, afleg_kaart);
            afleg_kaart = 0;
            if (TeamUitlegPunten(false, team) < GetMinimumPunten(team))
            { 
                StappenUitleggen(speler, 0);
            }
            UpdateAflegstapel(speler);
            break;
        }
        } // end switch
    }

    // geen kaart van de aflegstapel opgepakt of vervolgstappen
    if (HeeftUitgelegd(team))
    {
        // kunnen er kaarten worden bijgelegd
        KaartenBijleggen(speler, 0);
        // kan er een canasta worden gemaakt met jokers in de hand
        CanastaUitleggen(speler);
    }
    else
    {
        StappenUitleggen(speler, 0);
    }
    // als er een canasta is gelegd en de vriend heeft niet veel kaarten meer
    // onderzoek dan of alle kaarten kunnen worden uitgelegd.
    if (HeeftCanastaGelegd(team) || HeeftCanastaGelegd(team ^ 1))
    {
        ExtraKaartenUitleggen(speler);
    }
}

/// @brief Update de team uitgelegde kaarten
/// @param speler eSpeler
/// @param col Uitleg kolom
void Game_t::UpdateTeam(int speler, int col)
{
    int team = Teamid[speler];
    Stapel_t *remove = new Stapel_t(7);
    for (int i = 0; i < spelers[speler]->GetSize(); ++i)
    {
        int k = spelers[speler]->GetElementAt(i);
        if (SEL(k))
        {
            team_uitgelegd[team][col]->PushBack(ABS(k));
            remove->PushBack(k);
        }
    }
    if (afleg_kaart != 0)
    {
        team_uitgelegd[team][col]->PushBack(ABS(afleg_kaart));
        afleg_kaart = 0;
        aflegstapel->PopBack();
    }
    team_uitgelegd[team][col]->Sort();
    for (int i = 0; i < remove->GetSize(); ++i)
    {
        int k = remove->GetElementAt(i);
        int idx = spelers[speler]->Search(k);
        if (idx != -1)
        {
            spelers[speler]->DeleteAt(idx);
        }
    }
    delete remove;
}

/// @brief Update de aflegstapel
/// @param speler eSpeler
void Game_t::UpdateAflegstapel(int speler)
{
    int team = Teamid[speler], k, u;
    while (!aflegstapel->IsEmpty())
    {
        k = aflegstapel->PopBack();
        u = UITLEG(k);
        if (u == Rode3)
        {
            team_uitgelegd[team][Rode3]->PushBack(ABS(k));
        }
        else
        {
            spelers[speler]->PushBack(ABS(k));
        }
    }
    spelers[speler]->Sort();
    aflegstapel_oppakken = false;
}

/// @brief Constructor
Game_t::Game_t()
{
    for (int i = Noord; i <= West; ++i)
    {
        spelers[i] = new Stapel_t(14);
        ASSERT(spelers[i] != nullptr);
    }
    trekstapel = new Stapel_t(112);
    ASSERT(trekstapel != nullptr);
    aflegstapel = new Stapel_t(14);
    ASSERT(aflegstapel != nullptr);
    for (int i = Rode3; i <= Gr_Joker; ++i)
    {
        kaarttypen[i] = new Stapel_t(7);
        ASSERT(kaarttypen[i] != nullptr);
        if (i <= Vier)
        {
            team_uitgelegd[Wij][i] = new Stapel_t(7);
            ASSERT(team_uitgelegd[Wij][i] != nullptr);
            team_uitgelegd[Zij][i] = new Stapel_t(7);
            ASSERT(team_uitgelegd[Zij][i] != nullptr);
        }
    }
    teller = 0;
#ifdef NDEBUG
    deler = GetRandomValue(Noord, West);
#else
    deler = Noord;
#endif
    tekstinfo = "Welkom tot het Canasta van Peter Veenendaal";
    tekstfout = "";
    spelstatus = Spelstart;
}

/// @brief Destructor
Game_t::~Game_t()
{
    for (int i = Noord; i <= West; ++i)
    {
        delete spelers[i];
    }
    delete trekstapel;
    delete aflegstapel;
    for (int i = Rode3; i <= Gr_Joker; ++i)
    {
        delete kaarttypen[i];
        if (i <= Vier)
        {
            delete team_uitgelegd[Wij][i];
            delete team_uitgelegd[Zij][i];
        }
    }
}

/// @brief handle key press
/// @param key keycode
void Game_t::KeyPress(int key)
{
    if (enter_visible && key == KEY_ENTER)
    {
        enter_visible = false;
        if (spelstatus == Spelstart)
        {
            spelstatus = Spelspeel;
        }
    }
    if (spelstatus == Rondestop && key == KEY_F6)
    {
        ronde_continue = true;
    }
    if (spelstatus == Spelstop && key == KEY_F5)
    {
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
    tekstfout = "";
    // click on trekstapel vak
    if (x >= CARDREST_SIZE + SQUARE_SIZE &&
        x < CARDREST_SIZE + SQUARE_SIZE * 2 &&
        y >= CARDREST_SIZE + SQUARE_SIZE * 4 + 2 &&
        y < CARDREST_SIZE + SQUARE_SIZE * 5 - 2)
    {
        if (speler_ad_beurt != Zuid)
        {
            tekstfout = "Je bent niet aan de beurt...!";
            return;
        }
        if (spelstatus == Spelertrekkaart)
        {
            tekstfout = "";
            if (!Trekkaart(Zuid, true))
            {
                tekstfout = "de kaarten die opgepakt kunnen zijn op, einde ronde zonder winnaar!";
                spelstatus = Rondestop;
                return;
            };
            spelers[Zuid]->Sort();
            spelstatus = Speleruitleg;
        }
        else
        {
            tekstfout = "er mag maar 1 kaart worden getrokken!";
        }
        return;
    }
    // click on aflegstapel vak
    if (x >= CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_HALF_SIZE &&
        x < CARDREST_SIZE + SQUARE_SIZE * 3 + SQUARE_HALF_SIZE &&
        y >= CARDREST_SIZE + SQUARE_SIZE * 4 + 2 &&
        y < CARDREST_SIZE + SQUARE_SIZE * 5 - 2)
    {
        if (speler_ad_beurt != Zuid)
        {
            tekstfout = "Je bent niet aan de beurt...!";
            return;
        }
        if (spelstatus == Spelertrekkaart)
        {
            if (!MagOppakken(Zuid))
            {
                return;
            }
            spelstatus = Speleruitleg;
        }
        else if (spelstatus == Speleruitleg)
        {
            if (TeamUitlegPunten(false, Wij) > 0 && TeamUitlegPunten(false, Wij) < GetMinimumPunten(Wij))
            {
                tekstfout = "Nog niet voldoende punten uitgelegd!";
                return;
            }
            if (spelers[Zuid]->SelectedStapel() != 1)
            {
                tekstfout = "Om weg te werpen mag maar 1 kaart gekozen worden!";
                return;
            }
            else
            {
                tekstfout = "";
                int k = 0;
                for (int i = 0; i < spelers[Zuid]->GetSize(); ++i)
                {
                    k = spelers[Zuid]->GetElementAt(i);
                    if (SEL(k))
                    {
                        break;
                    }
                }
                ASSERT(k != 0);
                int idx = spelers[Zuid]->Search(k);
                ASSERT(idx >= 0);
                spelers[Zuid]->DeleteAt(idx);
                aflegstapel->PushBack(ABS(k));
                if (!spelers[Zuid]->IsEmpty())
                {
                    spelers[Zuid]->Sort();
                    spelstatus = Spelerstop;
                }
                else
                {
                    spelstatus = Rondestop;
                    speler_winnaar = Zuid;
                }
            }
        }
        return;
    }
    // click on cards
    if (spelstatus == Speleruitleg)
    {
        int len = spelers[Zuid]->GetSize();
        if (x >= CARDREST_SIZE + SQUARE_SIZE &&
            x < CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * (len + 1) &&
            y >= CARDREST_SIZE + 9 * SQUARE_SIZE &&
            y < CARDREST_SIZE + 9 * SQUARE_SIZE + CARD_HEIGHT_SIZE)
        {
            if (speler_ad_beurt != Zuid)
            {
                tekstfout = "Je bent niet aan de beurt...!";
                return;
            }
            if (spelstatus == Spelertrekkaart)
            {
                tekstfout = "Je moet eerst een kaart trekken voordat je gaat uitleggen of weg werpen...!";
                return;
            }
            else if (spelstatus == Speleruitleg)
            {
                for (int i = 0; i < len; ++i)
                {
                    if (x >= CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * i &&
                        x < CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * (i + 1))
                    {
                        int k = spelers[Zuid]->GetElementAt(i);
                        spelers[Zuid]->SetElementAt(i, -k);
                        return;
                    }
                }
                if (x >= CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * len &&
                    x < CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * (len + 1))
                {
                    int k = spelers[Zuid]->GetElementAt(len - 1);
                    spelers[Zuid]->SetElementAt(len - 1, -k);
                }
            }
            return;
        }
    }
    // click on uitleg vakje
    if (spelstatus == Speleruitleg)
    {
        if (x >= CARDREST_SIZE + SQUARE_SIZE &&
            x < CARDREST_SIZE + SQUARE_SIZE * 13 &&
            y >= CARDREST_SIZE + SQUARE_SIZE * 5 &&
            y < CARDREST_SIZE + SQUARE_SIZE * 6)
        {
            if (speler_ad_beurt != Zuid)
            {
                tekstfout = "Je bent niet aan de beurt...!";
                return;
            }
            if (spelstatus == Speleruitleg)
            {
                int col = 0;
                for (int i = Aas; i <= Vier; ++i)
                {
                    if (x >= CARDREST_SIZE + SQUARE_SIZE + SQUARE_SIZE * i &&
                        x < CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_SIZE * i)
                    {
                        col = i;
                        if (!KanUitleggen(Zuid, col))
                        {
                            return;
                        }
                        else
                        {
                            UpdateTeam(Zuid, col);
                            if (TeamUitlegPunten(false, Wij) >= GetMinimumPunten(Wij))
                            {
                                tekstfout = "";
                                if (aflegstapel_oppakken)
                                {
                                    UpdateAflegstapel(Zuid);
                                }
                                if (spelers[Zuid]->IsEmpty())
                                {
                                    speler_winnaar = Zuid;
                                    spelstatus = Rondestop;
                                }
                            }
                            else
                            {
                                tekstfout = "Nog niet voldoende punten uitgelegd!";
                            }
                            break;
                        }
                    }
                }
            }
            return;
        }
    }
}

/// @brief Update actions
void Game_t::Update()
{
    ++teller;
    if (teller % 10 != 0)
    {
        return;
    }
    if (spelstatus == Spelstart)
    {
        InitGame();
        return;
    }
    if (spelstatus == Spelspeel)
    {
        spelstatus = Rondestart;
        return;
    }
    if (spelstatus == Rondestart)
    {
        InitRonde();
        spelstatus = Rondespeel;
        return;
    }
    if (spelstatus == Rondespeel)
    {
        VulDeck();
        spelstatus = Schudden;
        return;
    }
    if (spelstatus == Schudden)
    {
        SchudDeck();
        spelstatus = Delen;
        return;
    }
    if (spelstatus == Delen)
    {
        DeelDeck();
        spelstatus = Spelerstart;
        return;
    }
    if (spelstatus == Spelerstart)
    {
        trek_kaart = -1;
        ++speler_ad_beurt;
        if (speler_ad_beurt > West)
        {
            speler_ad_beurt = Noord;
        }
        int team = Teamid[speler_ad_beurt];
        if (speler_ad_beurt != Zuid)
        {
            tekstinfo = "";
        }
        afleg_kaart = 0;
        afleg_kaart_actie = Geen_actie;
        aflegstapel_oppakken = false;
        for (int i = 0; i < spelers[speler_ad_beurt]->GetSize(); ++i)
        {
            int k = ABS(spelers[speler_ad_beurt]->GetElementAt(i));
            spelers[speler_ad_beurt]->SetElementAt(i, k);
        }
        spelstatus = Spelerspeel;
        return;
    }
    if (spelstatus == Spelerspeel)
    {
        spelstatus = Spelertrekkaart;
        return;
    }
    if (spelstatus == Spelertrekkaart)
    {
        tekstinfo = "";
        if (speler_ad_beurt != Zuid)
            Do_Ai_Move(speler_ad_beurt);
        else
            tekstinfo = " trek een kaart";
        return;
    }
    if (spelstatus == Speleruitleg)
    {
        tekstinfo = "";
        if (speler_ad_beurt != Zuid)
            Do_Ai_Move(speler_ad_beurt);
        else
            tekstinfo = " selecteer kaart(en) om weg te leggen";
        return;
    }
    if (spelstatus == Spelerwerp)
    {
        if (speler_ad_beurt != Zuid)
            Do_Ai_Move(speler_ad_beurt);
        return;
    }
    if (spelstatus == Spelerstop)
    {
        spelstatus = Spelerstart;
        return;
    }
    if (spelstatus == Rondestop)
    {
        if (speler_winnaar >= Noord && speler_winnaar <= West)
        {
            int team = Teamid[speler_winnaar];
            tekstinfo = "Ronde is afgelopen, " + SpelerNaam[speler_winnaar] + " heeft alles uitgelegd..., click op F6 om verder te gaan";
        }
        else
        {
            tekstinfo = "Ronde is afgelopen, omdat de kaarten die gtrokken kunnen worden op zijn, click op F6 om verder te gaan";
        }
        if (!ronde_geteld)
        {
            totaal_punten[Wij] += TeamUitlegPunten(true, Wij);
            totaal_punten[Zij] += TeamUitlegPunten(true, Zij);
            if (totaal_punten[Wij] >= 5000 || totaal_punten[Zij] >= 5000)
            {
                if (totaal_punten[Wij] == totaal_punten[Zij])
                {
                    team_winnaar = -1;
                }
                else
                {
                    team_winnaar = totaal_punten[Wij] > totaal_punten[Zij] ? Wij : Zij;
                }
                spelstatus = Spelstop;
            }
            ronde_geteld = true;
        }
        if (ronde_continue)
            spelstatus = Rondestart;
        return;
    }
    if (spelstatus == Spelstop)
    {
        if (team_winnaar >= 0)
        {
            tekstinfo = "Spel is afgelopen, " + TeamNaam[team_winnaar] + " zijn de winnaar..., click op F5 om opnieuw te starten";
        }
        else
        {
            tekstinfo = "Spel is afgelopen en gelijk geeindigd.., click op F6 om opnieuw te starten";
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
    if (spelstatus == Spelstart)
    {
        ClearBackground(DARKGRAY);
        for (int i = 0; i < 47; ++i)
        {
            DrawText(
                TekstUitleg[i].c_str(),
                CARDREST_SIZE,
                VERTICALLINE_SIZE * i + CARDREST_SIZE,
                20,
                KleurUitleg[i]);
        }
        DrawTexture(
            img->GetImage(enterbutton),
            SCREEN_WIDTH_SIZE - SQUARE_SIZE * 2,
            SCREEN_HEIGHT_SIZE - CARDREST_SIZE - SQUARE_SIZE,
            RAYWHITE);
        enter_visible = true;
        return;
    }
    ClearBackground(RAYWHITE);
    DrawTexture(
        img->GetImage(speeltafel),
        0,
        0,
        RAYWHITE);
    DrawTexture(
        img->GetImage(dir),
        0,
        0,
        RAYWHITE);
    // uitleg gebied zij
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE,
        YELLOW);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4,
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE * 4,
        YELLOW);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4,
        YELLOW);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE * 4,
        YELLOW);
    DrawText(
        "Zij",
        CARDREST_SIZE * 2 + SQUARE_SIZE,
        SQUARE_SIZE * 4 - SQUARE_HALF_SIZE / 2,
        20,
        YELLOW);
    for (int i = Rode3; i <= Vier; ++i)
    {
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE,
            YELLOW);
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE * 2,
            YELLOW);
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 2,
            YELLOW);
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE * 2,
            YELLOW);
        DrawText(
            KaartUitlegLetter[i].c_str(),
            CARDREST_SIZE + SQUARE_SIZE * (i + 1) + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE + 3,
            20,
            BLACK);
    }
    // uitleg gebied wij
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5,
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE * 5,
        GREEN);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 8,
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE * 8,
        GREEN);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5,
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 8,
        GREEN);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE * 5,
        CARDREST_SIZE + SQUARE_SIZE * 13,
        CARDREST_SIZE + SQUARE_SIZE * 8,
        GREEN);
    DrawText(
        "Wij",
        CARDREST_SIZE * 2 + SQUARE_SIZE,
        SQUARE_SIZE * 8 - SQUARE_HALF_SIZE / 2,
        20,
        GREEN);
    for (int i = 0; i < 12; ++i)
    {
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 5,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE * 5,
            GREEN);
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 6,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE * 6,
            GREEN);
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 5,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 6,
            GREEN);
        DrawLine(
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE * 5,
            CARDREST_SIZE + SQUARE_SIZE * i + SQUARE_SIZE * 2,
            CARDREST_SIZE + SQUARE_SIZE * 6,
            GREEN);
        DrawText(
            KaartUitlegLetter[i].c_str(),
            CARDREST_SIZE + SQUARE_SIZE * (i + 1) + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 5 + 3,
            20,
            BLACK);
    }
    // trekstapel gebied
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        CARDREST_SIZE + SQUARE_SIZE * 2,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        LIGHTGRAY);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        CARDREST_SIZE + SQUARE_SIZE * 2,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        LIGHTGRAY);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        LIGHTGRAY);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE * 2,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        CARDREST_SIZE + SQUARE_SIZE * 2,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        LIGHTGRAY);
    DrawText(
        "Trek",
        CARDREST_SIZE + SQUARE_SIZE + 4,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 6,
        20,
        DARKGRAY);
    // aflegstapel gebied
    Color c = aflegstapel->IsBlocked() ? RED : LIGHTGRAY;
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        CARDREST_SIZE + SQUARE_SIZE * 3 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        c);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        CARDREST_SIZE + SQUARE_SIZE * 3 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        c);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        c);
    DrawLine(
        CARDREST_SIZE + SQUARE_SIZE * 3 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 2,
        CARDREST_SIZE + SQUARE_SIZE * 3 + SQUARE_HALF_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 5 - 2,
        c);
    DrawText(
        "Afleg",
        CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_HALF_SIZE + 4,
        CARDREST_SIZE + SQUARE_SIZE * 4 + 6,
        20,
        DARKGRAY);
    Color wc = {255, 255, 255, 64};
    DrawRectangle(
        CARDREST_SIZE + SQUARE_SIZE,
        CARDREST_SIZE + SQUARE_SIZE * 8,
        12 * SQUARE_SIZE,
        SQUARE_HALF_SIZE,
        wc);
    DrawRectangle(
        CARDREST_SIZE + SQUARE_SIZE * 14,
        CARDREST_SIZE,
        SQUARE_SIZE * 3,
        SQUARE_SIZE * 10,
        wc);
    if (spelstatus >= Spelerstart)
    {
        // speler kaarten
        for (int i = 0; i < spelers[Noord]->GetSize(); ++i)
        {
#ifndef NDEBUG
            int k = ABS(spelers[Noord]->GetElementAt(i));
            ASSERT(k != 0);
            DrawTexture(
                img->GetImage(k),
                CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * i,
                CARDREST_SIZE,
                RAYWHITE);
#else
            DrawTexture(
                img->GetImage(Ba),
                CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * i,
                CARDREST_SIZE,
                RAYWHITE);
#endif
        }
        for (int i = 0; i < spelers[West]->GetSize(); ++i)
        {
#ifndef NDEBUG
            int k = ABS(spelers[West]->GetElementAt(i));
            ASSERT(k != 0);
            DrawTexture(
                img->GetImage(k),
                CARD_HALF_WIDTH_SIZE - CARDREST_SIZE,
                CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_HEIGHT_SIZE * i,
                RAYWHITE);
#else
            DrawTexture(
                img->GetImage(Ba),
                CARD_HALF_WIDTH_SIZE - CARDREST_SIZE,
                CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_HEIGHT_SIZE * i,
                RAYWHITE);
#endif
        }
        for (int i = 0; i < spelers[Zuid]->GetSize(); ++i)
        {
            int k = spelers[Zuid]->GetElementAt(i);
            ASSERT(k != 0);
            if (k < 0)
                DrawTexture(
                    img->GetImage(ABS(k)),
                    CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * i,
                    CARDREST_SIZE + 9 * SQUARE_SIZE - CARD_HALF_HEIGHT_SIZE,
                    RAYWHITE);
            else
                DrawTexture(
                    img->GetImage(ABS(k)),
                    CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_WIDTH_SIZE * i,
                    CARDREST_SIZE + 9 * SQUARE_SIZE,
                    RAYWHITE);
        }
        for (int i = 0; i < spelers[Oost]->GetSize(); ++i)
        {
#ifndef NDEBUG
            int k = ABS(spelers[Oost]->GetElementAt(i));
            ASSERT(k != 0);
            DrawTexture(
                img->GetImage(k),
                CARDREST_SIZE + 13 * SQUARE_SIZE,
                CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_HEIGHT_SIZE * i,
                RAYWHITE);
#else
            DrawTexture(
                img->GetImage(Ba),
                CARDREST_SIZE + 13 * SQUARE_SIZE,
                CARDREST_SIZE + SQUARE_SIZE + CARD_HALF_HEIGHT_SIZE * i,
                RAYWHITE);
#endif
        }
        // trekstapel
        if (!trekstapel->IsEmpty())
        {
            DrawTexture(
                img->GetImage(Ba),
                SQUARE_SIZE + CARDREST_SIZE * 2,
                CARDREST_SIZE + SQUARE_SIZE * 4 + 1,
                RAYWHITE);
            std::string aantal = std::to_string(trekstapel->GetSize());
            DrawText(
                aantal.c_str(),
                SQUARE_SIZE * 2 + CARDREST_SIZE * 2,
                CARDREST_SIZE + SQUARE_SIZE * 4 + CARD_HALF_HEIGHT_SIZE,
                16,
                BLACK);
        }
        // aflegstapel
        if (!aflegstapel->IsEmpty())
        {
            int k = ABS(aflegstapel->Peek());
            ASSERT(k != 0);
            DrawTexture(
                img->GetImage(k),
                SQUARE_SIZE * 2 + SQUARE_HALF_SIZE + CARDREST_SIZE * 2,
                CARDREST_SIZE + SQUARE_SIZE * 4 + 1,
                RAYWHITE);
            std::string aantal = std::to_string(aflegstapel->GetSize());
            if (aflegstapel->IsBlocked())
                aantal += " geblokkeerd";
            DrawText(
                aantal.c_str(),
                SQUARE_SIZE * 3 + SQUARE_HALF_SIZE + CARDREST_SIZE * 2,
                CARDREST_SIZE + SQUARE_SIZE * 4 + CARD_HALF_HEIGHT_SIZE,
                16,
                BLACK);
        }
        // uitlegkaarten zij
        for (int i = Rode3; i <= Vier; ++i)
        {
            int canasta = i > Rode3 ? team_uitgelegd[Zij][i]->CanastaStapel() : Geen;
            int size = team_uitgelegd[Zij][i]->GetSize();
            if (canasta == Geen && size > 0)
            {
                for (int j = 0; j < size; ++j)
                {
                    int k = ABS(team_uitgelegd[Zij][i]->GetElementAt(j));
                    DrawTexture(
                        img->GetImage(k),
                        CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                        CARDREST_SIZE * 4 + SQUARE_SIZE + CARDREST_SIZE * j,
                        RAYWHITE);
                }
            }
            else if (canasta == Onecht)
            {
                DrawText(
                    "Canasta",
                    CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                    CARDREST_SIZE * 4 + SQUARE_SIZE * 2,
                    16,
                    BLACK);
                bool is_uitgelegd = false;
                for (int j = 0; j < size; ++j)
                {
                    int k = ABS(team_uitgelegd[Zij][i]->GetElementAt(j));
                    if (UITLEG(k) == i && (KLEUR(k) == Klaveren || KLEUR(k) == Schoppen))
                    {
                        DrawTexture(
                            img->GetImage(k),
                            CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                            CARDREST_SIZE * 4 + SQUARE_SIZE,
                            RAYWHITE);
                        is_uitgelegd = true;
                        break;
                    }
                }
                if (!is_uitgelegd)
                {
                    int k = ZwarteKaarten[i];
                    DrawTexture(
                        img->GetImage(k),
                        CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                        CARDREST_SIZE * 4 + SQUARE_SIZE,
                        RAYWHITE);
                }
            }
            else if (canasta == Echt)
            {
                DrawText(
                    "Canasta",
                    CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                    CARDREST_SIZE * 4 + SQUARE_SIZE * 2,
                    16,
                    RED);
                bool is_uitgelegd = false;
                for (int j = 0; j < size; ++j)
                {
                    int k = ABS(team_uitgelegd[Zij][i]->GetElementAt(j));
                    if (UITLEG(k) == i && (KLEUR(k) == Harten || KLEUR(k) == Ruiten))
                    {
                        DrawTexture(
                            img->GetImage(k),
                            CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                            CARDREST_SIZE * 4 + SQUARE_SIZE,
                            RAYWHITE);
                        is_uitgelegd = true;
                        break;
                    }
                }
                if (!is_uitgelegd)
                {
                    int k = RodeKaarten[i];
                    DrawTexture(
                        img->GetImage(k),
                        CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                        CARDREST_SIZE * 4 + SQUARE_SIZE,
                        RAYWHITE);
                }
            }
        }
        // uitlegkaarten wij
        for (int i = Rode3; i <= Vier; ++i)
        {
            int canasta = i > Rode3 ? team_uitgelegd[Wij][i]->CanastaStapel() : Geen;
            int size = team_uitgelegd[Wij][i]->GetSize();
            if (canasta == Geen && size > 0)
            {
                for (int j = 0; j < size; ++j)
                {
                    int k = ABS(team_uitgelegd[Wij][i]->GetElementAt(j));
                    DrawTexture(
                        img->GetImage(k),
                        CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                        CARDREST_SIZE * 4 + SQUARE_SIZE * 5 + CARDREST_SIZE * j,
                        RAYWHITE);
                }
            }
            else if (canasta == Onecht)
            {
                DrawText(
                    "Canasta",
                    CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                    CARDREST_SIZE * 4 + SQUARE_SIZE * 6,
                    16,
                    BLACK);
                bool is_uitgelegd = false;
                for (int j = 0; j < size; ++j)
                {
                    int k = ABS(team_uitgelegd[Wij][i]->GetElementAt(j));
                    if (UITLEG(k) == i && (KLEUR(k) == Klaveren || KLEUR(k) == Schoppen))
                    {
                        DrawTexture(
                            img->GetImage(k),
                            CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                            CARDREST_SIZE * 4 + SQUARE_SIZE * 5,
                            RAYWHITE);
                        is_uitgelegd = true;
                        break;
                    }
                }
                if (!is_uitgelegd)
                {
                    int k = ZwarteKaarten[i];
                    DrawTexture(
                        img->GetImage(k),
                        CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                        CARDREST_SIZE * 4 + SQUARE_SIZE * 5,
                        RAYWHITE);
                }
            }
            else if (canasta == Echt)
            {
                DrawText(
                    "Canasta",
                    CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                    CARDREST_SIZE * 4 + SQUARE_SIZE * 6,
                    16,
                    RED);
                bool is_uitgelegd = false;
                for (int j = 0; j < size; ++j)
                {
                    int k = ABS(team_uitgelegd[Wij][i]->GetElementAt(j));
                    if (UITLEG(k) == i && (KLEUR(k) == Harten || KLEUR(k) == Ruiten))
                    {
                        DrawTexture(
                            img->GetImage(k),
                            CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                            CARDREST_SIZE * 4 + SQUARE_SIZE * 5,
                            RAYWHITE);
                        is_uitgelegd = true;
                        break;
                    }
                }
                if (!is_uitgelegd)
                {
                    int k = RodeKaarten[i];
                    DrawTexture(
                        img->GetImage(k),
                        CARDREST_SIZE * 2 + SQUARE_SIZE * i + SQUARE_SIZE,
                        CARDREST_SIZE * 4 + SQUARE_SIZE * 5,
                        RAYWHITE);
                }
            }
        }
    }
    if (spelstatus > Spelstart)
    {
        // teksten
        std::string delerstr = "Deler: " + SpelerNaam[deler];
        std::string beurtstr = "Beurt: " + SpelerNaam[speler_ad_beurt];
        DrawText(
            delerstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE,
            30,
            BLACK);
        DrawText(
            beurtstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_HALF_SIZE,
            30,
            BLACK);
        if (trek_kaart > 0)
        {
            DrawText(
                "Trekt: ",
                CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
                CARDREST_SIZE + SQUARE_SIZE,
                30,
                BLACK);
            DrawTexture(
                img->GetImage(trek_kaart),
                CARDREST_SIZE + SQUARE_SIZE * 16,
                CARDREST_SIZE + SQUARE_SIZE,
                RAYWHITE);
        }
        else if (afleg_kaart > 0)
        {
            DrawText(
                "Trekt: ",
                CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
                CARDREST_SIZE + SQUARE_SIZE,
                30,
                BLACK);
            DrawTexture(
                img->GetImage(afleg_kaart),
                CARDREST_SIZE + SQUARE_SIZE * 16,
                CARDREST_SIZE + SQUARE_SIZE,
                RAYWHITE);
        }
#ifndef NDEBUG
        std::string statusstr = "status: " + StatuscodeNaam[spelstatus];
        DrawText(
            statusstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 6,
            CARDREST_SIZE + SQUARE_SIZE * 4 + CARD_HALF_HEIGHT_SIZE,
            16,
            BLACK);
        std::string tellerstr = std::to_string(teller);
        DrawText(
            tellerstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 10,
            CARDREST_SIZE + SQUARE_SIZE * 4 + CARD_HALF_HEIGHT_SIZE,
            16,
            BLACK);
#endif
        if (tekstinfo != "")
            DrawText(
                tekstinfo.c_str(),
                CARDREST_SIZE + SQUARE_SIZE,
                CARDREST_SIZE + 8 * SQUARE_SIZE,
                20,
                BLACK);
        if (tekstfout != "")
            DrawText(
                tekstfout.c_str(),
                CARDREST_SIZE * 2 + SQUARE_SIZE,
                CARDREST_SIZE + 8 * SQUARE_SIZE + 20,
                20,
                RED);
        // totaal en ronde punten en min punten van Zij
        DrawText(
            TeamNaam[Zij].c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 2,
            20,
            YELLOW);
        std::string numberstr = "Min uitleg: " + std::to_string(GetMinimumPunten(Zij));
        DrawText(
            numberstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 2 + SQUARE_HALF_SIZE,
            20,
            YELLOW);
        numberstr = "Ronde punten:" + std::to_string(TeamUitlegPunten(true, Zij));
        DrawText(
            numberstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 3,
            20,
            YELLOW);
        numberstr = "Totaal punten:" + std::to_string(totaal_punten[Zij]);
        DrawText(
            numberstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 3 + SQUARE_HALF_SIZE,
            20,
            YELLOW);

        // totaal en ronde punten en min punten van Wij
        DrawText(
            TeamNaam[Wij].c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 6,
            20,
            GREEN);
        numberstr = "Min uitleg: " + std::to_string(GetMinimumPunten(Wij));
        DrawText(
            numberstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 6 + SQUARE_HALF_SIZE,
            20,
            GREEN);
        numberstr = "Ronde punten:" + std::to_string(TeamUitlegPunten(true, Wij));
        DrawText(
            numberstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 7,
            20,
            GREEN);
        numberstr = "Totaal punten:" + std::to_string(totaal_punten[Wij]);
        DrawText(
            numberstr.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 7 + SQUARE_HALF_SIZE,
            20,
            GREEN);
    }
    if (spelstatus >= Rondestop)
    {
        Color w = {211, 211, 211, 96};
        DrawRectangle(
            0,
            0,
            SCREEN_WIDTH_SIZE,
            SCREEN_HEIGHT_SIZE,
            w);
        // Ronde overzicht zij
        DrawText(
            "Ronde overzicht: ",
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 4,
            20,
            MAGENTA);
        std::string text = " uitgelegd:" + std::to_string(ABS(overzicht[Zij][0]));
        if (overzicht[Zij][0] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 4 + 20,
            20,
            MAGENTA);
        text = " bonus: " + std::to_string(ABS(overzicht[Zij][1]));
        if (overzicht[Zij][1] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 4 + 40,
            20,
            MAGENTA);
        text = " canasta: " + std::to_string(ABS(overzicht[Zij][2]));
        if (overzicht[Zij][2] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 4 + 60,
            20,
            MAGENTA);
        text = " uitgaan: " + std::to_string(ABS(overzicht[Zij][3]));
        if (overzicht[Zij][3] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 4 + 80,
            20,
            MAGENTA);
        text = " handkaarten: " + std::to_string(ABS(overzicht[Zij][4]));
        if (overzicht[Zij][4] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 4 + 100,
            20,
            MAGENTA);
        // Ronde overzicht Wij
        DrawText(
            "Ronde overzicht: ",
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 8,
            20,
            BLUE);
        text = " uitgelegd:" + std::to_string(ABS(overzicht[Wij][0]));
        if (overzicht[Wij][0] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 8 + 20,
            20,
            BLUE);
        text = " bonus: " + std::to_string(ABS(overzicht[Wij][1]));
        if (overzicht[Wij][1] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 8 + 40,
            20,
            BLUE);
        text = " canasta: " + std::to_string(ABS(overzicht[Wij][2]));
        if (overzicht[Wij][2] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 8 + 60,
            20,
            BLUE);
        text = " uitgaan: " + std::to_string(ABS(overzicht[Wij][3]));
        if (overzicht[Wij][3] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 8 + 80,
            20,
            BLUE);
        text = " handkaarten: " + std::to_string(ABS(overzicht[Wij][4]));
        if (overzicht[Wij][4] < 0)
        {
            text += "-";
        }
        DrawText(
            text.c_str(),
            CARDREST_SIZE + SQUARE_SIZE * 14 + SQUARE_HALF_SIZE,
            CARDREST_SIZE + SQUARE_SIZE * 8 + 100,
            20,
            BLUE);
    }
}
