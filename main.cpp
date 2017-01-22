#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

bool jeValidanURL(string URL);
void glavni_dio_URL(string& URL);
void validniLinkovi(string lokacija, string URL, int broj);
void sourceKodStranica(int n);
string u_string(int broj);
void filtriranje(int broj_stranica);
void prebrojavanje(int broj_stranica);
void oznacavanje(int broj_stranica);

int main()
{

    /////////////////////////////////////////////////////VALIDNOST URL///////////////////////////////////////////////////////////////
    string URL("");

    cout << "Dobrodosli na ASK program! Molimo Vas, unesite Vas URL: " << endl;
    cin >> URL;
    bool validnost = jeValidanURL(URL);

    //Osiguranje za ispravno unesen URL
    while(!validnost)
    {
        cout << "Unijeli ste URL nepostojece stranice, probajte ponovo: " << endl;
        cin >> URL;
        validnost = jeValidanURL(URL);
    }

    ofstream link;
    link.open("validniLinkovi.txt");
    link << URL << endl;
    link.close(); //spasavamo korisnicki uneseni link, te usput na ovaj nacin brisemo sve sto smo imali otprije u ovaj link

    //reduciranje URL za dobavljanje glavnog dijela URL-a (primjer: https://en.wikipedia.org/wiki/Black_Holes => https://en.wikipedia.org
    glavni_dio_URL(URL);
    cout << URL << endl;

    ///////////////////////////////////////////////////////DOWNLOAD STRANICE I VEZANIH LINKOVA////////////////////////////////////////////////////////////
    int broj_stranica;
    cout << "Koliko zelite stranica da se dodatno otvori sa vaseg linka: ";
    cin >> broj_stranica;

    string stranica = "test.txt";
    validniLinkovi(stranica, URL, broj_stranica); //dobavljanje validnih linkova LINKOVI SU U FILE-u validniLinkovi.txt

    ///////////////////////////////////////////////////////DOBAVLJANJE SOURCE KOD STRANICA///////////////////////////////////////////////////////////////
    sourceKodStranica(broj_stranica);
    cout << "Prosli Downloadiranja Source kod-ova! Sada filtriranje!" << endl;
    ///////////////////////////////////////////////////////FILTRIRANJE HTML KODOVA///////////////////////////////////////////////////////////////
    filtriranje(broj_stranica);
    cout << "Prosli smo i filtriranje! Idemo sada da prebrojimo rijeci!" << endl;
    ///////////////////////////////////////////////////////IZDVAJANJE RIJECI I PREBROJAVANJE ISTIH///////////////////////////////////////////////////////////////
    prebrojavanje(broj_stranica);
    cout << "Zavrsili smo i sa prebrojavanjem! Jos da oznacimo odakle smo izvlacili rijeci!" << endl;
    ///////////////////////////////////////////////////////OZNACAVANJE STRANICA///////////////////////////////////////////////////////////////
    oznacavanje(broj_stranica);
    cout << "Podaci su spremljeni! Mozete pogledati u folderima Rijeci za vise informacija o statistici! Hvala vam na koristenju! Ugodan dan!" << endl;

    return 0;
}


bool jeValidanURL(string URL)
{
    //Funkcija koja provjerava validnost URL-a unesenog od korisnika, kao i kasnije za provjeru validnih URL-ova iz korisnikovog unosa
    ofstream Brisanje;
    Brisanje.open("test.txt");
    Brisanje << "" << endl;
    Brisanje.close();

    string stranica = "<PATHTOCURL> -o test.txt " + URL;
    system(stranica.c_str()); //Dobavljanje source koda stranice, ako postoji

    //provjera validnosti stranice (tj. gledamo da li je cURL uspio da skine source kod stranice
    ifstream Provjera;
    string tester("");
    Provjera.open("test.txt");
    getline(Provjera, tester); //dobavljanje prve linije HTML source-koda, ako postoji
    Provjera.close();

    if(tester != "")
        return true;
    else
        return false;
}


void glavni_dio_URL(string& URL)
{
    //Funkcija koja reducira korisnicki unos URL-a zbog ekstenzija linkova
    int broj_kosih = 0; //broj kosih u glavnom dijelu URL-a bi trebao biti dva, u slucaju da je https
    string glavni("");

    if(URL.substr(0, 1) == "h")
    {
        int duzina = URL.length(); //duzina URL

        for(int i = 0; i < duzina; i++)
        {
            if(URL.substr(i, 1) == "/")
                broj_kosih++;

            if(broj_kosih >= 3) //ako smo dosli do 3 kosih, tada prekidamo petlju
                break;

            glavni += URL.substr(i, 1);
        }
    }

    else
    {
        int duzina = URL.length(); //duzina URL
        for(int i = 0; i < duzina; i++)
        {
            if(URL.substr(i, 1) == "/")
                break;

            glavni += URL.substr(i, 1);
        }
    }

    URL = glavni; //sada smo na ovaj nacin reducirali nas URL
}


void validniLinkovi(string lokacija, string URL, int broj)
{
    //PRVI DIO - dobavljanje ekstenzija i linkova//

    ifstream source;
    ofstream Ispis;
    //Funkcija koja filtrira linkove iz HTML koda (source kod) (moguci potpuni linkovi ili ekstenzije)

    //Otvaranje dokumenata i pripreme
    //Link - za otvaranje Source Kod-a i filtriranje linkova
    //source - za stvaranje novog dokumenta link.txt koja ce da sadrzi sve linkove iz source kod-a
    source.open(lokacija.c_str());
    Ispis.open("linkovi.txt");
    string linija;
    const int DIO = 9; //duzina <a href=" stringa
//    string link(""); //pomocni string za spasavanja linkova

    //int brojac = 0; //TEST

    //Prolazak kroz dokument i trazenja <a href=" tag-a KOD ZA FILTRIRANJE LINKOVA IZ SOURCE KODA
    while(getline(source, linija)) //preuzimanje linije Source kod-a
    {
        int duzina = linija.length(); //uzimanje duzine stringa linije

        for(int i = 0; i < duzina - DIO; i++) //prolazak kroz string znak po znak uzimajuci 9 znakova od i-tog znaka
        {
            if(linija.substr(i, 9) == "<a href=\"")
            {
                int pozicija = i + 9; //pocetak linka u anchor tag-u (morao sam, da ne bih poremetio for petlju

                //ispis linka
                while(linija.substr(pozicija, 1) != "\"") //sada uzimamo link unutar anchor taga
                {
                    Ispis << linija.substr(pozicija, 1);
                    pozicija++;
                }
                //brojac++;
                Ispis << endl;
            }
        }
    }

    //cout << brojac << endl; //TEST

    //Zatvaranje dokumenata
    Ispis.close();
    source.close();


    //DRUGI DIO FUNKCIJE - trazenja validnih linkova//
    ifstream podaci;
    ofstream validni;
    podaci.open("linkovi.txt"); //otvaranje stranice sa linkovima
    validni.open("validniLinkovi.txt", ios_base::app);

    string ekstenzija("");
    string kompletanLink(""); //URL sa ekstenzijom
    int broj_linkova = 0; //koliko korisnik zeli linkova da mu se skine
    bool postoji = false;

    //trazenja validnih linkova
    while(!podaci.eof() and broj_linkova < broj)
    {
        postoji = false;
        getline(podaci, ekstenzija);

        if(jeValidanURL(ekstenzija))
        {
            validni << ekstenzija << endl;
            postoji = true;
            broj_linkova++;
        }

        //U slucaju da je ekstenzija u pitanju, onda provjeravamo kompletan link sa glavnim dijelom URL-a
        if(!postoji and ekstenzija != "/")
        {
            kompletanLink = URL + ekstenzija;
            if(jeValidanURL(kompletanLink))
            {
                validni << kompletanLink << endl;
                broj_linkova++;
            }

        }
    }

    podaci.close();
    validni.close();
}


string u_string(int broj)
{
    //Funkcija koja pretvara cijeli broj u string
    string strcifre[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int intcifre[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    string strbroj(""); //ovdje cemo spasavati cifru po cifru cijelog broja

    if(broj == 0)
    {
        strbroj += strcifre[0];
    }

    int jedinica; //spasavamo cifru sa desne strane broja
    int pozicija;

    while(broj > 0)
    {
        jedinica = broj % 10; //dobavljamo cifru
        for(int i = 0; i < 10; i++)
            if(jedinica == intcifre[i]) //gledamo koju poziciju zauzima nasa cifra
                pozicija = i;

        strbroj = strcifre[pozicija] + strbroj;
        broj /= 10;
    }

    return strbroj;
}


void sourceKodStranica(int n)
{
    //Funkcija koja dobavlja source kod validnih linkova iz validniLinkovi.txt, korelacija validni linkovi i broj html source koda od 0 do broja zeljenih stranica
    ifstream URLstranica;
    URLstranica.open("validniLinkovi.txt");
    string url("");
    int broj = 0; //radi distinkcije

    //dobavljanje linkova i pomocu cURL, dobavljamo stranice
    while(!URLstranica.eof() and broj <= n)
    {
        getline(URLstranica, url);
        string brojka = u_string(broj); //u_string funkcija pretvara broj u integer
        string stranica = "<PATHTOCURL> -o Stranice/stranica" + brojka + ".txt " + url;
        system(stranica.c_str());

        broj++;
    }

    URLstranica.close();
}


void filtriranje(int broj_stranica)
{
    //Funckija koja filtrira source kod stranice, uklanjajuci tagove i script-e (JavaScript)
    int broj = 0;

    //Idemo sada kroz folder Stranice i otvaramo posebno fajlove do onoliko koliko je korisnik unio zeljeni broj stranica ILI  do koliko postoji stranica
    while(broj <= broj_stranica)
    {
        ifstream Stranica;
        ofstream Tekst;

        //dobavljanje lokacije
        string lokacija = "Stranice/stranica" + u_string(broj) + ".txt";
        string tekstualni_fajl = "Tekstovi/tekst" + u_string(broj) + ".txt";
        cout << lokacija << endl;
        Stranica.open(lokacija.c_str());
        Tekst.open(tekstualni_fajl.c_str());

        //ako smo uspjesno otvorili fajl
        if(!Stranica.fail())
        {
            string linija;
            bool jeTag = false; //pomocni bool koji ce odrediti kada mozemo pisati, a kada ne (tj. ne pisemo unutar tagova nista)
            bool postojiScript = false; //isto kao tag, samo sto vrijedi za <script>
            bool jeScript = false;

            while(getline(Stranica, linija))
            {
                int duzina = linija.length();
                const int duzina_scripta = 7; //duzina <script

                //otklanjanje tagova
                for(int i = 0; i < duzina; i++)
                {
                    //provjeravamo da li imamo script
                    if(i < duzina - duzina_scripta)
                    {
                        if(linija.substr(i, duzina_scripta) == "<script")
                            jeScript = true;
                        else if(linija.substr(i, duzina_scripta) == "</scrip")
                        {
                            jeScript = false;
                            i = i + duzina_scripta + 2;
                        }
                    }

                    //u slucaju da nismo unutar scripta, provjeravamo dalje za tagove
                    if(!jeScript)
                    {
                        if((linija.substr(i, 1) == "<" or linija.substr(i, 1) == "{") and jeTag == false) //u slucaju da postoji ovaj znak unutar teksta
                            jeTag = true;
                        else if(linija.substr(i, 1) == ">" and jeTag == true)
                            jeTag = false;

                        //ovaj uslov se potrudi da izdvoji samo slova i neke simbole poput ' i -
                        if(i < duzina and !jeTag and ((linija.at(i) >= 'A' and linija.at(i) <= 'Z') or (linija.at(i) >= 'a' and linija.at(i) <= 'z') or linija.at(i) == ' ' or linija.at(i) == '-' or linija.at(i) == '\'' or linija.at(i) == ''))
                            Tekst << linija.substr(i, 1);
                    }
                }

                Tekst << endl;
            }

            Stranica.close();
        }

        broj++;
        Tekst.close();
    }
}


void prebrojavanje(int broj_stranica)
{
    ofstream rezultat; //krajnji rezultat prebrojavanja
    int broj = 0;
    vector<string> strglavni; //SVEUKUPNI REZULTAT od svih otvorenih stranica
    vector<int> intglavni;
    vector<string> strsporedni; //Sluzi za pojedinacne rezultate prebrojavanja rijeci
    vector<int> intsporedni;
    string rijec;
    string paragraf;

    while(broj <= broj_stranica)
    {
        ifstream tekst;
        string lokacija = "Tekstovi/tekst" + u_string(broj) + ".txt"; //uzimamo sada lokacija tekstualnog fajla
        tekst.open(lokacija.c_str());
        cout << lokacija << endl;

        if(!tekst.fail())
        {
            while(getline(tekst, paragraf))
            {
                if(paragraf != "") //u slucaju da smo dobili praznu liniju, na ovaj nacin je rijesen bug prekida programa zbog length
                {
                    int duzina = paragraf.length();

                    for(int i = 0; i < duzina; i++)
                    {
                        rijec = "";

                        //dobavljamo rijec iz paragrafa
                        while(paragraf.at(i) != ' ')
                        {
                            rijec += paragraf.substr(i, 1);
                            i++;

                            if(i == duzina)
                                break;
                        }

                        //sada gledamo da li unutar vektora imamo tu rijec
                        if(strsporedni.size() != 0)
                        {
                            bool jePrisutan = false; //pretpostavljamo da ovu rijec do sada nismo nasli
                            for(int i = 0; i < strsporedni.size(); i++)
                            {
                                if(rijec == strsporedni[i]) //ako smo nasli, tada je rijec prisutna u nasem vektoru
                                    jePrisutan = true;
                            }

                            if(!jePrisutan) //ako nema rijeci u vektoru, dodajemo ga i brojimo od jedan u drugom, paralelnom vektoru
                            {
                                strsporedni.push_back(rijec);
                                intsporedni.push_back(1);
                            }

                            else
                            {
                                for(int i = 0; i < strsporedni.size(); i++)
                                {
                                    if(rijec == strsporedni[i])
                                        intsporedni[i] += 1;
                                }
                            }
                        }

                        else //u slucaju da je prvi put da dolazimo do rijeci
                        {
                            strsporedni.push_back(rijec);
                            intsporedni.push_back(1);
                        }
                    }
                }
            }

            vector<string> strvalidni;
            vector<int> intvalidni;

            //filtriranje sporednog vektora za rijeci od 5 ili vise slova
            for(int i = 0; i < strsporedni.size(); i++)
            {
                if(strsporedni[i] != "")
                {
                    if(strsporedni[i].length() >= 5)
                    {
                        strvalidni.push_back(strsporedni[i]);
                        intvalidni.push_back(intsporedni[i]);
                    }
                }
            }

            //SelectionSort sada za validni vektor
            for(int i = 0; i < intvalidni.size(); i++)
            {
                int indeks = i;

                for(int j = i + 1; j < intvalidni.size(); j++)
                {
                    if(intvalidni[j] > intvalidni[indeks])
                        indeks = j;
                }

                //swap brojevi
                int pom = intvalidni[i];
                intvalidni[i] = intvalidni[indeks];
                intvalidni[indeks] = pom;

                //swap rijeci
                string a = strvalidni[i];
                strvalidni[i] = strvalidni[indeks];
                strvalidni[indeks] = a;
            }

            if(intvalidni.size() > 5)
            {
                while(intvalidni.size() > 5) //suzavanje vektora, ukoliko je duzi od 5 (ovo u slucaju da imamo 5 ili manje rijeci koji su najveci
                {
                    intvalidni.pop_back();
                    strvalidni.pop_back();
                }
            }

            //Spremanje rijeci nakon svega procesiranog
            string gdjeSpremiti = "Rijeci/rijeci" + u_string(broj) + ".txt"; //spremanje podataka u rijec(n)
            ofstream rijeci;
            rijeci.open(gdjeSpremiti.c_str());

            //ispis rijeci
            for(int i = 0; i < strvalidni.size(); i++)
            {
                rijeci << strvalidni[i] << endl;
            }

            //ispis broj ponavljanja u fajlu
            for(int i = 0; i < intvalidni.size(); i++)
            {
                rijeci << intvalidni[i];

                if(i != intvalidni.size() - 1)
                    rijeci << endl;
            }
            rijeci.close();

            ///////////GLAVNI VEKTOR//////////
            //sada spremanje u glavni vektor, ukoliko je prazan
            if(strglavni.size() == 0)
            {
                for(int i = 0; i < strsporedni.size(); i++)
                {
                    strglavni.push_back(strsporedni[i]);
                    intglavni.push_back(intsporedni[i]);
                }
            }

            else //u slucaju da ima elemenata u GLAVNOM
            {
                bool jePrisutan;
                int velicinaGlavnog = strglavni.size();
                int pozicija;

                for(int i = 0; i < strsporedni.size(); i++)
                {
                    jePrisutan = false; //pretpostavimo da nije prisutan ovaj element u glavnom vektoru
                    for(int j = 0; j < velicinaGlavnog; j++)
                    {
                        if(strsporedni[i] == strglavni[j])
                        {
                            jePrisutan = true;
                            pozicija = j; //ako smo nasli, spasimo poziciju tog elementa u glavnom vektoru
                        }
                    }
                    //sada gledamo razlicite situacije sa ovom rijeci
                    if(jePrisutan)
                    {
                        intglavni[pozicija] += intsporedni[i]; //samo dodamo jos na taj element koliko imamo u otvorenom fajlu
                    }
                    else
                    {
                        strglavni.push_back(strsporedni[i]);
                        intglavni.push_back(intsporedni[i]);
                    }
                }
            }

            //Brisanje sporednog vektora
            int broj_rijeci = strsporedni.size();
            for(int i = 0; i < broj_rijeci; i++)
            {
                strsporedni.pop_back();
                intsporedni.pop_back();
            }
        }

        broj++;
        tekst.close();
    }

    //nakon svega, sada uradimo restrikciju glavnog vektora i pogledajmo krajnje rezultate
    vector<string> strvalidniglavni;
    vector<int> intvalidniglavni;

    //restrikcija
    for(int i = 0; i < strglavni.size(); i++)
    {
        if(strglavni[i] != "")
        {
            if(strglavni[i].length() >= 5)
            {
                strvalidniglavni.push_back(strglavni[i]);
                intvalidniglavni.push_back(intglavni[i]);
            }
        }

    }

    //SelectionSort za restrikciju od glavnog vektora
    for(int i = 0; i < intvalidniglavni.size(); i++)
    {
        int indeks = i;

        for(int j = i + 1; j < intvalidniglavni.size(); j++)
        {
            if(intvalidniglavni[j] > intvalidniglavni[indeks])
                indeks = j;
        }

        //swap brojevi
        int pom = intvalidniglavni[i];
        intvalidniglavni[i] = intvalidniglavni[indeks];
        intvalidniglavni[indeks] = pom;

        //swap rijeci
        string a = strvalidniglavni[i];
        strvalidniglavni[i] = strvalidniglavni[indeks];
        strvalidniglavni[indeks] = a;
    }

    if(intvalidniglavni.size() > 5)
    {
        while(intvalidniglavni.size() > 5) //suzavanje vektora, ukoliko je duzi od 5 (ovo u slucaju da imamo 5 ili manje rijeci koji su najveci
        {
            intvalidniglavni.pop_back();
            strvalidniglavni.pop_back();
        }
    }

    //spremamo u poseban fajl total, gdje spremamo ukupan zbir svih rijeci
    ofstream total;
    total.open("Rijeci/total.txt");
    for(int i = 0; i < intvalidniglavni.size(); i++)
    {
        total << strvalidniglavni[i] << endl;
    }

    for(int i = 0; i < intvalidniglavni.size(); i++)
    {
        total << intvalidniglavni[i] << endl;
    }

    total.close();
}


void oznacavanje(int broj_stranica)
{
    int broj = 0;

    string URL;
    ifstream URLstranica;
    URLstranica.open("validniLinkovi.txt");
    vector<string> Linkovi;

    while(getline(URLstranica, URL))
    {
        Linkovi.push_back(URL);
    }
    URLstranica.close();

    while(broj <= broj_stranica)
    {
        ifstream URLstranice;
        ofstream fajl;
        string lokacija = "Rijeci/rijeci" + u_string(broj) + ".txt";
        fajl.open(lokacija.c_str(), ios_base::app);
        fajl << endl << Linkovi[broj];
        fajl.close();
        broj++;
    }
}
