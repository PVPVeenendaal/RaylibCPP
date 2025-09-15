#pragma once
#include <iostream>
#include <raylib.h>
#include <string>

class Game_t
{
public:
	Game_t();
	~Game_t();
	void Load_Images();
	void Unload_Images() const;
	void Kj_update();
	void Kj_draw();
	void Kj_key_handler(int);
	void Kj_mouse_handler(int, int);
private:
	void Kj_init_spel();
	void Kj_init_manche();
	void Kj_init_slag();
	void Kj_sorteer_hand(int);
	void Kj_vul_deel_kaarten();
	void Kj_kaarten_schudden();
	void Kj_kaarten_delen1();
	void Kj_kaarten_delen2();
	void Kj_kaarten_delen3();
	void Kj_bepaal_troef_ai(int);	
	void Kj_verwijder_kaart(int);
	void Kj_bepaal_mag_spelen(int);
	void Kj_speler_beurt(int);
	void Kj_update_speler_heeft_kleuren();
	void Kj_test_tafel(int, int);
	void Kj_werk_data_bij(int, int);
	void Kj_bepaal_winnaar_slag();
	void Kj_bepaal_resultaat_manche();
	// analyse
	void Al_vul_filter(int*);
	int Al_tel_kleur(int, int*, int);
	int Al_vind_hogere_kaart(int, int*, int) const;
	int Al_vind_lagere_kaart(int, int*, int) const;
	void Al_speel_kaart_bij(int, int);
	int Al_speel_hogere_kaart_in_kleur(int, int);
	int Al_speel_lagere_kaart_in_kleur(int, int);
	int Al_speel_lagere_waarde_kaart_bij(int);
	int Al_speel_om_te_spekken(int);
	void Al_init_analysedata(int, int);
	void Al_bepaal_winnaar(int);
	void Al_print_speler_data(int) const;
	int Al_speel_als_1e_speler(int);
	int Al_speel_als_2e_speler(int);
	int Al_speel_als_3e_speler(int);
	int Al_speel_als_4e_speler(int);
	
	// data
	int teller;
	// afbeelding containers
	Texture2D speeltafel;
	Texture2D troefkaarten[4];
	Texture2D speelkaarten[32];
	Texture2D kaart_achtergrond;
	Texture2D enter_button;
	Texture2D pijlen[4];
	Texture2D dir;
	// speldata
	int deel_kaarten[32];		   // [32 kaarten]
	int spel_kaarten[32];		   // [32 kaarten] kaarten nog in het spel, wordt tijdens het spel bijgewerkt
	int filter[32];				   // [kaarten nog in het spel bij andere spelers die nog moeten bijspelen, spel_kaarten - speler+kaarten - tafel]
	int speler_kaarten[4][8];	   // [speler][kaarten] kaarten per speler
	bool speler_heeft_kleur[4][4]; // heeft de speler nog een bepaalde kleur, wordt bijgewerkt bij het verwijderen van kaarten [speler][kleur]
	int manche_teller;			   // aantal manches
	int manche_punten[2][16];	   // [wij, zij], [16 manches]
	int roem_punten[2][16];		   // [wij, zij], [16 manches]
	int totaal_punten[2];		   // [wij, zij]
	int tafel_kaarten[4];		   // [4 speler kaarten]
	int test_tafel[4];			   // [test met 4 speler kaarten]
	int test_roem;				   // roem behaald op de test tafel
	int test_winnaar;			   // winnaar van de test tafel
	int deler;					   // wie deelt de kaarten
	int troef_bepaler;			   // deler + 1
	int troef;					   // kleur troef
	int slagen_teller;			   // telling aantal slagen
	int slagen_gewonnen[2];		   // [wij, zij]
	int slag_winnaar[8];		   // [8 slagen]
	int slag_kleur_gevraagd[8];	   // [8 slagen]
	int slag_beginner[8];		   // [8 slagen]
	int slag_ad_beurt[8];		   // [8 slagen]
	int spel_status;			   // status van het spel, enum spel_status
	int tel_speler_gespeeld;	   // aantal spelers die uitgekomen zijn op tafel
	bool is_nat[2];				   // [wij, zij]
	bool is_mars[2];			   // [wij, zij]
	bool gemaakt[2];			   // [wij, zij]
	bool gespeeld[2];			   // [wij, zij]
	bool roemgemaakt;			   // is er roem in de slag ?
	int tekst_roem;				   // welke roem is er behaald ?
	// sturing
	bool slag_berekend;			   // is de slag berekend?		
	bool slag_continue;            // wordt er op F7 geklikt?, ga dan verder
	bool manche_berekend;          // is de manche berekend ?
	bool manche_continue;          // wordt er op F6 geklikt?, ga dan verder
	bool spel_continue;            // wordt er op F5 geklikt?, ga dan verder
	// analysedata
	int kaarten_mag_spelen[8];	// de kaarten die gespeeld mogen worden van de speler
	int tel_mag_spelen;			// het aantal kaarten dat gespeeld mag worden
	bool kaarten_ms_winnaar[8]; // is een kaart een winnaar van de slag?
	int speelkleur;				// slag_kleur_gevraagd[slagen_teller];
	int trf1;					// tel troeven speler => Al_tel_kleur(troef, kaarten_mag_spelen, 8);
	int trf2;					// tel troeven overige spelers die nog moeten bij spelen => Al_tel_kleur(troef, filter, 32);
	int trf3;					// tel troeven tafel; => Al_tel_kleur(troef, tafel_kaarten, 4);
	int trft;					// troeven nog in het spel => trf1+tfr2+trf3
	int trfidx1;				// index hogere troef van de speler => (trf1 > 0) ? Al_vind_hogere_kaart(troef, kaarten_mag_spelen, 8) : NO_VALUE;
	int trfidx2;				// index hogere troef van de overige spelers die nog moeten bij spelen => (trf2 > 0) ? Al_vind_hogere_kaart(troef, filter, 32) : NO_VALUE;
	int trfidx3;				// index hogere troef van de tafel => (trf3 > 0) ? Al_vind_hogere_kaart(troef, tafel_kaarten, 4) : NO_VALUE;
	int trfrng1;				// rang van de hogere troef van de speler => (trf1 > NO_VALUE) ? spel_rang[kaarten_mag_spelen[trfidx1]] : NO_VALUE;
	int trfrng2;				// rang van de hogere troef van de overige spelers die nog moeten bijspelen => (trf2 > NO_VALUE) ? spel_rang[trfidx2] : NO_VALUE;
	int trfrng3;				// rang van de hogere troef van de tafel => (trf3 > NO_VALUE) ? spel_rang[tafel_kaarten[trfidx3]] : NO_VALUE;
	int t1;						// tel kaarten in een kleur van de speler => klr == troef ? trfidx1 : Al_tel_kleur(klr, kaarten_mag_spelen, 8);
	int t2;						// tel kaarten in een kleur van de overige spelers die nog moeten bijspelen => klr == troef ? trfidx2 : Al_tel_kleur(klr, filter, 32);
	int t3;						// tel kaarten in een kleur van de tafel => klr == troef ? trfidx3 : Al_tel_kleur(klr, tafel_kaarten, 4);
	int tt;						// kaarten in een kleur nog in het spel => t1+t2+t3
	int idx1;					// index hogere kaart in een kleur van de speler => (t1 > 0) ? klr == troef ? trfidx1 : Al_vind_hogere_kaart(klr, kaarten_mag_spelen, 8) : NO_VALUE;
	int idx2;					// index hogere kaart in een kleur van de overige spelers die nog moeten bijspelen => (t2 > 0) ? klr == troef ? trfidx2 : Al_vind_hogere_kaart(klr, filter, 32) : NO_VALUE;
	int idx3;					// index hogere kaart in een kleur van de tafel => (t3 > 0) ? klr == troef ? trfidx3 : Al_vind_hogere_kaart(klr, tafel_kaarten, 4) : NO_VALUE;
	int rng1;					// rang van de hogere kaart in een kleur van de speler => (t1 > NO_VALUE) ? klr == troef ? trfrng1 : spel_rang[kaarten_mag_spelen[idx1]] : NO_VALUE;
	int rng2;					// rang van de hogere kaart in een kleur van de overige spelers die nog moeten bijspelen = (t2 > NO_VALUE) ? klr == troef ? trfrng2 : spel_rang[idx2] : NO_VALUE;
	int rng3;					// rang van de hogere kaart in een kleur van de tafel = (t3 > NO_VALUE) ? klr == troef ? trfrng3 : spel_rang[tafel_kaarten[idx3]] : NO_VALUE;
	int tegen1;					// eerst volgende tegenstander die nog een kaart moet bijspelen
	int tegen2;					// de volgende tegenstander die nog een kaart moet bijspelen
	int maat;					// de medespeler
	bool kan_troef_bijspelen;	// kunnen de tegenstander(s) die nog moeten bijspelen een troef spelen ?
	bool kan_klr_introeven;		// kan de gespeelde kleur worden ingetroefd door de tegenstander(s) die nog moeten bijspelen ?
};
