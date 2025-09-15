#include <iostream>
#include <raylib.h>
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
const std::string version = "1.10";

// sizes
const int SCREEN_WIDTH_SIZE = 1548; // 17 vakjes + extra links en rechts 9 pixels
const int SCREEN_HEIGHT_SIZE = 918; // 10 vakjes + extra top en bodem 9 pixels
const int SQUARE_SIZE = 90;
const int HALF_SQUARE_SIZE = 45;
const int CARD_WIDTH_SIZE = 72;
const int CARD_HEIGHT_SIZE = 88;
const int VERTICALLINE_SIZE = 24;
const int CARDREST_SIZE = 9;

// const
#if defined(NDEBUG)
const int MAX_MANCHES = 16;
#else
const int MAX_MANCHES = 2;
#endif
const int NO_VALUE = -1;
const int LAATSTE_SLAG = 10;

// spelers
enum
{
	noord,
	oost,
	zuid,
	west
};

// team
enum
{
	wij,
	zij,
};

// kleuren
enum
{
	klaveren,
	harten,
	schoppen,
	ruiten,
};

// kaarten
enum
{
	k7,
	k8,
	k9,
	kt,
	kb,
	kv,
	kk,
	ka,
	h7,
	h8,
	h9,
	ht,
	hb,
	hv,
	hk,
	ha,
	s7,
	s8,
	s9,
	st,
	sb,
	sv,
	sk,
	sa,
	r7,
	r8,
	r9,
	rt,
	rb,
	rv,
	rk,
	ra,
};

// kaarttypes
enum
{
	zeven,
	acht,
	negen,
	tien,
	boer,
	vrouw,
	koning,
	aas,
};

// spel_status
enum
{
	spelstart,
	schudden,
	delen1,
	delen2,
	delen3,
	bepaaltroef,
	rondestart,
	rondespeel,
	slagenstart,
	slagenspeel,
	slagenstop,
	manchestop,
	spelstop,
};

const int troefkleuren[4] = {klaveren, harten, schoppen, ruiten};
const int puntenkaart[8] = {0, 0, 0, 10, 2, 3, 4, 11};	 // 7,8,9,10,B,V,K,A
const int puntentroef[8] = {0, 0, 14, 10, 20, 3, 4, 11}; // 7,8,9,10,B,V,K,A
const int kaartrang[8] = {1, 2, 3, 7, 4, 5, 6, 8};		 // 7,8,9,10,B,V,K,A
const int troefrang[8] = {1, 2, 7, 5, 8, 3, 4, 6};		 // 7,8,9,10,B,V,K,A
const std::string kleurnaam[4] = {"klaveren", "harten", "schoppen", "ruiten"};
const std::string spelernaam[4] = {"Noord", "Oost", "Zuid", "West"};
const std::string spelerletter[4] = {"N", "O", "Z", "W"};
const std::string teamnaam[2] = {"Wij", "Zij"};
const std::string kaartnaam[32] = {"k7", "k8", "k9", "kt", "kb", "kv", "kk", "ka",
								   "h7", "h8", "h9", "ht", "hb", "hv", "hk", "ha",
								   "s7", "s8", "s9", "st", "sb", "sv", "sk", "sa",
								   "r7", "r8", "r9", "rt", "rb", "rv", "rk", "ra"};
const std::string uitleg[30] = {
	"Klaverjassen, regels",
	"-----------------------",
	"Dit programma hanteert de volgende regels:",
	"- Om de beurt verplicht troef maken, dus spelen.",
	"- Altijd de gevraagde kleur bekennen.",
	"- Het rotterdamse systeem, dat betekend:",
	"-- als de gevraagde kleur troef is dan altijd overtroeven als dat kan.",
	"-- als de gevraagde kleur niet bekent kan worden dan is troeven verplicht indien er troeven zijn,",
	"   als er al door een ander ingetroefd is dan moet je overtroeven als dat kan, kun je dit niet dan",
	"   moet je toch troeven met een lagere troef als je die hebt.",
	"",
	"Werking van het programma",
	"-----------------------------",
	"Je speelt als zuid speler, terwijl de a.i. speelt als de andere spelers, noord, oost en west.",
	"Noord en zuid zijn een team(wij) en west en oost zijn het andere team(zij).",
	"Zodra de kaarten geschud en verdeeld zijn dan wordt er eerst de troefkleur bepaalt, de a.i bepaalt zelfstanding de troef,",
	"als het jouw beurt is om verplicht troef te maken is dan kun je op een van de kaarten klikken voor de gewenste kleur",
	"of 1 .. 8 indrukken (zie de gele cijfers boven jouw kaarten).",
	"Rechtsboven de puntenlijst naast de getoonde kaarten wordt de troef getoond en wie de troef heeft gemaakt.",
	"Daarna moeten de kaarten worden gespeeld, de a.i doet dat automatisch, als het jouw beurt is dan",
	"zie je een cijfer van 1 .. 8 boven de kaart die gespeeld kan worden, je kan dat cijfer indrukken of met de muis op de kaart te klikken.",
	"Nadat alle 4 de kaarten zijn bijgespeeld dan wordt de winnaar bepaalt en getoond in gele tekst op het scherm.",
	"en de punten worden verwerkt voor de huidige manche in het punten-overzicht (rood gekleurd), rechts van de kaarten van oost.",
	"Is er roem (zie overzicht) gevallen in de slag dan wordt dat ook getoond samen met de winnaar van de slag.",
	"Nadat er op <enter> wordt geklikt of op de enter toets wordt gedrukt dan gaat het spel verder, de winnaar van de slag komt dan uit.",
	"Zijn alle kaarten uitgespeeld dan wordt bepaalt of de troefmaker samen met zijn partner het gehaald hebben of niet(= NAT).",
	"Wanneer alle slagen zijn gehaald dan wordt er een MARS gehaald en getoond in het punten overzicht wat 100 extra roem geeft.",
	"Na 16 manches is het spel voorbij en wordt het winnende team bepaalt en wordt getoond op het scherm.",
	"Door op enter te klikken of de entertoets in te drukken kan er een nieuw spel worden opgestart.",
	"Door op de escapetoets te klikken of op het kruisje rechtsboven te klikken kan het programma worden verlaten."};
const std::string roem[13] = {
	"Roem:",
	"- 3-kaart    : 20 punten",
	"- 4-kaart    : 50 punten",
	"- stuk; heer, vrouw van",
	"   troef      : 20 punten",
	"- 4 dezelfde",
	"   plaatjes   : 100 punten",
	"3-kaart : 20 punten",
	"4-kaart : 50 punten",
	"stuk : 20 punten",
	"3-kaart + stuk : 40 punten",
	"4-kaart + stuk : 70 punten",
	"4 plaatjes : 100 punten"};

// macro's
#define KAARTTYPE(k) (k % 8)
#define KAARTKLEUR(k) ((k - KAARTTYPE(k)) / 8)
#define KAARTRANG(k) (kaartrang[KAARTTYPE(k)])
#define TROEFRANG(k) (troefrang[KAARTTYPE(k)])
#define KAARTPUNTEN(k) (puntenkaart[KAARTTYPE(k)])
#define TROEFPUNTEN(k) (puntentroef[KAARTTYPE(k)])
#define BOOL2INT(b) (b ? 1 : 0)

/// @brief Start the program
/// @return result
int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	std::string title = "Klaverjassen in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
	InitWindow(SCREEN_WIDTH_SIZE, SCREEN_HEIGHT_SIZE, title.c_str());
	Game_t *game = new Game_t();
	SetTargetFPS(10); // Set our game to run at 10 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		/* *************************************
		 * update spel                         *
		 * *************************************/
		game->Kj_update();
		/* *************************************
		 * end update spel                     *
		 * *************************************/

		/* *************************************
		 * draw gui                            *
		 * *************************************/
		BeginDrawing();
		game->Kj_draw();
		EndDrawing();
		/* *************************************
		 * end draw gui                        *
		 * *************************************/

		/* *************************************
		 * key press                           *
		 * *************************************/
		int key = GetKeyPressed(); 
		if (key > 0)
		{
			// empty buffer
			int keybuf = key;
	        while(keybuf>0){
            	keybuf = GetKeyPressed();
        	}
			game->Kj_key_handler(key);
		}
		/* *************************************
		 * end key press                       *
		 * *************************************/

		/* *************************************
		 * mouse button press                  *
		 * *************************************/
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			int x = GetMouseX();
			int y = GetMouseY();
			game->Kj_mouse_handler(x, y);
		}
		/* *************************************
		 * end mouse button press              *
		 * *************************************/
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	delete game;
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

/// @brief Constructor
Game_t::Game_t()
{
	Load_Images();
	Kj_init_spel();
}

/// @brief Destructor
Game_t::~Game_t()
{
	Unload_Images();
}

/// @brief Load the Images
void Game_t::Load_Images()
{
	speeltafel = LoadTexture("./afbeeldingen/speeltafel.png");
	speeltafel.width = SCREEN_WIDTH_SIZE;
	speeltafel.height = SCREEN_HEIGHT_SIZE;
	troefkaarten[klaveren] = LoadTexture("./afbeeldingen/k2.png");
	troefkaarten[harten] = LoadTexture("./afbeeldingen/h2.png");
	troefkaarten[schoppen] = LoadTexture("./afbeeldingen/s2.png");
	troefkaarten[ruiten] = LoadTexture("./afbeeldingen/r2.png");
	speelkaarten[k7] = LoadTexture("./afbeeldingen/k7.png");
	speelkaarten[k8] = LoadTexture("./afbeeldingen/k8.png");
	speelkaarten[k9] = LoadTexture("./afbeeldingen/k9.png");
	speelkaarten[kt] = LoadTexture("./afbeeldingen/kt.png");
	speelkaarten[kb] = LoadTexture("./afbeeldingen/kb.png");
	speelkaarten[kv] = LoadTexture("./afbeeldingen/kv.png");
	speelkaarten[kk] = LoadTexture("./afbeeldingen/kk.png");
	speelkaarten[ka] = LoadTexture("./afbeeldingen/ka.png");
	speelkaarten[h7] = LoadTexture("./afbeeldingen/h7.png");
	speelkaarten[h8] = LoadTexture("./afbeeldingen/h8.png");
	speelkaarten[h9] = LoadTexture("./afbeeldingen/h9.png");
	speelkaarten[ht] = LoadTexture("./afbeeldingen/ht.png");
	speelkaarten[hb] = LoadTexture("./afbeeldingen/hb.png");
	speelkaarten[hv] = LoadTexture("./afbeeldingen/hv.png");
	speelkaarten[hk] = LoadTexture("./afbeeldingen/hk.png");
	speelkaarten[ha] = LoadTexture("./afbeeldingen/ha.png");
	speelkaarten[s7] = LoadTexture("./afbeeldingen/s7.png");
	speelkaarten[s8] = LoadTexture("./afbeeldingen/s8.png");
	speelkaarten[s9] = LoadTexture("./afbeeldingen/s9.png");
	speelkaarten[st] = LoadTexture("./afbeeldingen/st.png");
	speelkaarten[sb] = LoadTexture("./afbeeldingen/sb.png");
	speelkaarten[sv] = LoadTexture("./afbeeldingen/sv.png");
	speelkaarten[sk] = LoadTexture("./afbeeldingen/sk.png");
	speelkaarten[sa] = LoadTexture("./afbeeldingen/sa.png");
	speelkaarten[r7] = LoadTexture("./afbeeldingen/r7.png");
	speelkaarten[r8] = LoadTexture("./afbeeldingen/r8.png");
	speelkaarten[r9] = LoadTexture("./afbeeldingen/r9.png");
	speelkaarten[rt] = LoadTexture("./afbeeldingen/rt.png");
	speelkaarten[rb] = LoadTexture("./afbeeldingen/rb.png");
	speelkaarten[rv] = LoadTexture("./afbeeldingen/rv.png");
	speelkaarten[rk] = LoadTexture("./afbeeldingen/rk.png");
	speelkaarten[ra] = LoadTexture("./afbeeldingen/ra.png");
	kaart_achtergrond = LoadTexture("./afbeeldingen/ba.png");
	enter_button = LoadTexture("./afbeeldingen/Enter.png");
	enter_button.width = 2 * SQUARE_SIZE;
	enter_button.height = SQUARE_SIZE;
	pijlen[noord] = LoadTexture("./afbeeldingen/pijlomhoog.png");
	pijlen[zuid] = LoadTexture("./afbeeldingen/pijlomlaag.png");
	pijlen[oost] = LoadTexture("./afbeeldingen/pijlrechts.png");
	pijlen[west] = LoadTexture("./afbeeldingen/pijllinks.png");
	dir = LoadTexture("./afbeeldingen/dir.png");
	dir.width = 4 * SQUARE_SIZE;
	dir.height = 4 * SQUARE_SIZE;
	for (int i = 0; i < 32; ++i)
	{
		speelkaarten[i].width = CARD_WIDTH_SIZE;
		speelkaarten[i].height = CARD_HEIGHT_SIZE;
		if (i < 4)
		{
			troefkaarten[i].width = CARD_WIDTH_SIZE;
			troefkaarten[i].height = CARD_HEIGHT_SIZE;
			pijlen[i].width = SQUARE_SIZE;
			pijlen[i].height = SQUARE_SIZE;
		}
	}
	kaart_achtergrond.width = CARD_WIDTH_SIZE;
	kaart_achtergrond.height = CARD_HEIGHT_SIZE;
}

void Game_t::Unload_Images() const
{
	UnloadTexture(speeltafel);
	for (int i = 0; i < 32; ++i)
	{
		UnloadTexture(speelkaarten[i]);
		if (i < 4)
		{
			UnloadTexture(troefkaarten[i]);
			UnloadTexture(pijlen[i]);
		}
	}
	UnloadTexture(kaart_achtergrond);
	UnloadTexture(enter_button);
	UnloadTexture(dir);
}

/// @brief Initialiseer het spel
void Game_t::Kj_init_spel()
{
	teller = 0;
	manche_teller = 0;
	for (int i = 0; i < 16; ++i)
	{
		manche_punten[wij][i] = manche_punten[zij][i] = 0;
		roem_punten[wij][i] = roem_punten[zij][i] = 0;
	}
	spel_continue = false;
	totaal_punten[wij] = totaal_punten[zij] = 0;
	deler = GetRandomValue(noord, west);
	Kj_init_manche();
	spel_status = spelstart;
}

/// @brief Initialiseer een manche
void Game_t::Kj_init_manche()
{
	++deler;
	if (deler > west)
	{
		deler = noord;
	}
	Kj_vul_deel_kaarten();
	slagen_gewonnen[wij] = slagen_gewonnen[zij] = 0;
	slagen_teller = 0;
	troef_bepaler = deler + 1;
	if (troef_bepaler > west)
	{
		troef_bepaler = noord;
	}
	troef = NO_VALUE;
	for (int i = 0; i < 8; ++i)
	{
		slag_kleur_gevraagd[i] = NO_VALUE;
		slag_beginner[i] = NO_VALUE;
		slag_ad_beurt[i] = NO_VALUE;
		slag_winnaar[i] = NO_VALUE;
	}
	for (int s = noord; s <= west; ++s)
	{
		for (int i = 0; i < 8; ++i)
		{
			speler_kaarten[s][i] = NO_VALUE;
		}
	}
	for (int s = noord; s <= west; ++s)
	{
		for (int klr = klaveren; klr <= ruiten; ++klr)
		{
			speler_heeft_kleur[s][klr] = true;
		}
	}
	slag_beginner[slagen_teller] = troef_bepaler;
	slag_ad_beurt[slagen_teller] = troef_bepaler;
	is_nat[wij] = is_nat[zij] = false;
	is_mars[wij] = is_mars[zij] = false;
	gemaakt[wij] = gemaakt[zij] = false;
	gespeeld[wij] = troef_bepaler == noord || troef_bepaler == zuid;
	gespeeld[zij] = troef_bepaler == oost || troef_bepaler == west;
	manche_continue = false;
	manche_berekend = false;
	Kj_init_slag();
	spel_status = schudden;
}

/// @brief Initaliseer een slag
void Game_t::Kj_init_slag()
{
	tel_speler_gespeeld = 0;
	for (int i = 0; i < 4; ++i)
	{
		tafel_kaarten[i] = NO_VALUE;
	}
	roemgemaakt = false;
	slag_berekend = false;
	slag_continue = false;
	spel_status = slagenstart;
}

/// @brief Update de gui
void Game_t::Kj_update()
{
	if (spel_status >= bepaaltroef)
	{
		for (int s = noord; s <= west; ++s)
		{
			Kj_sorteer_hand(s);
		}
	}
	++teller;
	if (teller % 5 != 0)
	{
		return;
	}
	else if (spel_status == schudden)
	{
		Kj_kaarten_schudden();
		spel_status = delen1;
	}
	else if (spel_status == delen1)
	{
		Kj_kaarten_delen1();
		spel_status = delen2;
	}
	else if (spel_status == delen2)
	{
		Kj_kaarten_delen2();
		spel_status = delen3;
	}
	else if (spel_status == delen3)
	{
		Kj_kaarten_delen3();
		spel_status = bepaaltroef;
	}
	else if (spel_status == bepaaltroef && troef_bepaler != zuid)
	{
		Kj_bepaal_troef_ai(troef_bepaler);
		spel_status = rondestart;
	}
	else if (spel_status == rondestart)
	{
		spel_status = rondespeel;
	}
	else if (spel_status == rondespeel)
	{
		spel_status = slagenstart;
	}
	else if (spel_status == slagenstart)
	{
		Kj_init_slag();
		spel_status = slagenspeel;
	}
	else if (spel_status == slagenspeel)
	{
		if (tel_speler_gespeeld < 4)
		{
			Kj_bepaal_mag_spelen(slag_ad_beurt[slagen_teller]);
			if (slag_ad_beurt[slagen_teller] != zuid)
			{
				Kj_speler_beurt(slag_ad_beurt[slagen_teller]);
			}
		}
		else
		{
			spel_status = slagenstop;
		}
	}
	else if (spel_status == slagenstop)
	{
		if (!slag_berekend)
		{
			Kj_bepaal_winnaar_slag();
			slag_berekend = true;
		}
		if (slag_continue)
		{
			tel_speler_gespeeld = 0;
			++slagen_teller;
			if (slagen_teller < 8)
			{
				slag_beginner[slagen_teller] = slag_winnaar[slagen_teller - 1];
				slag_ad_beurt[slagen_teller] = slag_winnaar[slagen_teller - 1];
				for (int s = noord; s <= west; ++s)
				{
					Kj_verwijder_kaart(tafel_kaarten[s]);
				}
				Kj_update_speler_heeft_kleuren();
				spel_status = slagenstart;
			}
			else
			{
				spel_status = manchestop;
			}
		}
	}
	else if (spel_status == manchestop)
	{
		if (!manche_berekend)
		{
			Kj_bepaal_resultaat_manche();
			manche_berekend = true;
		}
		if (manche_continue)
		{
			++manche_teller;
			if (manche_teller < MAX_MANCHES)
			{
				Kj_init_manche();
				spel_status = schudden;
			}
			else
			{
				spel_status = spelstop;
			}
		}
	}
	else if (spel_status == spelstop)
	{
		if (spel_continue)
		{
			Kj_init_spel();
			spel_status = spelstart;
		}
	}
}

/// @brief Draw het spel
void Game_t::Kj_draw()
{
	ClearBackground(RAYWHITE);
	Color light = {238, 221, 130, 128};
	std::string nummertxt;
	DrawTexture(speeltafel, 0, 0, RAYWHITE);
	if (spel_status == spelstart)
	{
		DrawRectangle(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE, 16 * SQUARE_SIZE, VERTICALLINE_SIZE * 31, light);
		for (int i = 0; i < 30; ++i)
			DrawText(uitleg[i].c_str(), CARDREST_SIZE + HALF_SQUARE_SIZE, VERTICALLINE_SIZE * i + CARDREST_SIZE + HALF_SQUARE_SIZE, 20, DARKPURPLE);
		for (int i = 0; i < 7; ++i)
			DrawText(roem[i].c_str(), SCREEN_WIDTH_SIZE - SQUARE_SIZE * 4, VERTICALLINE_SIZE * (i + 1) + CARDREST_SIZE + HALF_SQUARE_SIZE, 20, DARKBLUE);
		DrawTexture(enter_button, SQUARE_SIZE * 10 + CARDREST_SIZE, SCREEN_HEIGHT_SIZE - SQUARE_SIZE - CARDREST_SIZE, RAYWHITE);
		DrawText("Druk of click met de muis op ENTER", SQUARE_SIZE * 12 + CARDREST_SIZE, SCREEN_HEIGHT_SIZE - HALF_SQUARE_SIZE - CARDREST_SIZE, 20, GREEN);
		return;
	}
	DrawTexture(dir, 3 * SQUARE_SIZE + CARDREST_SIZE, 3 * SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);

	// teken kaarten
	for (int i = 0; i < 8; ++i)
	{
		int k = speler_kaarten[noord][i];
		if (k > NO_VALUE)
		{
#ifndef NDEBUG
			DrawTexture(speelkaarten[k],
						SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE, CARDREST_SIZE, RAYWHITE);
#else
			DrawTexture(kaart_achtergrond,
						SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE, CARDREST_SIZE, RAYWHITE);
#endif
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		int k = speler_kaarten[zuid][i];
		if (k > NO_VALUE)
		{
			DrawTexture(speelkaarten[k], SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 9 + CARDREST_SIZE, RAYWHITE);
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		int k = speler_kaarten[west][i];
		if (k > NO_VALUE)
		{
#ifndef NDEBUG
			DrawTexture(speelkaarten[k],
						CARDREST_SIZE, SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
#else
			DrawTexture(kaart_achtergrond,
						CARDREST_SIZE, SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
#endif
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		int k = speler_kaarten[oost][i];
		if (k > NO_VALUE)
		{
#ifndef NDEBUG
			DrawTexture(speelkaarten[k],
						SQUARE_SIZE * 9 + CARDREST_SIZE, SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
#else
			DrawTexture(kaart_achtergrond,
						SQUARE_SIZE * 9 + CARDREST_SIZE, SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
#endif
		}
	}
	// teken deel kaarten
	if (spel_status < bepaaltroef)
	{
		switch (deler)
		{
		case noord:
			DrawTexture(kaart_achtergrond, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		case oost:
			DrawTexture(kaart_achtergrond, SQUARE_SIZE * 7 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		case zuid:
			DrawTexture(kaart_achtergrond, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 7 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		case west:
			DrawTexture(kaart_achtergrond, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		}
	}
	// teken punten lijst
	int kladblokcol = SQUARE_SIZE * 11 + CARDREST_SIZE;
	int kladblokrij = SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE;

	DrawRectangle(kladblokcol, kladblokrij, SQUARE_SIZE * 4, SQUARE_SIZE * 7, light);
	DrawLine(kladblokcol + SQUARE_SIZE * 2, kladblokrij, kladblokcol + SQUARE_SIZE * 2, kladblokrij + SQUARE_SIZE * 7, BLACK);
	DrawText("Wij(N/Z)", kladblokcol + HALF_SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE, 20, DARKBLUE);
	DrawText("Zij(W/O)", kladblokcol + SQUARE_SIZE * 2 + HALF_SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE, 20, DARKBLUE);
	DrawText(" Punten  Roem", kladblokcol, kladblokrij + VERTICALLINE_SIZE * 2, 20, DARKBLUE);
	DrawText(" Punten  Roem", kladblokcol + SQUARE_SIZE * 2, kladblokrij + VERTICALLINE_SIZE * 2, 20, DARKBLUE);
	DrawLine(kladblokcol, kladblokrij + VERTICALLINE_SIZE * 3, kladblokcol + 4 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * 3, BLACK);
	for (int i = 0; i < 16; ++i)
	{
		Color clr = i == manche_teller ? RED
									   : BLACK;
		nummertxt = std::to_string(manche_punten[wij][i]);
		if (manche_punten[wij][i] == 0)
		{
			DrawText("-", kladblokcol + VERTICALLINE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else if (manche_punten[wij][i] == -1)
		{
			DrawText("NAT", kladblokcol + VERTICALLINE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else if (manche_punten[wij][i] == -2)
		{
			DrawText("MARS", kladblokcol + VERTICALLINE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else
		{
			DrawText(nummertxt.c_str(), kladblokcol + VERTICALLINE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		nummertxt = std::to_string(roem_punten[wij][i]);
		if (roem_punten[wij][i] <= 0)
		{
			DrawText(" ", kladblokcol + VERTICALLINE_SIZE + SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else
		{
			DrawText(nummertxt.c_str(), kladblokcol + VERTICALLINE_SIZE + SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		nummertxt = std::to_string(manche_punten[zij][i]);
		if (manche_punten[zij][i] == 0)
		{
			DrawText("-", kladblokcol + VERTICALLINE_SIZE + 2 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else if (manche_punten[zij][i] == -1)
		{
			DrawText("NAT", kladblokcol + VERTICALLINE_SIZE + 2 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else if (manche_punten[zij][i] == -2)
		{
			DrawText("MARS", kladblokcol + VERTICALLINE_SIZE + 2 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else
		{
			DrawText(nummertxt.c_str(), kladblokcol + VERTICALLINE_SIZE + 2 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		nummertxt = std::to_string(roem_punten[zij][i]);
		if (roem_punten[zij][i] <= 0)
		{
			DrawText(" ", kladblokcol + VERTICALLINE_SIZE + 3 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
		else
		{
			DrawText(nummertxt.c_str(), kladblokcol + VERTICALLINE_SIZE + 3 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * (i + 4), 20, clr);
		}
	}
	DrawLine(kladblokcol, kladblokrij + VERTICALLINE_SIZE * 21, kladblokcol + 4 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * 21, BLACK);
	DrawText("Totaal", kladblokcol + HALF_SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * 22, 20, DARKBLUE);
	DrawText("Totaal", kladblokcol + HALF_SQUARE_SIZE + 2 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * 22, 20, DARKBLUE);
	DrawLine(kladblokcol, kladblokrij + VERTICALLINE_SIZE * 23, kladblokcol + 4 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * 23, BLACK);
	nummertxt = std::to_string(totaal_punten[wij]);
	DrawText(nummertxt.c_str(), kladblokcol + HALF_SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * 24, 20, DARKBLUE);
	nummertxt = std::to_string(totaal_punten[zij]);
	DrawText(nummertxt.c_str(), kladblokcol + HALF_SQUARE_SIZE + 2 * SQUARE_SIZE, kladblokrij + VERTICALLINE_SIZE * 24, 20, DARKBLUE);
	// teken aan het einde van de slag
	if (spel_status >= slagenstop)
	{
		switch (slag_winnaar[slagen_teller])
		{
		case noord:
			if (roemgemaakt)
			{
				DrawText("Noord wint de slag met roem", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
				DrawText(roem[tekst_roem].c_str(), SQUARE_SIZE * 5 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			else
			{
				DrawText("Noord wint de slag", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			break;
		case oost:
			if (roemgemaakt)
			{
				DrawText("Oost wint de slag met roem", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
				DrawText(roem[tekst_roem].c_str(), SQUARE_SIZE * 5 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			else
			{
				DrawText("Oost wint de slag", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			break;
		case zuid:
			if (roemgemaakt)
			{
				DrawText("Zuid wint de slag met roem", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
				DrawText(roem[tekst_roem].c_str(), SQUARE_SIZE * 5 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			else
			{
				DrawText("Zuid wint de slag", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			break;
		case west:
			if (roemgemaakt)
			{
				DrawText("West wint de slag met roem", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
				DrawText(roem[tekst_roem].c_str(), SQUARE_SIZE * 5 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			else
			{
				DrawText("West wint de slag", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			}
			break;
		}
		if (gespeeld[wij])
		{
			if (is_nat[wij])
			{
				DrawText("Wij zijn nat", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE, 20, YELLOW);
			}
			else if (is_mars[wij])
			{
				DrawText("Wij hebben een mars gehaald", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE, 20, YELLOW);
			}
			else if (gemaakt[wij])
			{
				DrawText("Wij hebben gemaakt", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE, 20, YELLOW);
			}
		}
		else
		{
			if (is_nat[zij])
			{
				DrawText("Zij zijn nat", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE, 20, YELLOW);
			}
			else if (is_mars[zij])
			{
				DrawText("Zij hebben een mars gehaald", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE, 20, YELLOW);
			}
			else if (gemaakt[zij])
			{
				DrawText("Zij hebben gemaakt", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE, 20, YELLOW);
			}
		}

		if (manche_teller == MAX_MANCHES && slagen_teller == 8)
		{
			if (totaal_punten[wij] > totaal_punten[zij])
			{
				DrawText("Wij hebben het spel gewonnen!", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE * 2, 20, YELLOW);
			}
			else if (totaal_punten[zij] > totaal_punten[wij])
			{
				DrawText("Zij hebben het spel gewonnen!", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE * 2, 20, YELLOW);
			}
			else
			{
				DrawText("Er is geen winnaar van het spel!", SQUARE_SIZE + CARDREST_SIZE * 2, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE + VERTICALLINE_SIZE * 2, 20, YELLOW);
			}
			DrawText("Einde spel, druk op F5 om opnieuw te spelen", SQUARE_SIZE * 10 + CARDREST_SIZE, SCREEN_HEIGHT_SIZE - HALF_SQUARE_SIZE - CARDREST_SIZE, 20, GREEN);
		}
		else if (slagen_teller == 8)
		{
			DrawText("Einde manche, druk op F6 om verder te gaan", SQUARE_SIZE * 10 + CARDREST_SIZE, SCREEN_HEIGHT_SIZE - HALF_SQUARE_SIZE - CARDREST_SIZE, 20, GREEN);
		}
		else
		{
			DrawText("Einde slag, druk op F7 om verder te gaan", SQUARE_SIZE * 10 + CARDREST_SIZE, SCREEN_HEIGHT_SIZE - HALF_SQUARE_SIZE - CARDREST_SIZE, 20, GREEN);
		}
	}
	if (troef >= 0)
	{
		DrawTexture(troefkaarten[troef], SQUARE_SIZE * 11 + CARDREST_SIZE, CARDREST_SIZE, RAYWHITE);
		switch (troef_bepaler)
		{
		case noord:
			DrawText("troef bepaalt door Noord.", SQUARE_SIZE * 12 + CARDREST_SIZE, HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			break;
		case oost:
			DrawText("troef bepaalt door Oost.", SQUARE_SIZE * 12 + CARDREST_SIZE, HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			break;
		case zuid:
			DrawText("troef bepaalt door Zuid.", SQUARE_SIZE * 12 + CARDREST_SIZE, HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
			break;
		case west:
			DrawText("troef bepaalt door West.", SQUARE_SIZE * 12 + CARDREST_SIZE, HALF_SQUARE_SIZE + CARDREST_SIZE, 20, YELLOW);
		}
	}
	if (spel_status == slagenspeel)
	{
		switch (slag_ad_beurt[slagen_teller])
		{
		case noord:
			DrawTexture(pijlen[noord], SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		case oost:
			DrawTexture(pijlen[oost], SQUARE_SIZE * 7 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		case zuid:
			DrawTexture(pijlen[zuid], SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 7 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		case west:
			DrawTexture(pijlen[west], SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
			break;
		}
	}
	if (spel_status >= slagenspeel)
	{
		int k = tafel_kaarten[noord];
		if (k > NO_VALUE)
		{
			DrawTexture(speelkaarten[k], SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
		}
		k = tafel_kaarten[zuid];
		if (k > NO_VALUE)
		{
			DrawTexture(speelkaarten[k], SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 7 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
		}
		k = tafel_kaarten[west];
		if (k > NO_VALUE)
		{
			DrawTexture(speelkaarten[k], SQUARE_SIZE + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
		}
		k = tafel_kaarten[oost];
		if (k > NO_VALUE)
		{
			DrawTexture(speelkaarten[k], SQUARE_SIZE * 7 + HALF_SQUARE_SIZE + CARDREST_SIZE, SQUARE_SIZE * 4 + HALF_SQUARE_SIZE + CARDREST_SIZE, RAYWHITE);
		}
	}
	if (spel_status == bepaaltroef && troef_bepaler == zuid)
	{
		for (int i = 0; i < 8; ++i)
		{
			nummertxt = std::to_string(i + 1);
			DrawText(nummertxt.c_str(), SQUARE_SIZE * i + SQUARE_SIZE + CARD_WIDTH_SIZE / 2 + CARDREST_SIZE, SQUARE_SIZE * 9 + CARDREST_SIZE - VERTICALLINE_SIZE, 20, YELLOW);
		}
		DrawText("Kies troef, druk op: 1..8", SQUARE_SIZE * 11, SCREEN_HEIGHT_SIZE - 60, 20, GREEN);
		DrawText("of click met de muis op een kaart", SQUARE_SIZE * 11, SCREEN_HEIGHT_SIZE - 30, 20, GREEN);
	}
	if (slag_ad_beurt[slagen_teller] == zuid && spel_status == slagenspeel)
	{
		for (int i = 0; i < 8; ++i)
		{
			nummertxt = std::to_string(i + 1);
			if (kaarten_mag_spelen[i] > NO_VALUE)
				DrawText(nummertxt.c_str(), SQUARE_SIZE * i + SQUARE_SIZE + CARD_WIDTH_SIZE / 2 + CARDREST_SIZE, SQUARE_SIZE * 9 + CARDREST_SIZE - VERTICALLINE_SIZE, 20, YELLOW);
			DrawText("Kies kaart, druk op 1..8 of click met de muis op de kaart", SQUARE_SIZE * 10, SCREEN_HEIGHT_SIZE - 40, 20, GREEN);
		}
	}
	std::string tellerstr = std::to_string(teller);
	DrawText(tellerstr.c_str(), 1, 1, 20, RED);
}

/// @brief Toets afhandeling
/// @param key Keycode
void Game_t::Kj_key_handler(int key)
{
	int k;
	switch (key)
	{
	case KEY_ONE:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][0];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[0] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[0] = speler_kaarten[zuid][0];
			Al_speel_kaart_bij(zuid, 0);
		}
		break;
	case KEY_TWO:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][1];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[1] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[1] = speler_kaarten[zuid][1];
			Al_speel_kaart_bij(zuid, 1);
		}
		break;
	case KEY_THREE:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][2];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[2] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[2] = speler_kaarten[zuid][2];
			Al_speel_kaart_bij(zuid, 2);
		}
		break;
	case KEY_FOUR:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][3];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[3] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[3] = speler_kaarten[zuid][3];
			Al_speel_kaart_bij(zuid, 3);
		}
		break;
	case KEY_FIVE:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][4];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[4] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[4] = speler_kaarten[zuid][4];
			Al_speel_kaart_bij(zuid, 4);
		}
		break;

	case KEY_SIX:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][5];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[5] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[5] = speler_kaarten[zuid][5];
			Al_speel_kaart_bij(zuid, 5);
		}
		break;
	case KEY_SEVEN:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][6];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[6] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[6] = speler_kaarten[zuid][6];
			Al_speel_kaart_bij(zuid, 6);
		}
		break;
	case KEY_EIGHT:
		if (troef_bepaler == zuid && spel_status == bepaaltroef)
		{
			k = speler_kaarten[zuid][7];
			troef = KAARTKLEUR(k);
			spel_status = rondestart;
		}
		else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid && kaarten_mag_spelen[7] > NO_VALUE)
		{
#ifndef NDEBUG
			printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
			kaarten_mag_spelen[7] = speler_kaarten[zuid][7];
			Al_speel_kaart_bij(zuid, 7);
		}
		break;
	case KEY_ENTER:
		if (spel_status == spelstart)
		{
			spel_status = schudden;
		}
		break;
	case KEY_F5:
		if (spel_status == spelstop)
		{
			spel_continue = true;
		}
		break;
	case KEY_F6:
		if (spel_status == manchestop)
		{
			manche_continue = true;
		}
		break;
	case KEY_F7:
		if (spel_status == slagenstop)
		{
			slag_continue = true;
		}
		break;
	}
}

/// @brief Afhandelen muis klik
/// @param x Column
/// @param y Row
void Game_t::Kj_mouse_handler(int x, int y)
{
	int k;
	if (troef_bepaler == zuid && spel_status == bepaaltroef)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (x >= SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE && x <= SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE + CARD_WIDTH_SIZE &&
				y >= SQUARE_SIZE * 9 + CARDREST_SIZE && y <= SQUARE_SIZE * 9 + CARDREST_SIZE + CARD_HEIGHT_SIZE)
			{
				k = speler_kaarten[zuid][i];
				troef = KAARTKLEUR(k);
				spel_status = rondestart;
				break;
			}
		}
	}
	else if (spel_status == slagenspeel && slag_ad_beurt[slagen_teller] == zuid)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (x >= SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE && x <= SQUARE_SIZE * i + SQUARE_SIZE + CARDREST_SIZE + CARD_WIDTH_SIZE &&
				y >= SQUARE_SIZE * 9 + CARDREST_SIZE && y <= SQUARE_SIZE * 9 + CARDREST_SIZE + CARD_HEIGHT_SIZE && kaarten_mag_spelen[i] > NO_VALUE)
			{
#ifndef NDEBUG
				printf("%d Speler %d (%s) speelt kaart bij\n", __LINE__, tel_speler_gespeeld + 1, spelernaam[zuid].c_str());
#endif
				kaarten_mag_spelen[i] = speler_kaarten[zuid][i];
				Al_speel_kaart_bij(zuid, i);
				break;
			}
		}
	}
	else if (spel_status == spelstart)
	{
		if (x >= SQUARE_SIZE * 10 && x < SQUARE_SIZE * 12 - CARDREST_SIZE && y >= SCREEN_HEIGHT_SIZE - SQUARE_SIZE - CARDREST_SIZE && y < SCREEN_HEIGHT_SIZE - CARDREST_SIZE)
		{
			spel_status = schudden;
		}
	}
}

/// @brief sorteer een hand
/// @param eSpeler 
void Game_t::Kj_sorteer_hand(int speler)
{
	int kaartwaarde[8] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int kl, ha, sc, ru;
	switch (troef)
	{
	case klaveren:
		kl = 400000;
		ha = 300000;
		sc = 200000;
		ru = 100000;
		break;
	case harten:
		ha = 400000;
		sc = 300000;
		ru = 200000;
		kl = 100000;
		break;
	case schoppen:
		sc = 400000;
		ru = 300000;
		kl = 200000;
		ha = 100000;
		break;
	case ruiten:
		ru = 400000;
		kl = 300000;
		ha = 200000;
		sc = 100000;
		break;
	default:
		kl = 400000;
		ha = 300000;
		sc = 200000;
		ru = 100000;
		break;
	}

	for (int i = 0; i < 8; ++i)
	{
		int k = speler_kaarten[speler][i];
		if (k > NO_VALUE)
		{
			int klr = KAARTKLEUR(k);
			switch (klr)
			{
			case klaveren:
				kaartwaarde[i] = klr == troef ? kl + TROEFRANG(k) : kl + KAARTRANG(k);
				break;
			case harten:
				kaartwaarde[i] = klr == troef ? ha + TROEFRANG(k) : ha + KAARTRANG(k);
				break;
			case schoppen:
				kaartwaarde[i] = klr == troef ? sc + TROEFRANG(k) : sc + KAARTRANG(k);
				break;
			case ruiten:
				kaartwaarde[i] = klr == troef ? ru + TROEFRANG(k) : ru + KAARTRANG(k);
				break;
			}
		}
		else
			{kaartwaarde[i] = NO_VALUE;}
	}
	int max, tmp;
	for (int i = 0; i < 7; ++i)
	{
		max = i;
		for (int j = i + 1; j < 8; ++j)
		{
			if (kaartwaarde[j] > kaartwaarde[max])
			{
				max = j;
			}
		}
		tmp = kaartwaarde[i];
		kaartwaarde[i] = kaartwaarde[max];
		kaartwaarde[max] = tmp;
		tmp = speler_kaarten[speler][i];
		speler_kaarten[speler][i] = speler_kaarten[speler][max];
		speler_kaarten[speler][max] = tmp;
	}
}

/// @brief Vul de array's met de beschikbare kaarten
void Game_t::Kj_vul_deel_kaarten()
{
	for (int i = 0; i < 32; ++i)
	{
		deel_kaarten[i] = i;
		spel_kaarten[i] = i;
	}
}

/// @brief Schud de kaarten
void Game_t::Kj_kaarten_schudden()
{
	for (int n = 0; n < 3; ++n)
	{
		for (int i = 0; i < 32; ++i)
		{
			int rnd = GetRandomValue(0, 31);
			int tmp = deel_kaarten[i];
			deel_kaarten[i] = deel_kaarten[rnd];
			deel_kaarten[rnd] = tmp;
		}
	}
}

/// @brief Deel de eerste 3 kaarten
void Game_t::Kj_kaarten_delen1()
{
	int ontvanger = deler;
	for (int n = 0; n < 4; ++n)
	{
		ontvanger++;
		if (ontvanger > west)
		{
			ontvanger = noord;
		}
		for (int i = 0; i < 3; ++i)
		{
			int tmp = ontvanger * 8 + i;
			speler_kaarten[ontvanger][i] = deel_kaarten[tmp];
		}
		Kj_sorteer_hand(ontvanger);
	}
}

/// @brief Deel de volgende 2 kaarten
void Game_t::Kj_kaarten_delen2()
{
	int ontvanger = deler;
	for (int n = 0; n < 4; n++)
	{
		++ontvanger;
		if (ontvanger > west)
		{
			ontvanger = noord;
		}
		for (int i = 3; i < 5; ++i)
		{
			int tmp = ontvanger * 8 + i;
			speler_kaarten[ontvanger][i] = deel_kaarten[tmp];
		}
		Kj_sorteer_hand(ontvanger);
	}
}

/// @brief Deel de laatste 3 kaarten
void Game_t::Kj_kaarten_delen3()
{
	int ontvanger = deler;
	// delen 3
	for (int n = 0; n < 4; ++n)
	{
		++ontvanger;
		if (ontvanger > west)
		{
			ontvanger = noord;
		}
		for (int i = 5; i < 8; ++i)
		{
			int tmp = ontvanger * 8 + i;
			speler_kaarten[ontvanger][i] = deel_kaarten[tmp];
		}
		Kj_sorteer_hand(ontvanger);
	}
}

/// @brief De AI bepaalt de troef kleur
/// @param speler Noord, Oost of West
void Game_t::Kj_bepaal_troef_ai(int speler)
{
	int telkleuren[4] = {0, 0, 0, 0};
	int troefwaarde[4] = {0, 0, 0, 0};
	int overigewaarde[4] = {0, 0, 0, 0};
	int totaal[4] = {0, 0, 0, 0};

	// analyseer de kaarten in de hand
	// tel voor elke kleur het aantal kaarten, de troefpunten en overigepunten (niet zijnde troef)
	for (int klr = klaveren; klr <= ruiten; ++klr)
	{
		for (int i = 0; i < 8; ++i)
		{
			int k = speler_kaarten[speler][i];
			if (k > NO_VALUE)
			{
				++telkleuren[KAARTKLEUR(k)];
				if (KAARTKLEUR(k) == klr) // troefkleur
				{
					troefwaarde[klr] += TROEFPUNTEN(k);
				}
				else
				{
					overigewaarde[klr] += KAARTPUNTEN(k);
				}
			}
		}
	}

	// tel de waarden voor elke kleur op plus het kwadraat van het aantal kaarten in de kleur
	for (int i = 0; i < 4; ++i)
	{
		totaal[i] = troefwaarde[i] + overigewaarde[i] + telkleuren[i] * telkleuren[i];
	}

	// bepaal de kleur met de meeste punten en maak deze troef
	troef = klaveren;
	for (int klr = harten; klr <= ruiten; ++klr)
	{
		if (totaal[klr] > totaal[troef])
		{
			troef = klr;
		}
		else if (totaal[klr] == totaal[troef] && GetRandomValue(0, 9) >= 5)
		{
			troef = klr;
		}
	}
}

/// @brief Verwijder de kaart uit het spel
/// @param k index kaart
void Game_t::Kj_verwijder_kaart(int k)
{
#ifndef NDEBUG
	ASSERT(k > NO_VALUE);
#endif
	spel_kaarten[k] = NO_VALUE;
}

/// @brief Bepaal de kaarten die gespeeld mogen worden
/// @param speler Noord, Oost, Zuid of West
void Game_t::Kj_bepaal_mag_spelen(int speler)
{
	// initialise
	for (int i = 0; i < 8; ++i)
	{
		kaarten_mag_spelen[i] = NO_VALUE;
	}
	// als de speler begint of bij de laatste slag mogen alle mogelijke kaarten gespeeld worden
	if (slag_beginner[slagen_teller] == speler || slagen_teller == 7)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (speler_kaarten[speler][i] > NO_VALUE)
			{
				kaarten_mag_spelen[i] = speler_kaarten[speler][i];
			}
		}
	}
	else
	{
		int k;
		int kleuren[8] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
		int troefwaarde[8] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
		int telkleuren[4] = {0, 0, 0, 0};

		for (int i = 0; i < 8; ++i)
		{
			k = speler_kaarten[speler][i];
			if (k > NO_VALUE)
			{
				kleuren[i] = KAARTKLEUR(k);
				++telkleuren[KAARTKLEUR(k)];
				if (kleuren[i] == troef)
				{
					troefwaarde[i] = TROEFRANG(k);
				}
			}
		}

		// bepaalde de hogere troefwaarde als die gespeeld is
		int hogeretroefwaarde = NO_VALUE;
		for (int i = 0; i < 4; ++i)
		{
			if (tafel_kaarten[i] > NO_VALUE)
			{
				k = tafel_kaarten[i];
				if (KAARTKLEUR(k) == troef && TROEFRANG(k) > hogeretroefwaarde)
				{
					hogeretroefwaarde = TROEFRANG(k);
				}
			}
		}

		speelkleur = slag_kleur_gevraagd[slagen_teller];
		// bepaal de mogelijke slagen indien troef gevraagd is
		if (speelkleur == troef) // De beginnende speler heeft troef gevraagd
		{
			if (troef > NO_VALUE && telkleuren[troef] > 0) // De speler heeft troef, onderzoek of er overgetroefd kan worden.
			{
				int tel = 0;
				for (int i = 0; i < 8; ++i)
				{
					if (kleuren[i] != NO_VALUE && kleuren[i] == troef && troefwaarde[i] > hogeretroefwaarde)
					{
						kaarten_mag_spelen[i] = speler_kaarten[speler][i];
						++tel;
					}
				}
				if (tel > 0) // speler heeft troefkaarten die kunnen overtroeven, zoeken is klaar
				{
					return;
				}
				// overtroeven kan niet maar omdat er troef is gevraagd kan elke troef worden gespeeld
				for (int i = 0; i < 8; ++i)
				{
					if (kleuren[i] != NO_VALUE && kleuren[i] == troef)
					{
						kaarten_mag_spelen[i] = speler_kaarten[speler][i];
					}
				}
			}
			else
			{
				// speler heeft geen troef elke kaart kan worden bijgespeeld
				for (int i = 0; i < 8; ++i)
				{
					if (kleuren[i] > NO_VALUE)
					{
						kaarten_mag_spelen[i] = speler_kaarten[speler][i];
					}
				}
			}
		}
		else
		{
			// beginnende speler heeft geen troef gespeeld, bepaal of speler kaarten in de gevraagde kleur heeft en dus gespeeld mogen worden
			if (telkleuren[speelkleur] > 0)
			{
				for (int i = 0; i < 8; ++i)
				{
					if (kleuren[i] != NO_VALUE && kleuren[i] == slag_kleur_gevraagd[slagen_teller])
					{
						kaarten_mag_spelen[i] = speler_kaarten[speler][i];
					}
				}
			}
			else
			{
				// speler heeft geen kaarten in de gevraagde kleur, bepaal of de speler nog troeven heeft, indien er al door een andere speler
				// heeft ingetroefd moet de speler overtroeven, kan hij dit niet dan moet hij verplicht ondertroeven
				if (troef > NO_VALUE && telkleuren[troef] > 0)
				{
					int tel = 0;
					if (hogeretroefwaarde > NO_VALUE)
					{
						// er is al ingetroefd
						for (int i = 0; i < 8; ++i)
						{
							if (kleuren[i] != NO_VALUE && kleuren[i] == troef && troefwaarde[i] > hogeretroefwaarde)
							{
								kaarten_mag_spelen[i] = speler_kaarten[speler][i];
								++tel;
							}
						}
						if (tel > 0) // speler heeft een of meerdere hogere troeven, zoek is klaar
						{
							return;
						}
						// er kan niet overtoefd worden dus elke mogelijke troef kan worden bij gepeeld
						for (int i = 0; i < 8; ++i)
						{
							if (kleuren[i] != NO_VALUE && kleuren[i] == troef)
							{
								kaarten_mag_spelen[i] = speler_kaarten[speler][i];
							}
						}
					}
					else
					{
						// er is nog niet ingetroefd dus elke troef is mogelijk
						for (int i = 0; i < 8; ++i)
						{
							if (kleuren[i] != NO_VALUE && kleuren[i] == troef)
							{
								kaarten_mag_spelen[i] = speler_kaarten[speler][i];
							}
						}
					}
				}
				else
				{
					// speler heeft de gevraagde kleur niet en ook geen troef, elke kaart kan worden gespeeld
					for (int i = 0; i < 8; ++i)
					{
						if (kleuren[i] > NO_VALUE)
						{
							kaarten_mag_spelen[i] = speler_kaarten[speler][i];
						}
					}
				}
			}
		}
	}
}

/// @brief speel een kaart bij voor de AI.
/// @param speler Noord, Oost of West
void Game_t::Kj_speler_beurt(int speler)
{
	int fnd = NO_VALUE;

	tel_mag_spelen = 0;
	for (int i = 0; i < 8; ++i)
	{
		if (kaarten_mag_spelen[i] > NO_VALUE)
		{
			++tel_mag_spelen;
			fnd = i;
		}
	}

	if (tel_mag_spelen == 1)
	{
		// speler speelt de enige kaart die hij mag spelen bij
#ifndef NDEBUG
		printf("%d Speler %d (%s) speelt zijn enige kaart bij\n", __LINE__, speler, spelernaam[speler].c_str());
#endif
		Al_speel_kaart_bij(speler, fnd);
		return;
	}

	Al_vul_filter(kaarten_mag_spelen);

	// random kaart
	for (int i = 0; i < 8; ++i)
	{
		if (kaarten_mag_spelen[i] > NO_VALUE && GetRandomValue(0, 9) >= 5)
		{
			fnd = i;
		}
	}

	if (tel_speler_gespeeld == 0) /// eerste speler die een kaart speelt
	{
		if (Al_speel_als_1e_speler(speler) > NO_VALUE)
		{
			return;
		}
	}
	if (tel_speler_gespeeld == 1) /// speel als 2e speler kaart bij als antwoord op de eerste kaart
	{
		if (Al_speel_als_2e_speler(speler) > NO_VALUE)
		{
			return;
		}
	}
	if (tel_speler_gespeeld == 2) /// speel als 3e speler een kaart bij als antwoord op de eerste 2 kaarten
	{
		if (Al_speel_als_3e_speler(speler) > NO_VALUE)
		{
			return;
		}
	}
	if (tel_speler_gespeeld == 3) /// speel als 4e speler een kaart bij als antwoord op de eerste 3 kaarten
	{
		if (Al_speel_als_4e_speler(speler) > NO_VALUE)
		{
			return;
		}
	}

	// speler heeft geen goede oplossing gevonden en speelt een willkeurige kaart bij
#ifndef NDEBUG
	printf("%d Speler %d (%s) speelt een willekeurige kaart bij\n", __LINE__, speler, spelernaam[speler].c_str());
#endif
	Al_speel_kaart_bij(speler, fnd);
}

/// @brief Update de vlaggen voor een speler indien alle kaarten in een kleur zijn gespeeld
void Game_t::Kj_update_speler_heeft_kleuren()
{
	for (int klr = klaveren; klr <= ruiten; ++klr)
	{
		if (Al_tel_kleur(klr, spel_kaarten, 32) == 0) // alle kaarten in een kleur zijn gespeeld
		{
			for (int s = noord; s <= west; ++s)
			{
				speler_heeft_kleur[s][klr] = false;
			}
		}
	}
}

/// @brief Test het resultaat van een gespeelde kaart op de tafel door de speler
/// @param speler Noord, Oost of West
/// @param kaart Gespeelde kaart
void Game_t::Kj_test_tafel(int speler, int kaart)
{
	int k;
	int spl = slag_beginner[slagen_teller];
	int klr[4] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int wrd[4] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int rmrng[4] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	bool trf[4] = {false, false, false, false};
	bool troefgevraagd = speelkleur == troef;
	bool troefgespeeld = false;
	int telstuk = 0;
	int telgespeeld = 0;

#ifndef NDEBUG
	printf("%d Testtafel: (troef:%s)\n", __LINE__, kleurnaam[troef].c_str());
	for (int i = 0; i < 4; ++i)
	{
		if (test_tafel[i] == NO_VALUE)
		{
			printf("-%s:*\n", spelernaam[i].c_str());
		}
		else
		{
			printf("-%s:%s\n", spelernaam[i].c_str(), kaartnaam[test_tafel[i]].c_str());
		}
	}
#endif

	for (int s = noord; s <= west; ++s)
	{
		k = test_tafel[s];
		if (k == NO_VALUE)
		{
			rmrng[s] = NO_VALUE;
			wrd[s] = NO_VALUE;
			klr[s] = NO_VALUE;
			trf[s] = false;
			continue;
		}
		++telgespeeld;
		klr[s] = KAARTKLEUR(k);
		rmrng[s] = (4 - klr[s]) * 10 + KAARTTYPE(k);
		if (KAARTKLEUR(k) == troef)
		{
			wrd[s] = TROEFRANG(k);
			if (KAARTTYPE(k) == vrouw || KAARTTYPE(k) == koning)
			{
				++telstuk;
			}
			trf[s] = true;
			troefgespeeld = true;
		}
		else if (KAARTKLEUR(k) == slag_kleur_gevraagd[slagen_teller])
		{
			wrd[s] = KAARTRANG(k);
			trf[s] = false;
		}
		else
		{
			wrd[s] = NO_VALUE;
			trf[s] = false;
		}
	}
	// bepaal roem
	// sorteer rmrng
	test_roem = 0;
	if (telgespeeld >= 3)
	{
		for (int i = 0; i < 3; ++i)
		{
			int mxi = i;
			for (int idy = i + 1; idy < 4; ++idy)
			{
				if (rmrng[idy] > rmrng[mxi])
				{
					mxi = idy;
				}
			}
			if (mxi != i)
			{
				int tmp = rmrng[i];
				rmrng[i] = rmrng[mxi];
				rmrng[mxi] = tmp;
			}
		}

		// is er stuk gepeeld?
		if (telstuk == 2)
		{
			test_roem += 20;
		}

		// zijn er 4 dezelfde plaatjes gespeeld
		int tel = 0;
		for (int i = 0; i < 4; ++i)
		{
			if ((rmrng[i] % 10 == rmrng[0] % 10) && (rmrng[i] % 10 == aas || rmrng[i] % 10 == koning || rmrng[i] % 10 == vrouw || rmrng[i] % 10 == boer))
			{
				++tel;
			}
		}
		if (tel == 4)
		{
			test_roem += 100;
		}

		// is er vierkaart in een kleur gespeeld?
		if (rmrng[0] - rmrng[1] == 1 && rmrng[0] - rmrng[2] == 2 && rmrng[0] - rmrng[3] == 3)
		{
			test_roem += 50;
		}

		// is er een driekaart in een kleur gespeeld?
		else if (((rmrng[0] - rmrng[1] == 1) && (rmrng[0] - rmrng[2] == 2)) || ((rmrng[1] - rmrng[2] == 1) && (rmrng[1] - rmrng[3] == 2)))
		{
			test_roem += 20;
		}
	}

	test_winnaar = slag_beginner[slagen_teller];
	// bepaal de winnaar
	if (troefgevraagd)
	{
		// bepaal de hogere troef speler indien er troef is gespeeld anders gaat de slag toch verloren
		for (int i = 1; i <= 3; ++i)
		{
			++spl;
			if (spl > west)
			{
				spl = noord;
			}
			if (trf[spl] && wrd[spl] > wrd[test_winnaar])
			{
				test_winnaar = spl;
			}
		}
	}
	else
	{
		// er wordt een andere kleur gevraagd
		if (troefgespeeld)
		{
			// er is ingetroefd, de waarde van de niet troef spelers wordt op NO_VALUE gezet
			for (int s = noord; s <= west; ++s)
			{
				if (!trf[s])
				{
					wrd[s] = NO_VALUE;
				}
			}
			for (int i = 1; i <= 3; ++i)
			{
				++spl;
				if (spl > west)
				{
					spl = noord;
				}
				if (trf[spl] && wrd[spl] > wrd[test_winnaar])
				{
					test_winnaar = spl;
				}
			}
		}
		else
		{
			// kaarten die een kleur hebben die niet van de gevraagde kleur zijn hebben NO_VALUE als waarde
			for (int i = 1; i <= 3; ++i)
			{
				++spl;
				if (spl > west)
				{
					spl = noord;
				}
				if (wrd[spl] > wrd[test_winnaar])
				{
					test_winnaar = spl;
				}
			}
		}
	}
#ifndef NDEBUG
	printf("%d test_winnaar: %s, roem %d\n", __LINE__, spelernaam[test_winnaar].c_str(), test_roem);
#endif
}

/// @brief Werk data bij van de gespeelde kaart
/// @param speler Noord, Oost, Zuid of West
/// @param fnd index van de gespeelde kaart binnen de hand 
void Game_t::Kj_werk_data_bij(int speler, int fnd)
{
	int k;
	tafel_kaarten[speler] = speler_kaarten[speler][fnd];
	k = tafel_kaarten[speler];
	filter[k] = NO_VALUE;
#ifndef NDEBUG
	ASSERT(k > NO_VALUE);
#endif
	speler_kaarten[speler][fnd] = NO_VALUE;
	if (slag_beginner[slagen_teller] == speler)
	{
		slag_kleur_gevraagd[slagen_teller] = KAARTKLEUR(k);
	}
	++tel_speler_gespeeld;
	++slag_ad_beurt[slagen_teller];
	if (slag_ad_beurt[slagen_teller] > west)
	{
		slag_ad_beurt[slagen_teller] = noord;
	}
}

/// @brief Bepaal de winnaar van de slag
void Game_t::Kj_bepaal_winnaar_slag()
{
	int k;
	int punten = 0;
	int roem = 0;
	int win = slag_beginner[slagen_teller];
	int spl = slag_beginner[slagen_teller];
	int klr[4] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int wrd[4] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int rmrng[4] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	bool trf[4] = {false, false, false, false};
	bool troefgevraagd = slag_kleur_gevraagd[slagen_teller] == troef;
	bool troefgespeeld = false;
	int telstuk = 0;

	for (int s = noord; s <= west; ++s)
	{
		k = tafel_kaarten[s];
		klr[s] = KAARTKLEUR(k);
		rmrng[s] = (4 - klr[s]) * 10 + KAARTTYPE(k);
		if (KAARTKLEUR(k) == troef)
		{
			wrd[s] = TROEFRANG(k);
			punten += TROEFPUNTEN(k);
			if (KAARTTYPE(k) == vrouw || KAARTTYPE(k) == koning)
			{
				++telstuk;
			}
			trf[s] = true;
			troefgespeeld = true;
		}
		else if (KAARTKLEUR(k) == slag_kleur_gevraagd[slagen_teller])
		{
			wrd[s] = KAARTRANG(k);
			punten += KAARTPUNTEN(k);
			trf[s] = false;
		}
		else
		{
			wrd[s] = NO_VALUE;
			punten += KAARTPUNTEN(k);
			trf[s] = false;
		}
	}
	// bepaal roem
	// sorteer rmrng
	for (int i = 0; i < 3; ++i)
	{
		int mxi = i;
		for (int idy = i + 1; idy < 4; ++idy)
		{
			if (rmrng[idy] > rmrng[mxi])
			{
				mxi = idy;
			}
		}
		if (mxi != i)
		{
			int tmp = rmrng[i];
			rmrng[i] = rmrng[mxi];
			rmrng[mxi] = tmp;
		}
	}

	// is er stuk gepeeld?
	if (telstuk == 2)
	{
		tekst_roem = 9;
		roem += 20;
	}

	// zijn er 4 dezelfde plaatjes gespeeld
	int tel = 0;
	for (int i = 0; i < 4; ++i)
	{
		if ((rmrng[i] % 10 == rmrng[0] % 10) && (rmrng[i] % 10 == aas || rmrng[i] % 10 == koning || rmrng[i] % 10 == vrouw || rmrng[i] % 10 == boer))
		{
			++tel;
		}
	}
	if (tel == 4)
	{
		roem += 100;
		tekst_roem = 12;
	}

	// is er vierkaart in een kleur gespeeld?
	if (rmrng[0] - rmrng[1] == 1 && rmrng[0] - rmrng[2] == 2 && rmrng[0] - rmrng[3] == 3)
	{
		roem += 50;
		tekst_roem = (telstuk == 2) ? 11 : 8;
	}

	// is er een driekaart in een kleur gespeeld?
	else if ((rmrng[0] - rmrng[1] == 1 && rmrng[0] - rmrng[2] == 2) || (rmrng[1] - rmrng[2] == 1 && rmrng[1] - rmrng[3] == 2))
	{
		roem += 20;
		tekst_roem = (telstuk == 2) ? 10 : 7;
	}

	// bepaal de winnaar
	if (troefgevraagd)
	{
		// bepaal de hogere troef speler indien er troef is gespeeld anders gaat de slag toch verloren
		for (int i = 1; i <= 3; ++i)
		{
			++spl;
			if (spl > west)
			{
				spl = noord;
			}
			if (trf[spl] && wrd[spl] > wrd[win])
			{
				win = spl;
			}
		}
	}
	else
	{
		// er wordt een andere kleur gevraagd
		if (troefgespeeld)
		{
			// er is introefd, de waarde van de niet troef spelers wordt op NO_VALUE gezet
			for (int s = noord; s <= west; ++s)
			{
				if (!trf[s])
				{
					wrd[s] = NO_VALUE;
				}
			}
			for (int i = 1; i <= 3; ++i)
			{
				++spl;
				if (spl > west)
				{
					spl = noord;
				}
				if (trf[spl] && wrd[spl] > wrd[win])
				{
					win = spl;
				}
			}
		}
		else
		{
			// kaarten die een kleur hebben die niet van de gevraagde kleur zijn hebben NO_VALUE als waarde
			for (int i = 1; i <= 3; ++i)
			{
				++spl;
				if (spl > west)
				{
					spl = noord;
				}
				if (wrd[spl] > wrd[win])
				{
					win = spl;
				}
			}
		}
	}
	roemgemaakt = roem > 0;
	slag_winnaar[slagen_teller] = win;
	if (slagen_teller == 7) // laatste slag
		punten += 10;
	if (win == zuid || win == noord)
	{
		roem_punten[wij][manche_teller] += roem;
		manche_punten[wij][manche_teller] += punten;
		++slagen_gewonnen[wij];
	}
	else
	{
		roem_punten[zij][manche_teller] += roem;
		manche_punten[zij][manche_teller] += punten;
		++slagen_gewonnen[zij];
	}
}

/// @brief bepaal het resultaat van de actieve manche
void Game_t::Kj_bepaal_resultaat_manche()
{
	int totaal = manche_punten[wij][manche_teller] + roem_punten[wij][manche_teller] +
				 manche_punten[zij][manche_teller] + roem_punten[zij][manche_teller];
	int verplicht = totaal / 2 + 1;
	if (gespeeld[wij])
	{
		if (manche_punten[wij][manche_teller] + roem_punten[wij][manche_teller] < verplicht)
		{
			is_nat[wij] = true;
			totaal_punten[zij] += totaal;
			roem_punten[zij][manche_teller] += roem_punten[wij][manche_teller];
			manche_punten[zij][manche_teller] = 162;
			manche_punten[wij][manche_teller] = -1;
			roem_punten[wij][manche_teller] = -1;
		}
		else if (slagen_gewonnen[wij] == 8)
		{
			is_mars[wij] = true;
			roem_punten[wij][manche_teller] += 100;
			totaal_punten[wij] += totaal + 100;
			manche_punten[wij][manche_teller] = -2;
		}
		else
		{
			gemaakt[wij] = true;
			totaal_punten[wij] += manche_punten[wij][manche_teller] + roem_punten[wij][manche_teller];
			totaal_punten[zij] += manche_punten[zij][manche_teller] + roem_punten[zij][manche_teller];
		}
	}
	if (gespeeld[zij])
	{
		if (manche_punten[zij][manche_teller] + roem_punten[zij][manche_teller] < verplicht)
		{
			is_nat[zij] = true;
			totaal_punten[wij] += totaal;
			roem_punten[wij][manche_teller] += roem_punten[zij][manche_teller];
			manche_punten[wij][manche_teller] = 162;
			manche_punten[zij][manche_teller] = -1;
			roem_punten[zij][manche_teller] = -1;
		}
		else if (slagen_gewonnen[zij] == 8)
		{
			is_mars[zij] = true;
			roem_punten[zij][manche_teller] += 100;
			totaal_punten[zij] += totaal + 100;
			manche_punten[zij][manche_teller] = -2;
		}
		else
		{
			gemaakt[zij] = true;
			totaal_punten[wij] += manche_punten[wij][manche_teller] + roem_punten[wij][manche_teller];
			totaal_punten[zij] += manche_punten[zij][manche_teller] + roem_punten[zij][manche_teller];
		}
	}
}

/// @brief vul stapel kaarten met kaarten van de spelers die nog moeten bijspelen
/// @param valide_kaarten lijst met kaarten
void Game_t::Al_vul_filter(int *valide_kaarten)
{
	int k;
	for (int i = 0; i < 32; ++i)
	{
		filter[i] = spel_kaarten[i];
	}
	for (int i = 0; i < 8; ++i)
	{
		// verwijder eigen kaarten uit de filter
		k = valide_kaarten[i];
		if (k > NO_VALUE)
		{
			filter[k] = NO_VALUE;
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		// verwijder tafel kaarten uit de filter
		k = tafel_kaarten[i];
		if (k > NO_VALUE)
		{
			filter[k] = NO_VALUE;
		}
	}
}

/// @brief Tel de kaarten van een kleur in de lijst van valide kaarten
/// @param kleur klaveren, harten, schoppen of ruiten
/// @param valide_kaarten lijst met kaarten
/// @param len lengte van de lijst
/// @return telling
int Game_t::Al_tel_kleur(int kleur, int *valide_kaarten, int len)
{
	int k;
	int telklr = 0;
	for (int i = 0; i < len; ++i)
	{
		k = valide_kaarten[i];
		if (k > NO_VALUE && KAARTKLEUR(k) == kleur)
		{
			++telklr;
		}
	}
	return telklr;
}

/// @brief Vind de hogere kaart (rang) in een kleur in de lijst van valide kaarten
/// @param kleur kleur klaveren, harten, schoppen of ruiten
/// @param valide_kaarten lijst met kaarten
/// @param len lengte van de lijst
/// @return index binnen de lijst
int Game_t::Al_vind_hogere_kaart(int kleur, int *valide_kaarten, int len) const
{
	int k;
	int maxval = NO_VALUE;
	int maxidx = NO_VALUE;
	for (int i = 0; i < len; ++i)
	{
		k = valide_kaarten[i];
		if (k > NO_VALUE && KAARTKLEUR(k) == kleur)
		{
			int rang = kleur == troef ? TROEFRANG(k) : KAARTRANG(k);
			if (rang > maxval)
			{
				maxidx = i;
				maxval = rang;
			}
		}
	}
	return maxidx;
}

/// @brief Vind de lagere kaart (rang) in een kleur in de lijst van valide kaarten
/// @param kleur klaveren, harten, schoppen of ruiten
/// @param valide_kaarten lijst met kaarten
/// @param len lengte van de lijst
/// @return index binnen de lijst
int Game_t::Al_vind_lagere_kaart(int kleur, int *valide_kaarten, int len) const
{
	int k;
	int minval = 1000000;
	int minidx = NO_VALUE;
	for (int i = 0; i < len; ++i)
	{
		k = valide_kaarten[i];
		if (k > NO_VALUE && KAARTKLEUR(k) == kleur)
		{
			int rang = kleur == troef ? TROEFRANG(k) : KAARTRANG(k);
			if (rang < minval)
			{
				minidx = i;
				minval = rang;
			}
		}
	}
	return minidx;
}

/// @brief speel een kaart bij, werk het bezit van kleuren bij afhankelijk van de bijgespeelde kaart
/// @param speler Noord, Oost, Zuid of West
/// @param idx index van de gespeelde kaart
void Game_t::Al_speel_kaart_bij(int speler, int idx)
{
#ifndef NDEBUG
	printf("%d speler %s, speelt kaart bij, index %d, kaart: %s\n", __LINE__, spelernaam[speler].c_str(), idx, kaartnaam[kaarten_mag_spelen[idx]].c_str());
#endif
	Kj_werk_data_bij(speler, idx);
	int beginner = slag_beginner[slagen_teller];
	if (speler != beginner)
	{
		int beginkleur = KAARTKLEUR(tafel_kaarten[beginner]);
		int speelkleur = KAARTKLEUR(tafel_kaarten[speler]);
		if (speelkleur != beginkleur)
		{
			speler_heeft_kleur[speler][beginkleur] = false;
			if (speelkleur != troef)
			{
				speler_heeft_kleur[speler][troef] = false;
			}
		}
	}
}

/// @brief Speel de hogere kaart in een kleur, houd rekening met mogelijke roemspeel de hogere kaart in een kleur, houd rekening met mogelijke roem
/// @param speler Noord, Oost of West
/// @param kleur klaveren, harten, schoppen of ruiten
/// @return index binnen de lijst van kaarten die gespeeld mogen worden
int Game_t::Al_speel_hogere_kaart_in_kleur(int speler, int kleur)
{
#ifndef NDEBUG
	printf("%d %s speelt een hogere kaart in kleur %s\n", __LINE__, spelernaam[speler].c_str(), kleurnaam[kleur].c_str());
#endif

	int kaartwaarde[8] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int k = NO_VALUE;
	int tmp = NO_VALUE;

	for (int i = 0; i < 8; ++i)
	{
		k = kaarten_mag_spelen[i];
		if (k == NO_VALUE)
		{
			continue;
		}
		if (KAARTKLEUR(k) != kleur)
		{
			continue;
		}
		kaartwaarde[i] = kleur == troef ? TROEFPUNTEN(k) * 10 + TROEFRANG(k) : KAARTPUNTEN(k) * 10 + KAARTRANG(k);
		if (!(kaarten_ms_winnaar[i]))
		{
			kaartwaarde[i] *= -1;
		}
		for (int j = 0; j < 4; ++j)
		{
			test_tafel[j] = tafel_kaarten[j];
		}
		Kj_test_tafel(speler, k);
		if (test_roem > 0)
		{
			test_roem *= 10;
			if (test_winnaar == speler)
			{
				if (kaarten_ms_winnaar[i])
				{
					kaartwaarde[i] += test_roem;
				} // winnaar => hogere prioriteit
				else
				{
					kaartwaarde[i] -= test_roem;
				} // slag gaat mogelijk verloren => lagere prioriteit
			}
			else if (test_winnaar == maat)
			{
				int klr = KAARTKLEUR(test_tafel[test_winnaar]);
				bool iswinnaar = false;
				int rang = NO_VALUE;
				if (klr == troef)
				{
					rang = TROEFRANG(test_tafel[test_winnaar]);
					iswinnaar = (rang >= trfrng2 && rang >= trfrng3);
				}
				else
				{
					rang = KAARTRANG(test_tafel[test_winnaar]);
					iswinnaar = (rang >= rng2 && rang >= rng3);
				}
				if (iswinnaar)
				{
					kaartwaarde[i] += test_roem;
				} // winnaar => hogere prioriteit
				else
				{
					kaartwaarde[i] -= test_roem;
				} // slag gaat mogelijk verloren => lagere prioriteit
			}
			else
			{
				kaartwaarde[i] -= test_roem; // slag gaat mogelijk verloren => lagere prioriteit
			}
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		if (kaartwaarde[i] != NO_VALUE)
		{
			if (tmp == NO_VALUE)
			{
				tmp = i;
			}
			else if (kaartwaarde[i] > kaartwaarde[tmp])
			{
				tmp = i;
			}
			else if (kaartwaarde[i] == kaartwaarde[tmp] && GetRandomValue(0, 9) >= 5)
			{
				tmp = i;
			}
		}
	}
	if (tmp > NO_VALUE)
	{
		Al_speel_kaart_bij(speler, tmp);
	}
	return tmp;
}

/// @brief speel de lagere kaart in een kleur, houd rekening met mogelijke roem
/// @param speler Noord, Oost of West
/// @param kleur klaveren, harten, schoppen of ruiten 
/// @return index binnen de lijst van kaarten die gespeeld mogen worden
int Game_t::Al_speel_lagere_kaart_in_kleur(int speler, int kleur)
{
#ifndef NDEBUG
	printf("%d %s speelt een lagere kaart in kleur %s\n", __LINE__, spelernaam[speler].c_str(), kleurnaam[kleur].c_str());
#endif

	int kaartwaarde[8] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int k = NO_VALUE;
	int tmp = NO_VALUE;

	for (int i = 0; i < 8; ++i)
	{
		k = kaarten_mag_spelen[i];
		if (k == NO_VALUE)
		{
			continue;
		}
		if (KAARTKLEUR(k) != kleur)
		{
			continue;
		}
		kaartwaarde[i] = kleur == troef ? TROEFPUNTEN(k) * 10 + TROEFRANG(k) : KAARTPUNTEN(k) * 10 + KAARTRANG(k);
		for (int j = 0; j < 4; ++j)
		{
			test_tafel[j] = tafel_kaarten[j];
		}
		Kj_test_tafel(speler, k);
		if (test_roem > 0)
		{
			test_roem *= 10;
			if (test_winnaar == speler)
			{
				if (kaarten_ms_winnaar[i])
				{
					kaartwaarde[i] -= test_roem;
				} // winnaar => hogere prioriteit
				else
				{
					kaartwaarde[i] += test_roem;
				} // slag gaat mogelijk verloren => lagere prioriteit
			}
			else if (test_winnaar == maat)
			{
				int klr = KAARTKLEUR(test_tafel[test_winnaar]);
				bool iswinnaar = false;
				int rang = NO_VALUE;
				if (klr == troef)
				{
					rang = TROEFRANG(test_tafel[test_winnaar]);
					iswinnaar = (rang >= trfrng2 && rang >= trfrng3);
				}
				else
				{
					rang = KAARTRANG(test_tafel[test_winnaar]);
					iswinnaar = (rang >= rng2 && rang >= rng3);
				}
				if (iswinnaar)
				{
					kaartwaarde[i] -= test_roem;
				} // winnaar => hogere prioriteit
				else
				{
					kaartwaarde[i] += test_roem;
				} // slag gaat mogelijk verloren => lagere prioriteit
			}
			else
			{
				kaartwaarde[i] += test_roem; // slag gaat mogelijk verloren => lagere prioriteit
			}
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		if (kaartwaarde[i] != NO_VALUE)
		{
			if (tmp == NO_VALUE)
			{
				tmp = i;
			}
			else if (kaartwaarde[i] < kaartwaarde[tmp])
			{
				tmp = i;
			}
			else if (kaartwaarde[i] == kaartwaarde[tmp] && GetRandomValue(0, 9) < 5)
			{
				tmp = i;
			}
		}
	}
	if (tmp > NO_VALUE)
	{
		Al_speel_kaart_bij(speler, tmp);
	}
	return tmp;
}

/// @brief Speel een waardevolle kaart bij maar geen winnaar
/// @param speler Noord, Oost of West
/// @return index binnen de lijst van kaarten die gespeeld mogen worden
int Game_t::Al_speel_lagere_waarde_kaart_bij(int speler)
{
#ifndef NDEBUG
	printf("%d %s speelt een lagere waarde kaart bij\n", __LINE__, spelernaam[speler].c_str());
#endif

	int k = NO_VALUE;
	int kaartwaarde[8] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};
	int tmp = NO_VALUE;

	for (int i = 0; i < 8; ++i)
	{
		k = kaarten_mag_spelen[i];
		if (k == NO_VALUE)
		{
			continue;
		}
		for (int j = 0; j < 4; ++j)
		{
			test_tafel[j] = tafel_kaarten[j];
		}
		kaartwaarde[i] = KAARTKLEUR(k) == troef ? 0 : KAARTPUNTEN(k) * 10 + KAARTRANG(k);
		if (kaarten_ms_winnaar[i])
		{
			kaartwaarde[i] *= 10;
		} // lagere prioriteit
	}
	for (int i = 0; i < 8; ++i)
	{
		if (kaartwaarde[i] != NO_VALUE)
		{
			if (tmp == NO_VALUE)
			{
				tmp = i;
			}
			else if (kaartwaarde[i] < kaartwaarde[tmp])
			{
				tmp = i;
			}
			else if (kaartwaarde[i] == kaartwaarde[tmp] && GetRandomValue(0, 9) < 5)
			{
				tmp = i;
			}
		}
	}
	if (tmp > NO_VALUE)
	{
		Al_speel_kaart_bij(speler, tmp);
	}
	return tmp;
}

/// @brief Speel een waardevolle kaart bij maar geen winnaar
/// @param speler Noord, Oost of West
/// @return index binnen de lijst van kaarten die gespeeld mogen worden
int Game_t::Al_speel_om_te_spekken(int speler)
{
#ifndef NDEBUG
	printf("%d %s speelt een kaart om te spekken bij\n", __LINE__, spelernaam[speler].c_str());
#endif

	int k;
	int tmp = NO_VALUE;
	int kaartwaarde[8] = {NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE};

	Al_bepaal_winnaar(NO_VALUE);

	for (int i = 0; i < 8; ++i)
	{
		k = kaarten_mag_spelen[i];
		if (k == NO_VALUE)
		{
			continue;
		}
		kaartwaarde[i] = KAARTKLEUR(k) == troef ? TROEFPUNTEN(k) * -10 - TROEFRANG(k) : KAARTPUNTEN(k) * 10 + KAARTRANG(k);
		if (kaarten_ms_winnaar[i])
		{
			kaartwaarde[i] = -kaartwaarde[i];
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		if (kaartwaarde[i] != NO_VALUE)
		{
			if (tmp == NO_VALUE)
			{
				tmp = i;
			}
			else if (kaartwaarde[i] > kaartwaarde[tmp])
			{
				tmp = i;
			}
			else if (kaartwaarde[i] == kaartwaarde[tmp] && GetRandomValue(0, 9) >= 5)
			{
				tmp = i;
			}
		}
	}
	if (tmp > NO_VALUE)
	{
		Al_speel_kaart_bij(speler, tmp);
	}
	return tmp;
}

/// @brief Initialiseer de data voor de analyse
/// @param speler Noord, Oost of West
/// @param kleur klaveren, harten, schoppen of ruiten
void Game_t::Al_init_analysedata(int speler, int kleur)
{
	speelkleur = kleur;
	trf1 = Al_tel_kleur(troef, kaarten_mag_spelen, 8);
	trf2 = Al_tel_kleur(troef, filter, 32);
	trf3 = Al_tel_kleur(troef, tafel_kaarten, 4);
	trft = trf1 + trf2 + trf3;
	trfidx1 = (trf1 > 0) ? Al_vind_hogere_kaart(troef, kaarten_mag_spelen, 8) : NO_VALUE;
	trfidx2 = (trf2 > 0) ? Al_vind_hogere_kaart(troef, filter, 32) : NO_VALUE;
	trfidx3 = (trf3 > 0) ? Al_vind_hogere_kaart(troef, tafel_kaarten, 4) : NO_VALUE;
	trfrng1 = (trf1 > 0 && trfidx1 > NO_VALUE) ? TROEFRANG(kaarten_mag_spelen[trfidx1]) : NO_VALUE;
	trfrng2 = (trf2 > 0 && trfidx2 > NO_VALUE) ? TROEFRANG(spel_kaarten[trfidx2]) : NO_VALUE;
	trfrng3 = (trf3 > 0 && trfidx3 > NO_VALUE) ? TROEFRANG(tafel_kaarten[trfidx3]) : NO_VALUE;

	if (kleur == troef)
	{
		t1 = trf1;
		t2 = trf2;
		t3 = trf3;
		tt = trft;
		idx1 = trfidx1;
		idx2 = trfidx2;
		idx3 = trfidx3;
		rng1 = trfrng1;
		rng2 = trfrng2;
		rng3 = trfrng3;
	}
	else if (kleur > NO_VALUE)
	{
		t1 = Al_tel_kleur(kleur, kaarten_mag_spelen, 8);
		t2 = Al_tel_kleur(kleur, filter, 32);
		t3 = Al_tel_kleur(kleur, tafel_kaarten, 4);
		tt = t1 + t2 + t3;
		idx1 = (t1 > 0) ? Al_vind_hogere_kaart(kleur, kaarten_mag_spelen, 8) : NO_VALUE;
		idx2 = (t2 > 0) ? Al_vind_hogere_kaart(kleur, filter, 32) : NO_VALUE;
		idx3 = (t3 > 0) ? Al_vind_hogere_kaart(kleur, tafel_kaarten, 4) : NO_VALUE;
		rng1 = (t1 > 0 && idx1 > NO_VALUE) ? KAARTRANG(kaarten_mag_spelen[idx1]) : NO_VALUE;
		rng2 = (t2 > 0 && idx2 > NO_VALUE) ? KAARTRANG(spel_kaarten[idx2]) : NO_VALUE;
		rng3 = (t3 > 0 && idx3 > NO_VALUE) ? KAARTRANG(tafel_kaarten[idx3]) : NO_VALUE;
	}
	else
	{
		t1 = t2 = t3 = tt = 0;
		idx1 = idx2 = idx3 = 0;
		rng1 = rng2 = rng3 = 0;
	}
	switch (speler)
	{
	case noord:
		tegen1 = oost;
		tegen2 = west;
		maat = zuid;
		break;
	case oost:
		tegen1 = zuid;
		tegen2 = noord;
		maat = west;
		break;
	case west:
		tegen1 = noord;
		tegen2 = zuid;
		maat = oost;
		break;
	}
	kan_troef_bijspelen = false;
	kan_klr_introeven = false;

#ifndef NDEBUG
	printf("%d troefrang (%s): %d %d %d, rang (%s): %d %d %d\n", __LINE__,
		   kleurnaam[troef].c_str(), trfrng1, trfrng2, trfrng3, kleurnaam[kleur].c_str(), rng1, rng2, rng3);
#endif
}

/// @brief Bepaal van de kaarten die mogen spelen of het een winnaar is
/// @param kleur klaveren, harten, schoppen of ruiten
void Game_t::Al_bepaal_winnaar(int kleur)
{
	int r1, r2, r3;
	r1 = r2 = r3 = NO_VALUE;

	if (kleur == troef)
	{
		for (int i = 0; i < 8; ++i)
		{
			kaarten_ms_winnaar[i] = false;
			if (kaarten_mag_spelen[i] > NO_VALUE && KAARTKLEUR(kaarten_mag_spelen[i]) == kleur)
			{
				// speler
				r1 = TROEFRANG(kaarten_mag_spelen[i]);
				// rest spelers en tafel
				kaarten_ms_winnaar[i] = r1 > trfrng2 && r1 > trfrng3;
			}
		}
	}
	else if (kleur != NO_VALUE)
	{
		for (int i = 0; i < 8; ++i)
		{
			kaarten_ms_winnaar[i] = false;
			if (kaarten_mag_spelen[i] > NO_VALUE && KAARTKLEUR(kaarten_mag_spelen[i]) == kleur && trf3 == 0)
			{
				// speler
				r1 = KAARTRANG(kaarten_mag_spelen[i]);
				// rest spelers en tafel
				kaarten_ms_winnaar[i] = r1 > rng2 && r1 > rng3;
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; ++i)
		{
			kaarten_ms_winnaar[i] = false;
			if (kaarten_mag_spelen[i] > NO_VALUE)
			{
				r1 = KAARTRANG(kaarten_mag_spelen[i]);
				r2 = Al_vind_hogere_kaart(KAARTKLEUR(kaarten_mag_spelen[i]), filter, 32);
				r3 = Al_vind_hogere_kaart(KAARTKLEUR(kaarten_mag_spelen[i]), tafel_kaarten, 4);
				kaarten_ms_winnaar[i] = r1 > r2 && r1 > r3;
			}
		}
	}
}

/// @brief Print de speler data
/// @param speler Noord, Oost of West
void Game_t::Al_print_speler_data(int speler) const
{
	printf("\nsplkrt :");
	for (int i = 0; i < 8; ++i)
	{
		if (speler_kaarten[speler][i] != NO_VALUE)
		{
			printf("%s ", kaartnaam[speler_kaarten[speler][i]].c_str());
		}
		else
		{
			printf("** ");
		}
	}
	printf("\nkms    :");
	for (int i = 0; i < 8; ++i)
	{
		if (kaarten_mag_spelen[i] != NO_VALUE)
		{
			printf("%s ", kaartnaam[kaarten_mag_spelen[i]].c_str());
		}
		else
		{
			printf("** ");
		}
	}
	printf("\nwinnaar:");
	for (int i = 0; i < 8; ++i)
	{
		printf("%d  ", BOOL2INT(kaarten_ms_winnaar[i]));
	}
	printf("\nheeft kleuren\n");
	printf("S : K H S R\n");
	printf("--+--------\n");
	for (int s = noord; s <= west; ++s)
	{
		printf("%s | ", spelerletter[s].c_str());
		for (int klr = klaveren; klr <= ruiten; ++klr)
		{
			printf("%d ", BOOL2INT(speler_heeft_kleur[s][klr]));
		}
		printf("\n");
	}
	printf("--+--------\n");
}

/// @brief Speel als eerste speler
/// @param speler Noord, Oost of West
/// @return index binnen de lijst van kaarten die gespeeld mogen worden of NO_VALUE als geen geschikte kaart wordt gevonden
int Game_t::Al_speel_als_1e_speler(int speler)
{
#ifndef NDEBUG
	printf("%d Speler 1 (%s) speelt kaart bij\n", __LINE__, spelernaam[speler].c_str());
#endif

	Al_init_analysedata(speler, troef);
	Al_bepaal_winnaar(troef);

#ifndef NDEBUG
	Al_print_speler_data(speler);
#endif

	if (trf1 > 0)
	{
		// Speler heeft nog 1 of meer troeven
		if (trf2 > 0)
		{
			kan_troef_bijspelen = speler_heeft_kleur[tegen1][troef] || speler_heeft_kleur[tegen2][troef];
#ifndef NDEBUG
			printf("%d kan_troef_bijspelen: %d\n", __LINE__, BOOL2INT(kan_troef_bijspelen));
#endif
			// resterende spelers hebben nog 1 of meer troeven
			if (kan_troef_bijspelen)
			{
				// zij hebben mogelijk nog troeven
				if (trfrng1 > trfrng2)
				{
					// speler heeft de hogere troef
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
				else
				{
					// speler heeft de lagere troef
					if (trf1 > trf2)
					{
						// speler heeft meer troeven dan de andere spelers
						return Al_speel_lagere_kaart_in_kleur(speler, troef);
					}
					else
					{
						// speler heeft minder troeven dan de andere spelers (goto A)
					}
				}
			}
			else
			{
				// resterende spelers hebben geen troeven meer (goto A)
			}
		}
		else
		{
			// andere spelers hebben geen troeven meer (goto A)
		}
	}
	// A:
	// onderzoek of er in de andere kleuren een winnaar is
	for (int klr = klaveren; klr <= ruiten; klr++)
	{
		if (klr == troef)
		{
			continue;
		} // troef is al onderzocht
		if (Al_tel_kleur(klr, kaarten_mag_spelen, 8) > 0)
		{
			Al_init_analysedata(speler, klr);
			Al_bepaal_winnaar(klr);
#ifndef NDEBUG
			printf("\nkms    :");
			for (int i = 0; i < 8; ++i)
			{
				if (kaarten_mag_spelen[i] != NO_VALUE)
				{
					printf("%s ", kaartnaam[kaarten_mag_spelen[i]].c_str());
				}
				else
				{
					printf("** ");
				}
			}
			printf("\nwinnaar:");
			for (int i = 0; i < 8; ++i)
			{
				printf("%d  ", BOOL2INT(kaarten_ms_winnaar[i]));
			}
			printf("\n");
#endif

			kan_klr_introeven = ((!speler_heeft_kleur[tegen1][klr] && speler_heeft_kleur[tegen1][troef]) ||
								 (!speler_heeft_kleur[tegen2][klr] && speler_heeft_kleur[tegen2][troef])); // kan er worden ingetroefd?
#ifndef NDEBUG
			printf("%d kan_klr_introeven: %d\n", __LINE__, BOOL2INT(kan_klr_introeven));
#endif
			if (rng1 > rng2 && !kan_klr_introeven)
			{
				// speler heeft een winnaar en er kan niet worden ingetroefd
				return Al_speel_hogere_kaart_in_kleur(speler, klr);
			}
		}
	}
	int maxklr = NO_VALUE;
	int maxtel = NO_VALUE;
	for (int klr = klaveren; klr <= ruiten; ++klr)
	{
		if (klr == troef)
		{
			continue;
		} // troef is al onderzocht
		int tel = Al_tel_kleur(klr, kaarten_mag_spelen, 8);
		if (tel > 0 && tel > maxtel)
		{
			maxtel = tel;
			maxklr = klr;
		}
	}
	if (maxklr > NO_VALUE)
	{
		return Al_speel_lagere_kaart_in_kleur(speler, maxklr);
	}

	// geen winnaar gevonden of geen lengte gevonden
	return Al_speel_lagere_waarde_kaart_bij(speler);
}

/// @brief Speel als 2e speler een kaart bij
/// @param speler Noord, Oost of West
/// @return index binnen de lijst van kaarten die gespeeld mogen worden of NO_VALUE als geen geschikte kaart wordt gevonden
int Game_t::Al_speel_als_2e_speler(int speler)
{
#ifndef NDEBUG
	printf("%d Speler 2 (%s) speelt kaart bij\n", __LINE__, spelernaam[speler].c_str());
#endif

	Al_init_analysedata(speler, slag_kleur_gevraagd[slagen_teller]);
	Al_bepaal_winnaar(slag_kleur_gevraagd[slagen_teller]);

#ifndef NDEBUG
	Al_print_speler_data(speler);
#endif

	if (speelkleur == troef)
	{
		kan_troef_bijspelen = speler_heeft_kleur[tegen1][troef];
#ifndef NDEBUG
		printf("%d kan_troef_bijspelen: %d\n", __LINE__, BOOL2INT(kan_troef_bijspelen));
#endif

		// kleur gevraagd is troef
		if (trf1 > 0)
		{
			// speler heeft 1 of meer troeven
			if (trf2 > 0 && kan_troef_bijspelen)
			{
				// tegenstander die nog moet bij spelen heeft 1 of meer troeven
				if (trfrng1 > trfrng2 && trfrng1 > trfrng3)
				{
					// speler heeft de hogere troef t.o.v. de tegenstander en de tafel
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
				else
				{
					// speler heeft de lagere troef t.o.v. de tegenstander en de tafel
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
			}
			else
			{
				// tegenstander die nog moet bij spelen heeft geen troeven meer
				if (trfrng1 > trfrng3)
				{
					// speler heeft de hogere troef t.o.v. de tafel
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
				else
				{
					// speler heeft de lagere troef t.o.v. de tafel
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
			}
		}
		else
		{
			// speler heeft geen troeven meer
			return Al_speel_lagere_waarde_kaart_bij(speler);
		}
	}
	// er wordt een andere kleur gevraagd
	kan_klr_introeven = !speler_heeft_kleur[tegen1][speelkleur] && speler_heeft_kleur[tegen1][troef];
#ifndef NDEBUG
	printf("%d kan_klr_introeven: %d\n", __LINE__, BOOL2INT(kan_klr_introeven));
#endif
	if (t1 > 0)
	{
		// speler heeft de gevraagde kleur
		if (kan_klr_introeven)
		{
			// tegenstander kan mogelijk introeven
			return Al_speel_lagere_kaart_in_kleur(speler, speelkleur);
		}
		else
		{
			/// tegenstander kan mogelijk niet introeven
			if (rng1 > rng2 && rng1 > rng3)
			{
				// speler heeft de hogere kaart t.o.v. de tegenstander en de tafel in de gevraagde kleur
				return Al_speel_hogere_kaart_in_kleur(speler, speelkleur);
			}
			else
			{
				// speler heeft de lagere kaart t.o.v. de tegnstander en de tafel in de gevraagde kleur
				return Al_speel_lagere_kaart_in_kleur(speler, speelkleur);
			}
		}
	}
	else
	{
		// speler heeft niet de gevraagde kleur
		if (trf1 > 0)
		{
			// speler heeft troef en moet introeven
			if (trfrng1 > trfrng2)
			{
				// speler heeft de hogere troef t.o.v. de tegenstander
				return Al_speel_lagere_kaart_in_kleur(speler, troef);
			}
			else
			{
				// speler heeft de lagere troef
				if (!kan_klr_introeven)
				{
					// tegenstander kan niet introeven
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
				else
				{
					// tegenstander kan overtroeven
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
			}
		}
		else
		{
			// speler kan niet introeven
			return Al_speel_lagere_waarde_kaart_bij(speler);
		}
	}
	// geen geschikte kaart gevonden
	return NO_VALUE;
}

/// @brief Speel als 3e speler een kaart bij
/// @param speler Noord, Oost of West
/// @return index binnen de lijst van kaarten die gespeeld mogen worden of NO_VALUE als geen geschikte kaart wordt gevonden
int Game_t::Al_speel_als_3e_speler(int speler)
{
#ifndef NDEBUG
	printf("%d Speler 3 (%s) speelt kaart bij\n", __LINE__, spelernaam[speler].c_str());
#endif

	Al_init_analysedata(speler, slag_kleur_gevraagd[slagen_teller]);
	Al_bepaal_winnaar(slag_kleur_gevraagd[slagen_teller]);

#ifndef NDEBUG
	Al_print_speler_data(speler);
#endif

	if (speelkleur == troef)
	{
		// de gevraagde kleur is troef
		kan_troef_bijspelen = speler_heeft_kleur[tegen1][troef];
#ifndef NDEBUG
		printf("%d kan_troef_bijspelen: %d\n", __LINE__, BOOL2INT(kan_troef_bijspelen));
#endif
		if (trf1 > 0)
		{
			// speler heeft ook een of meer troeven
			if (trf2 > 0 && speler_heeft_kleur[tegen1][troef])
			{
				// de tegenstander heeft nog een of meer troeven
				if (trfrng1 > trfrng2 && trfrng1 > trfrng3)
				{
					// speler heeft de hogere troef t.o.v de tegenstander en de tafel
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
				else
				{

					// speler heeft de lagere troef t.o.v tegenstander en de tafel
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
			}
			else
			{
				// de laatste tegenstander heeft geen troeven meer
				if (trfrng1 > trfrng3)
				{
					// speler heeft de hogere troef t.o.v de tafel, onderzoek of de slag aan de maat is
					if (trfidx3 == maat)
					{
						// de slag is aan de maat, speel een lagere troef
						return Al_speel_lagere_kaart_in_kleur(speler, troef);
					}
					else
					{
						// de slag is niet aan de maat
						return Al_speel_hogere_kaart_in_kleur(speler, troef);
					}
				}
				else
				{
					// speler heeft de lagere troef t.o.v de tafel
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
			}
		}
		else
		{
			// speler heeft geen troeven meer
			kan_troef_bijspelen = speler_heeft_kleur[tegen1][troef];
			if (trfidx3 == maat && !kan_troef_bijspelen)
			{
				// de slag is aan de maat en heeft de hogere troef en er kan niet door de laatste tegenstander worden getroefd
				return Al_speel_om_te_spekken(speler);
			}
			else
			{
				// de slag is niet aan de maat of de hogere troef kan worden gespeeld
				return Al_speel_lagere_waarde_kaart_bij(speler);
			}
		}
	}

	// andere kleur dan troef wordt gevraagd
	kan_klr_introeven = !speler_heeft_kleur[tegen1][speelkleur] && speler_heeft_kleur[tegen1][troef];
#ifndef NDEBUG
	printf("%d kan_klr_introeven: %d\n", __LINE__, BOOL2INT(kan_klr_introeven));
#endif
	if (t1 > 0)
	{
		// tspeler heeft de gevraagde kleur
		if (kan_klr_introeven || trf3 > 0)
		{
			// er kan worden ingetroefd of er is al ingetroefd
			return Al_speel_lagere_kaart_in_kleur(speler, speelkleur);
		}
		else
		{
			// er kan niet worden ingetroefd of er is nog niet ingetroefd
			if (rng1 > rng2 && rng1 > rng3 && trf3 == 0)
			{
				// speler heeft de hogere kaart in de gevraagde kleur t.o.v de tegenstander en de tafel en er is niet ingetroefd
				return Al_speel_hogere_kaart_in_kleur(speler, speelkleur);
			}
			else
			{
				// speler heeft de lagere kaart in de gevraagde kleur t.o.v. de tegenstander of er is ingetroefd
				return Al_speel_lagere_kaart_in_kleur(speler, speelkleur);
			}
		}
	}
	else
	{
		// speler heeft geen kaarten in de kleur
		if (trf1 > 0)
		{
			// speler heeft troef en moet introeven
			if (trfrng1 > trfrng2 && trfrng1 > trfrng3 && kan_klr_introeven)
			{
				return Al_speel_hogere_kaart_in_kleur(speler, troef);
			}
			else
			{
				// speler heeft de lagere troef t.o.v. tegenstander en de tafel, speel de lagere troef
				return Al_speel_lagere_kaart_in_kleur(speler, troef);
			}
		}
		else
		{
			// speler kan niet introeven
			if (trfidx3 == maat && !kan_klr_introeven)
			{
				// de slag is aan de maat en kan niet worden ingetroefd door de tegnstander
				return Al_speel_om_te_spekken(speler);
			}
			else
			{
				// de slag is niet aan de maat of er kan worden ingetroefd
				return Al_speel_lagere_waarde_kaart_bij(speler);
			}
		}
	}
	// geen geschikte kaart gevonden
	return NO_VALUE;
}

/// @brief Speel als 4e speler een kaart bij
/// @param speler Noord, Oost of West
/// @return index binnen de lijst van kaarten die gespeeld mogen worden of NO_VALUE als geen geschikte kaart wordt gevonden
int Game_t::Al_speel_als_4e_speler(int speler)
{
#ifndef NDEBUG
	printf("%d Speler 4 (%s) speelt kaart bij\n", __LINE__, spelernaam[speler].c_str());
#endif

	Al_init_analysedata(speler, slag_kleur_gevraagd[slagen_teller]);
	Al_bepaal_winnaar(slag_kleur_gevraagd[slagen_teller]);

#ifndef NDEBUG
	Al_print_speler_data(speler);
#endif

	if (speelkleur == troef)
	{
		// de gevraagde kleur is troef
		if (trf1 > 0)
		{
			// speler heeft ook een of meer troeven
			if (trfrng1 > trfrng3)
			{
				// speler heeft de hogere troef t.o.v de tafel
				if (trfidx3 == maat)
				{
					// de slag is aan de maat
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
				else
				{
					// de slag is niet aan de maat
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
			}
			else
			{
				// speler heeft de lagere troef t.o.v de tafel
				if (trfidx3 == maat)
				{
					// de slag is aan de maat
					if (GetRandomValue(0, 9) >= 5)
					{
						return Al_speel_lagere_kaart_in_kleur(speler, troef);
					}
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
				else
				{
					// slag is niet aan de maat
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
			}
		}
		else
		{
			// speler heeft geen troeven meer
			if (trfidx3 == maat)
			{
				// de slag is aan de maat
				return Al_speel_om_te_spekken(speler);
			}
			else
			{
				// de slag is niet aan de maat
				return Al_speel_lagere_waarde_kaart_bij(speler);
			}
		}
	}
	// andere kleur dan troef wordt gevraagd
	if (t1 > 0)
	{
		// speler heeft de gevraagde kleur
		if (trf3 > 0)
		{
			// er is al ingetroefd
			return Al_speel_lagere_kaart_in_kleur(speler, speelkleur);
		}
		else
		{
			// er is niet in getroefd
			if (rng1 > rng3)
			{
				// de speler heeft de hogere kaart in de gevraagde kleur t.o.v de tafel
				return Al_speel_hogere_kaart_in_kleur(speler, speelkleur);
			}
			else
			{
				if (idx3 == maat)
				{
					// de slag is aan de maat
					if (GetRandomValue(0, 9) >= 5)
					{
						return Al_speel_lagere_kaart_in_kleur(speler, speelkleur);
					}
					return Al_speel_hogere_kaart_in_kleur(speler, speelkleur);
				}
				else
				{
					// de speler heeft de lagere kaart in de gevraagde kleur
					return Al_speel_lagere_kaart_in_kleur(speler, speelkleur);
				}
			}
		}
	}
	else
	{
		// speler heeft geen kaarten in de gevraagde kleur
		if (trf1 > 0)
		{
			// speler heeft troef en moet introeven\n");
			if (trfrng1 > trfrng3)
			{
				// speler heeft de hogere troef
				if (trf3 > 0 && trfidx3 == maat)
				{
					// er is al ingetroefd en de slag is aan de maat
					return Al_speel_lagere_kaart_in_kleur(speler, troef);
				}
				else
				{
					// de slag is niet aan de maat
					return Al_speel_hogere_kaart_in_kleur(speler, troef);
				}
			}
			else
			{
				// speler heeft de lagere troef
				return Al_speel_lagere_kaart_in_kleur(speler, troef);
			}
		}
		else
		{
			// speler kan niet introeven
			if ((trf3 > 0 && trfidx3 == maat) || (trf3 == NO_VALUE && idx3 == maat))
			{
				// de slag is aan de maat
				return Al_speel_om_te_spekken(speler);
			}
			else
			{
				// de slag is niet aan de maat
				return Al_speel_lagere_waarde_kaart_bij(speler);
			}
		}
	}
	// er is geen geschikte kaart gevonden
	return NO_VALUE;
}
