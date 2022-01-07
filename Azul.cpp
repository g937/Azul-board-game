#include <iostream>
#include <ctime>
#define KORONGMERET 4
using namespace std;

struct Player {
	string tabla[9][14];
	int pontszam;
	int telisorokszama = 0;
	int telioszlopokszama = 0;
	int hanyszinbolvanot = 0;
	int azonosito;
};
struct Game {
	char** pkorong;
	int korongszam;
	int asztal[6] = { 0,0,0,0,0,1 };
	int zsaktart[5] = { 20, 20, 20, 20, 20 };
	char eldobottcsempek[5] = { 0,0,0,0,0 };
	int zsakbanlevocsempek = 100;
	int ujKezdo = -1;
};
int Kpozicio(Player* jatekosok, int jatekosszam);
void Csempeletesz2(Player* jatekosok, int hanyadikjatekos, char szin, Game* jatek, int korongsorszam);
int Minuszpontok(Player* jatekosok, int hanyadikjatekos, Game* jatek);
void Pontozas(Player* jatekosok, int hanyadikjatekos, int hova, int sor);
void Muveletek2(Player* jatekosok, int hanyadikjatekos, int sor, int oszlop, Game* jatek);
bool Megfelelo2(Player* jatekosok, int hanyadikjatekos, string szin, int sor, int oszlop, bool* padlora);
void Muveletek(Game* jatek, int korongsorszam, char szin, int mennyivel);
void Padlovonal(Player* jatekosok, int hanyadikjatekos, char szin, int mennyit, Game* jatek);
int HolKezdjuk2(Player* jatekosok, int hanyadikjatekos);
bool JoHely(Player* jatekosok, int hanyadikjatekos, int hova, char szin, Game* jatek, int korongsorszam, bool* padlora);
int HolKezdjuk(Player* jatekosok, int hanyadikjatekos, int sor);
void CsempeLetesz(Player* jatekosok, int korongsorszam, char szin, int hanyadikjatekos, Game* jatek);
void zsakfeltolt(Game* jatek);
void zsakurit(char mitcsokkent, Game* jatek);
int CsempekSzama(Game* jatek) {
	int osszeg = 0;
	for (int i = 0; i < 5; i++) {
		osszeg += jatek->zsaktart[i];
	}
	jatek->zsakbanlevocsempek = osszeg;
	return jatek->zsakbanlevocsempek;
}
char huzas(Game* jatek) {
	int KisorsoltCsempeSSZ;
	char csempe;
	int CsempeSzam = CsempekSzama(jatek);

	if (CsempeSzam == 0) {
		zsakfeltolt(jatek);
		CsempeSzam = CsempekSzama(jatek);
	}
	KisorsoltCsempeSSZ = (rand() % CsempeSzam) + 1;
	if (KisorsoltCsempeSSZ <= jatek->zsaktart[0]) {
		csempe = 'A';
	}
	else if (KisorsoltCsempeSSZ <= jatek->zsaktart[0] + jatek->zsaktart[1]) {
		csempe = 'B';
	}
	else if (KisorsoltCsempeSSZ <= jatek->zsaktart[0] + jatek->zsaktart[1] + jatek->zsaktart[2]) {
		csempe = 'C';
	}
	else if (KisorsoltCsempeSSZ <= jatek->zsaktart[0] + jatek->zsaktart[1] + jatek->zsaktart[2] + jatek->zsaktart[3]) {
		csempe = 'D';
	}
	else {
		csempe = 'E';
	}
	return csempe;
}
void zsakfeltolt(Game* jatek) {
	int i;
	for (i = 0; i < 5; i++) {
		jatek->zsaktart[i] = jatek->eldobottcsempek[i];
		jatek->eldobottcsempek[i] = 0;
	}
};
void csempeeldob(char mitdob, Game* jatek, int mennyit) {
	for (int i = 0; i < mennyit; i++) {
		jatek->eldobottcsempek[mitdob - 'A'] += 1;
	}
}
char** korongfoglal(int jatekosszam, Game* jatek) {
	switch (jatekosszam) {
	case 2: jatek->korongszam = 5; break;
	case 3: jatek->korongszam = 7; break;
	case 4: jatek->korongszam = 9; break;
	default: break;
	}

	jatek->pkorong = new char* [jatek->korongszam];
	if (jatek->pkorong == NULL) {
		cout << "Hiba! Sikertelen memoriafoglalas\n";
		exit(1);
	}
	for (int j = 0; j < jatek->korongszam; j++) {
		jatek->pkorong[j] = new char[KORONGMERET];
		if (jatek->pkorong[j] == NULL) {
			cout << "Hibas memoriafoglalas!\n";
			exit(1);
		}
	}
	return jatek->pkorong;
}
void korongfeltolt(Game* jatek) {
	for (int i = 0; i < jatek->korongszam; i++) {
		for (int j = 0; j < KORONGMERET; j++) {
			jatek->pkorong[i][j] = huzas(jatek);
			zsakurit(jatek->pkorong[i][j], jatek);
		}
	}
}
int SzinSzamlal(Game* jatek, int hanyadik, char mit) {
	int osszes = 0;
	int j;
	if (hanyadik == 0) {
		switch (mit) {
		case 'A': osszes = jatek->asztal[0]; break;
		case 'B': osszes = jatek->asztal[1]; break;
		case 'C': osszes = jatek->asztal[2]; break;
		case 'D': osszes = jatek->asztal[3]; break;
		case 'E': osszes = jatek->asztal[4]; break;
		default: break;
		}
	}
	else {
		for (j = 0; j < KORONGMERET; j++) {
			if (jatek->pkorong[hanyadik - 1][j] == mit) {
				osszes++;
			}
		}
	}
	return osszes;
}
bool KorongMegfelel(Game* jatek, int korongsorszam) {
	bool jo = korongsorszam <= jatek->korongszam && korongsorszam > -1;
	if (jo == false) {
		return jo;
	}
	int j = 0;
	if (korongsorszam > 0) {
		while (j < KORONGMERET && not isalpha(jatek->pkorong[korongsorszam - 1][j])) {
			j++;
		}
	}
	return jo = j < KORONGMERET;
}
bool Megfelel(Game* jatek, int korongsorszam, char szin) {
	return SzinSzamlal(jatek, korongsorszam, szin) > 0;
}
void Korongurit(Game* jatek, int korongsorszam) {
	for (int j = 0; j < KORONGMERET; j++) {
		jatek->pkorong[korongsorszam - 1][j] = ' ';
	}
}
void Asztalurit(Game* jatek, char szin, int mennyivel) {
	jatek->asztal[szin - 'A'] -= mennyivel;
}
void Asztalfeltolt(Game* jatek, int korongsorszam, char szin) {
	char adottszin;
	for (int j = 0; j < KORONGMERET; j++) {
		if (jatek->pkorong[korongsorszam - 1][j] != szin) {
			adottszin = jatek->pkorong[korongsorszam - 1][j];
			jatek->asztal[adottszin - 'A']++;
		}
	}
}
void zsakurit(char mitcsokkent, Game* jatek) {
	jatek->zsaktart[mitcsokkent - 'A']--;
}
void KorongAsztalKiir(const Game* jatek) {
	for (int i = 0; i < jatek->korongszam; i++) {
		cout << i + 1 << ". korong:\t";
		for (int j = 0; j < KORONGMERET; j++) {
			cout << jatek->pkorong[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "Asztal:\t"; int l = 0;
	for (int k = 0; k < 6; k++) {
		switch (k) {
		case 0: cout << "A: "; break;
		case 1: cout << "B: "; break;
		case 2: cout << "C: "; break;
		case 3: cout << "D: "; break;
		case 4: cout << "E: "; break;
		case 5: cout << "K: "; break;
		default: break;
		}
		cout << jatek->asztal[k] << "\t";
	}
}
void ForduloElokeszit(Game* jatek, int jatekosszam) {
	korongfeltolt(jatek);
	KorongAsztalKiir(jatek);
}
Player* TablaFoglal(int jatekosszam) {
	int meret = jatekosszam;
	Player* mutato;
	mutato = new Player[meret];
	string board[9][14] = { { "  ", " ", " ", " ", " ", " ", " ", " ", "1", "2", "3", "4", "5", "" },
							{ "1.", "-", " ", " ", " ", " ", " ", " ", "*", "*", "*", "*", "*", "" },
							{ "2.", "-", "-", " ", " ", " ", " ", " ", "*", "*", "*", "*", "*", "" },
							{ "3.", "-", "-", "-", " ", " ", " ", " ", "*", "*", "*", "*", "*", "" },
							{ "4.", "-", "-", "-", "-", " ", " ", " ", "*", "*", "*", "*", "*", "" },
							{ "5.", "-", "-", "-", "-", "-", " ", " ", "*", "*", "*", "*", "*", "" },
							{ " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "" },
							{ " ", "_", " ", "_", " ", "_", " ", "_", " ", "_", " ", "_", " ", "_" },
							{ "-1", "-1", "-2", "-2", "-2", "-3", "-3", " ", " ", " ", " ", " ", " ", "" },

	};
	for (int j = 0; j < meret; j++) {
		mutato[j].pontszam = 0;
		mutato[j].azonosito = j;
		for (int s = 0; s < 9; s++) {
			for (int o = 0; o < 14; o++)
				mutato[j].tabla[s][o] = board[s][o];
		}
	}
	return mutato;
}
void TablaRajzol(const Player* jatekosok, int jatekosszam, int hanyadikjatekos) {
	cout << endl;
	cout << "\n" << jatekosok[hanyadikjatekos].azonosito + 1 << ". jatekos:" << '\n';
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 14; j++) {
			cout << jatekosok[hanyadikjatekos].tabla[i][j];
		}
		cout << endl;
	}
	cout << "Pontszam: " << jatekosok[hanyadikjatekos].pontszam << "\n";

}
void CsempeKivalaszt(Game* jatek, Player* jatekosok, int jatekosszam, int k) {
	int korongsorszam;
	char szin;
	bool megfelel;
	do {
		cout << "Adja meg a korong sorszamat: ";
		cin >> korongsorszam;
		megfelel = KorongMegfelel(jatek, korongsorszam);
		if (megfelel == false) {
			cout << "Nem letezo korongsorszamot adott meg." << endl;
		}
		if (megfelel == true) {
			cout << "Adja meg a szint, amit el szeretne venni: ";
			cin >> szin;
			megfelel = Megfelel(jatek, korongsorszam, szin);
			if (megfelel == false) {
				cout << "A megadott helyen nem talalhato ilyen szin." << endl;
			}
		}
	} while (!megfelel);
	CsempeLetesz(jatekosok, korongsorszam, szin, k, jatek);
	TablaRajzol(jatekosok, jatekosszam, k);
	cout << "\n";
	KorongAsztalKiir(jatek);
	cout << "\n";
}
void CsempeLetesz(Player* jatekosok, int korongsorszam, char szin, int hanyadikjatekos, Game* jatek) {
	if (korongsorszam == 0) {
		if (jatek->asztal[5] == 1) {
			Padlovonal(jatekosok, hanyadikjatekos, 'K', 1, jatek);
			jatek->asztal[5]--;
		}
	}
	int sor;
	bool jo;
	bool padlora = false;
	do {
		cout << "Adja meg hanyadik sorba szeretne letenni: ";
		cin >> sor;
		jo = JoHely(jatekosok, hanyadikjatekos, sor, szin, jatek, korongsorszam, &padlora);
		if (jo == false) {
			cout << "A megadott sorban mar szerepel a csempe./A megadott sorban jelenleg mas szint gyujt.\n";
		}
	} while (!jo);
	int mennyit = SzinSzamlal(jatek, korongsorszam, szin);
	int mennyivel = mennyit;

	if (padlora == true) {
		Csempeletesz2(jatekosok, hanyadikjatekos, szin, jatek, korongsorszam);
	}
	else {
		int mettol = HolKezdjuk(jatekosok, hanyadikjatekos, sor);
		if (sor == 0 or mettol > sor) {
			Padlovonal(jatekosok, hanyadikjatekos, szin, mennyit, jatek);
		}
		else {
			while (mennyit > 0 && mettol <= sor) {
				jatekosok[hanyadikjatekos].tabla[sor][mettol] = szin;
				mennyit--;
				mettol++;
			}
			if (mennyit > 0) {
				Padlovonal(jatekosok, hanyadikjatekos, szin, mennyit, jatek);
			}
		}
	}
	Muveletek(jatek, korongsorszam, szin, mennyivel);
}
void Muveletek(Game* jatek, int korongsorszam, char szin, int mennyivel) {
	if (korongsorszam > 0) {
		Asztalfeltolt(jatek, korongsorszam, szin);
		Korongurit(jatek, korongsorszam);
	}
	else {
		Asztalurit(jatek, szin, mennyivel);
	}

}
void Padlovonal(Player* jatekosok, int hanyadikjatekos, char szin, int mennyit, Game* jatek) {
	int hol;
	hol = HolKezdjuk2(jatekosok, hanyadikjatekos);
	if (hol < 14) {
		while (hol < 14 && mennyit>0) {
			jatekosok[hanyadikjatekos].tabla[7][hol] = szin;
			mennyit--;
			hol += 2;
		}
	}
	if (mennyit > 0) {
		csempeeldob(szin, jatek, mennyit);
	}

}
int HolKezdjuk2(Player* jatekosok, int hanyadikjatekos) {
	int j = 1;
	while (j < 14) {
		if (jatekosok[hanyadikjatekos].tabla[7][j] == "_") {
			return j;
		}
		j += 2;
	}
	return j;
}
int HolKezdjuk(Player* jatekosok, int hanyadikjatekos, int sor) {
	int j = 1;
	while (j <= sor) {
		if (jatekosok[hanyadikjatekos].tabla[sor][j] == "-") {
			return j;
		}
		else {
			j++;
		}
	}
	return j;
}
bool JoHely(Player* jatekosok, int hanyadikjatekos, int hova,
	char szin, Game* jatek, int korongsorszam, bool* padlora) {
	*padlora = false;
	bool jo = hova == 0 or jatekosok[hanyadikjatekos].tabla[hova][1] == "-" or
		jatekosok[hanyadikjatekos].tabla[hova][1][0] == szin;
	for (int j = 8; j < 13; j++) {
		if (jatekosok[hanyadikjatekos].tabla[hova][j][0] == szin) {
			return jo = false;
		}
	}
	if (jo == false) {
		int i = 1;
		while (i < 6 && jatekosok[hanyadikjatekos].tabla[i][1] != "-") {
			i++;
		}
		if (i > 5) {
			*padlora = true;
			jo = true;
		}
	}
	return jo;
}
void Csempeletesz2(Player* jatekosok, int hanyadikjatekos, char szin, Game* jatek, int korongsorszam) {
	int mennyit = SzinSzamlal(jatek, korongsorszam, szin);
	Padlovonal(jatekosok, hanyadikjatekos, szin, mennyit, jatek);
}
void Csempezes(Player* jatekosok, Game* jatek, int jatekosszam, int hanyadikjatekos) {
	int hanyadik;
	for (int i = 1; i < 6; i++) {
		int j = 1;
		int szamol = 0;
		for (j = 1; j <= i; j++) {
			if (jatekosok[hanyadikjatekos].tabla[i][j] != "-") {
				szamol++;
			}
		}
		if (szamol >= i) {
			cout << i << ". sor szerint csempez: ";
			bool jo;
			bool padlora = false;
			do {
				cout << "Adja meg melyik oszlopba szeretne elhelyezni a csempet: ";
				cin >> hanyadik;
				jo = Megfelelo2(jatekosok, hanyadikjatekos,
					jatekosok[hanyadikjatekos].tabla[i][1], i, hanyadik + 7, &padlora);
				if (jo == false) {
					cout << "A megadott oszlopban mar szerepel (ilyen szinu) csempe.\n";
				}
			} while (!jo);
			if (padlora == true) {
				Padlovonal(jatekosok, hanyadikjatekos, jatekosok[hanyadikjatekos].tabla[i][1][0], i, jatek);
				for (int j = 1; j <= i; j++) {
					jatekosok[hanyadikjatekos].tabla[i][j] = "-";
				}
			}
			else {
				Muveletek2(jatekosok, hanyadikjatekos, i, hanyadik + 7, jatek);
				Pontozas(jatekosok, hanyadikjatekos, hanyadik + 7, i);
			}
			TablaRajzol(jatekosok, jatekosszam, hanyadikjatekos);
		}
	}
	jatekosok[hanyadikjatekos].pontszam -= Minuszpontok(jatekosok, hanyadikjatekos, jatek);
	if (jatekosok[hanyadikjatekos].pontszam < 0) {
		jatekosok[hanyadikjatekos].pontszam = 0;
	}
	TablaRajzol(jatekosok, jatekosszam, hanyadikjatekos);
}
void Muveletek2(Player* jatekosok, int hanyadikjatekos, int sor, int oszlop, Game* jatek) {
	jatekosok[hanyadikjatekos].tabla[sor][oszlop] = jatekosok[hanyadikjatekos].tabla[sor][1];
	csempeeldob(jatekosok[hanyadikjatekos].tabla[sor][1][0], jatek, sor - 1);
	for (int j = 1; j <= sor; j++) {
		jatekosok[hanyadikjatekos].tabla[sor][j] = "-";
	}
}
bool Megfelelo2(Player* jatekosok, int hanyadikjatekos, string szin, int sor, int oszlop, bool* padlora) {
	bool jo = true;
	if (jatekosok[hanyadikjatekos].tabla[sor][oszlop] != "*") {
		return jo = false;
	}
	for (int j = 8; j < 14; j++) {
		if (jatekosok[hanyadikjatekos].tabla[sor][j] ==
			jatekosok[hanyadikjatekos].tabla[sor][1]) {
			jo = false;
		}
	}
	for (int i = 1; i < 6; i++) {
		if (jatekosok[hanyadikjatekos].tabla[i][oszlop] ==
			jatekosok[hanyadikjatekos].tabla[sor][1]) {
			jo = false;
		}
	}
	if (jo == false) {
		int hanybetu = 0;
		for (int l = 8; l < 13; l++) {
			if (jatekosok[hanyadikjatekos].tabla[sor][l] != "*") {
				hanybetu++;
			}
		}
		if (hanybetu >= 4) {
			*padlora = true;
			jo = true;
		}
	}
	return jo;
}
bool KorongAsztalures(Game* jatek) {
	bool ures = true;
	for (int i = 0; i < 6; i++) {
		if (jatek->asztal[i] != 0)
			return ures = false;
	}
	for (int i = 0; i < jatek->korongszam; i++) {
		for (int j = 0; j < KORONGMERET; j++) {
			if (jatek->pkorong[i][j] != ' ') {
				return ures = false;
			}
		}
	}
	return ures;
}
bool Jatekvege(Player* jatekosok, int jatekosszam) {
	for (int k = 0; k < jatekosszam; k++) {
		for (int i = 1; i < 6; i++) {
			int j = 8;
			while (j < 13 && jatekosok[k].tabla[i][j] != "*") {
				j++;
			}
			if (j >= 13) {
				return true;
			}
		}
	}
	return false;
}
void Pontozas(Player* jatekosok, int hanyadikjatekos, int hova, int sor) {
	int kapottpont = 0;
	int egyik = 0;
	int masik = 0;
	if (jatekosok[hanyadikjatekos].tabla[sor][hova] != "*") {
		kapottpont++;
	}
	int j = hova - 1;
	while (j > 7 && jatekosok[hanyadikjatekos].tabla[sor][j] != "*") {
		egyik++;
		kapottpont++;
		j--;
	}
	j = hova + 1;
	while (j < 13 && jatekosok[hanyadikjatekos].tabla[sor][j] != "*") {
		egyik++;
		kapottpont++;
		j++;
	}
	j = sor - 1;
	while (j > 0 && jatekosok[hanyadikjatekos].tabla[j][hova] != "*") {
		masik++;
		kapottpont++;
		j--;
	}
	j = sor + 1;
	while (j < 6 && jatekosok[hanyadikjatekos].tabla[j][hova] != "*") {
		masik++;
		kapottpont++;
		j++;
	}
	if (egyik > 0 && masik > 0) {
		kapottpont++;
	}
	jatekosok[hanyadikjatekos].pontszam += kapottpont;
}
int Minuszpontok(Player* jatekosok, int hanyadikjatekos, Game* jatek) {
	int minuszpont = 0;
	int kapottminuszpont = 0;
	for (int j = 1; j < 14; j += 2) {
		if (jatekosok[hanyadikjatekos].tabla[7][j] != "_") {
			switch (j) {
			case 1: minuszpont = 1; break;
			case 3: minuszpont = 1; break;
			case 5: minuszpont = 2; break;
			case 7: minuszpont = 2; break;
			case 9: minuszpont = 2; break;
			case 11: minuszpont = 3; break;
			case 13: minuszpont = 3; break;
			default: break;
			}
			kapottminuszpont += minuszpont;
			if (jatekosok[hanyadikjatekos].tabla[7][j] == "K") {
				jatek->asztal[5] = 1;
			}
			else {
				csempeeldob(jatekosok[hanyadikjatekos].tabla[7][j][0], jatek, 1);
			}
			jatekosok[hanyadikjatekos].tabla[7][j] = "_";
		}
	}
	return kapottminuszpont;
}
void Bonuszpontsegitseg(Player* jatekosok, int jatekosszam) {
	int i;
	int j;
	int telisor;
	int telioszlop;
	for (int k = 0; k < jatekosszam; k++) {
		telisor = 0;
		for (i = 1; i < 6; i++) {
			j = 8;
			while (j < 13 && jatekosok[k].tabla[i][j] != "*") {
				j++;
			}
			if (j == 13) {
				telisor++;
			}
		}
		jatekosok[k].telisorokszama = telisor;
		telioszlop = 0;
		for (j = 8; j < 13; j++) {
			i = 1;
			while (i < 6 && jatekosok[k].tabla[i][j] != "*") {
				i++;
			}
			if (i == 6) {
				telioszlop++;
			}
		}
		jatekosok[k].telioszlopokszama = telioszlop;

		for (int l = 65; l < 70; l++) {
			char szin;
			switch (l) {
			case 65: szin = 'A'; break;
			case 66: szin = 'B'; break;
			case 67: szin = 'C'; break;
			case 68: szin = 'D'; break;
			case 69: szin = 'E'; break;
			default: break;
			}
			int szindb = 0;
			for (i = 1; i < 6; i++) {
				for (j = 8; j < 13; j++) {
					if (jatekosok[k].tabla[i][j][0] == szin) {
						szindb++;
					}
				}
			}
			if (szindb == 5) {
				jatekosok[k].hanyszinbolvanot++;
			}
		}
	}
}
void Bonuszpontok(Player* jatekosok, int jatekosszam) {
	int bonuszpont;
	for (int k = 0; k < jatekosszam; k++) {
		bonuszpont = 0;
		bonuszpont = jatekosok[k].telisorokszama * 2 + jatekosok[k].telioszlopokszama * 7
			+ jatekosok[k].hanyszinbolvanot * 10;
		jatekosok[k].pontszam += bonuszpont;
		TablaRajzol(jatekosok, jatekosszam, k);
		cout << k + 1 << ". jatekos bonuszpontja: " << bonuszpont << endl;
	}
}
Player* Eredmenyek(Player* jatekosok, int jatekosszam) {
	Player* eredmenyek = new Player[jatekosszam];
	for (int i = 0; i < jatekosszam; i++) {
		eredmenyek[i] = jatekosok[i];
	}
	int k;
	int i = jatekosszam - 1;
	while (i >= 1) {
		k = 0;
		while (k < i) {
			if (eredmenyek[k].pontszam < eredmenyek[k + 1].pontszam) {
				Player csere = eredmenyek[k];
				eredmenyek[k] = eredmenyek[k + 1];
				eredmenyek[k + 1] = csere;
			}
			else if (eredmenyek[k].pontszam == eredmenyek[k + 1].pontszam) {
				if (eredmenyek[k].telisorokszama < eredmenyek[k + 1].telisorokszama) {
					Player csere = eredmenyek[k];
					eredmenyek[k] = eredmenyek[k + 1];
					eredmenyek[k + 1] = csere;
				}
			}
			k++;
		}
		i--;
	}
	return eredmenyek;
}
void Eredmenykiiras(Player* eredmenyek, Player* jatekosok, int jatekosszam) {
	int elso = eredmenyek[0].pontszam;
	int elsosorai = eredmenyek[0].telisorokszama;
	int i = 1;
	int tobbelso = 0;
	int hanyadik = 1;
	cout << "\n";
	cout << 1 << ". helyezett: " << jatekosok[eredmenyek[0].azonosito].azonosito + 1 << ". jatekos, " <<
		"pontszama: " << jatekosok[eredmenyek[0].azonosito].pontszam << endl;
	while (i < jatekosszam) {
		while (i < jatekosszam && elso == eredmenyek[i].pontszam && elsosorai == eredmenyek[i].telisorokszama) {
			cout << hanyadik << ". helyezett: " << jatekosok[eredmenyek[i].azonosito].azonosito + 1 << ". jatekos, " <<
				"pontszama: " << jatekosok[eredmenyek[i].azonosito].pontszam << endl;
			i++;
		}
		if (i < jatekosszam) {
			elso = eredmenyek[i].pontszam;
			elsosorai = eredmenyek[i].telisorokszama;
			hanyadik++;
		}
	}
}
int Kpozicio(Player* jatekosok, int jatekosszam) {
	for (int k = 0; k < jatekosszam; k++) {
		for (int i = 1; i < 14; i += 2) {
			if (jatekosok[k].tabla[7][i] == "K") {
				return k;
			}
		}
	}
}
void JatekosRendez(Player* jatekosok, Player* seged, int jatekosszam, int kinel) {
	if (kinel == 0) {
		return;
	}
	int i = 0;
	int j;
	for (j = kinel; j < jatekosszam; j++) {
		jatekosok[i] = seged[j];
		i++;
	}
	for (j = 0; j < kinel; j++) {
		jatekosok[i] = seged[j];
		i++;
	}
}
void Segedtomb(Player* jatekosok, int jatekosszam, Player* seged) {
	for (int i = 0; i < jatekosszam; i++) {
		seged[i] = jatekosok[i];
	}
}

int main() {
	srand(time(0));
	Game jatek;
	Game* pjatek;
	pjatek = new Game(jatek);
	int jatekosszam;
	bool jo;
	do {
		cout << "Adja meg a jatekosok szamat (2-4): ";
		cin >> jatekosszam;
		jo = jatekosszam > 1 && jatekosszam < 5;
		if (jo == false) {
			cout << "Ennyien nem jatszhatjak a jatekot!" << endl;
		}
	} while (!jo);
	Player* pjatekosok;
	pjatekosok = TablaFoglal(jatekosszam);
	int i, k, l, m;
	bool ures;
	korongfoglal(jatekosszam, pjatek);
	int kinel = 0;
	Player* seged = new Player[jatekosszam];
	do {
		ForduloElokeszit(pjatek, jatekosszam);
		for (l = 0; l < jatekosszam; l++) {
			TablaRajzol(pjatekosok, jatekosszam, l);
		}
		if (kinel != 0) {
			Segedtomb(pjatekosok, jatekosszam, seged);
			JatekosRendez(pjatekosok, seged, jatekosszam, kinel);
		}
		do {
			for (k = 0; k < jatekosszam; k++) {
				ures = KorongAsztalures(pjatek);
				if (ures == false) {
					cout << "\n" << pjatekosok[k].azonosito + 1 << ". jatekos valaszt: ";
					TablaRajzol(pjatekosok, jatekosszam, k);
					CsempeKivalaszt(pjatek, pjatekosok, jatekosszam, k);
				}
			}
		} while (!ures);
		kinel = Kpozicio(pjatekosok, jatekosszam);
		for (i = 0; i < jatekosszam; i++) {
			cout << "\n" << pjatekosok[i].azonosito + 1 << ". jatekos csempez: ";
			TablaRajzol(pjatekosok, jatekosszam, i);
			Csempezes(pjatekosok, pjatek, jatekosszam, i);
		}
	} while (!Jatekvege(pjatekosok, jatekosszam));
	Bonuszpontsegitseg(pjatekosok, jatekosszam);
	Bonuszpontok(pjatekosok, jatekosszam);
	Player* eredmenyek;
	eredmenyek = Eredmenyek(pjatekosok, jatekosszam);
	Eredmenykiiras(eredmenyek, pjatekosok, jatekosszam);
	delete[] seged;
	delete[] eredmenyek;
	delete pjatek;
	delete[] pjatekosok;
	return 0;
}
