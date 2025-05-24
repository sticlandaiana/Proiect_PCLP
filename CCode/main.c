/* CERINTA: SNAKES AND LADDERS GAME
   REGULILE JOCULUI:
   1) Castiga prima persoana care ajunge la 100 de puncte.
   2) Scarile cresc punctajul.
   3) Serpii scad punctajul.
   5) Punctajul reprezinta defapt pozitia de pe tabla.
   4) Fiecare jucator are o singura sansa intr-o singura traversare.
   INDICATII:
   1) Jocul poate fi jucat de doi jucatori.
   2) valorile aleatorii de parcurgere a tablei se obtin prin aruncarea zarului cu 6 fete.
   3) Punctele pentru fiecare jucator vor fi mentinute folosind variabile.
   4) Jocul se va incheia dupa ce unul dintre jucatori atinge 100 de puncte.
   CONFORM TABLEI DE JOC ALEASA:
   1) Pozitiile de inceput si de final ale SCARILOR sunt: 2->23, 8->34, 20->77, 32->68, 41->79, 74->88, 82->100, 85->95.
   2) Pozitiile de final si de inceput ale SERPILOR sunt: 29->9, 38->15, 47->5, 53->33, 62->37, 86->54, 92->70, 97->25.
   3) Primele numerele din fiecare pereche de la punctele 1) si 2) ale acestei sectiuni reprezinta pozitii critice
   care maresc sau scad punctajul.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///generarea de numere random convenabile, astfel incat sa fie evitata o durata prea mare a jocului
int arunca_zarul_biasat(double bias);
///verificarea pozitiilor critice de pe tabla de joc, serpii si scarile
int cautam_serpi_si_scari(int pozitia);
///inaintarea propriu-zisa a pionilor pe tabla, prin modificarea valorilor in functie de rezultatul zarului
int runda_noua(int nr_jucatorului, int *pozitia, FILE *in, FILE *out);
///declansarea jocului si declararea castigatorului
void sa_jucam_jocul(FILE *in, FILE *out);
///ne asiguram ca jucatorii apasa z pentru a juca
void asteapta_apasare_z(FILE *in, FILE *out);
///calcularea biasului, necesar pentru generarea convenabila a numerelor random
double calculeaza_bias(int pozitie);

int main()
{
    srand(time(NULL));

    FILE *in = fopen("input2.txt", "r");
    FILE *out = fopen("output2.txt", "w");
    if (in == NULL || out == NULL)
    {
        printf("Eroare la deschiderea fisierelor.\n");
        return 1;
    }

    fprintf(out, "Sa ne jucam Snakes and Ladders!\n");

    sa_jucam_jocul(in, out);

    fclose(in);
    fclose(out);

    return 0;
}
///generarea unui numar aleator intre 1 si 6, dar cu o probabilitate mai mare de a genera
///un numar mai mare daca jucatorul este mai aproape de sfarsitul jocului
int arunca_zarul_biasat(double bias)
{
    //RAND_MAX este o constanta definita in biblioteca <stdlib.h> si reprezinta
    //valoarea maxima care poate fi generata de functia rand()
    double x = (double)rand() / RAND_MAX; //genereaza un numar aleator intre 0 si 1, prin impartirea unui numar generat clasic, la valoarea maxima care poate fi generata
    if (x < bias)
        return rand() % 3 + 4; //daca x este mai mic decat bias, se genereaza un numar intre 4 si 6
    else
        return rand() % 3 + 1; //daca x este mai mare decat bias, se genereaza un numar intre 1 si 3
}
///se calculeaza biasul pentru generarea numerelor random, in functie de pozitia jucatorului pe tabla
///biasul creste odata cu apropierea de sfarsitul jocului, facilitand progresul jucatorilor
double calculeaza_bias(int pozitie)
{
    if (pozitie < 50)
        return 0.5; ///jucatorul are un punctaj mai mic de 50, deci se afla in prima jumatate a tablei => bias mai mic
    else
        return 0.5 + 0.4 * (pozitie - 50) / 50; ///biasul este crescut in functie de distanta de la mijlocul tablei
}

int cautam_serpi_si_scari(int pozitia)
{
    switch(pozitia)
    {
///cautam SCARILE
    case 2:
        return 21; //2->23, adica pe pozitia 2 se afla o scara, deci inaintam 21 de patratele pentru a ajunge pe pozitia 23
    case 8:
        return 26; //8->34
    case 20:
        return 57; //20->77
    case 32:
        return 36; //32->68
    case 41:
        return 38; //41->79
    case 74:
        return 14; //74->88
    case 82:
        return 18; //82->100
    case 85:
        return 10; //85->95
///cautam SERPII
    case 29:
        return -20; //29->9, adica pe pozitia 29 se afla un sarpe si trebuie sa ne intoarcem 20 de patratele pentru a ajunge pe pozitia 9
    case 38:
        return -23; //38->15
    case 47:
        return -42; //47->5
    case 53:
        return -20; //53->33
    case 62:
        return -25; //62->37
    case 86:
        return -32; //86->54
    case 92:
        return -22; //92->70
    case 97:
        return -72; //97->25
    default:
        return 0;
    }
}

///furnizam pozitia cu ajutorul unui pointer
int runda_noua(int nr_jucatorului, int *pozitia, FILE *in, FILE *out)
{
    int valoarea_zarului, sarpe_sau_scara;

    asteapta_apasare_z(in, out); //tasta z apasata de jucator pentru a inainta pe tabla

    double bias = calculeaza_bias(*pozitia); //apelam functia necesara calcularii biasului
    valoarea_zarului = arunca_zarul_biasat(bias); //generam numarul random care reprezinta de fapt valoarea afisata de zar
    fprintf(out, "Zarul jucatorului %d indica valoarea %d.\n", nr_jucatorului, valoarea_zarului); //afisam valoarea zarului, in functie de jucator
    *pozitia += valoarea_zarului; //incrementam pozitia


    //urmeaza sa verificam valorile critice si sa incrementam sau decrementam pozitia dupa caz
    sarpe_sau_scara = cautam_serpi_si_scari(*pozitia); //variabila primeste valoarea returnata de functie in cazul unei SCARI sau al unui SARPE

    if (sarpe_sau_scara != 0) //pozitia este una critica, vor avea loc modificari
    {
        *pozitia += sarpe_sau_scara; //incrementam pozitia cu o valoare negativa sau pozitiva, in functie de context
        if (sarpe_sau_scara > 0) //verificam semnul numarului pentru a stii daca am avut de a face cu o SCARA sau un SARPE
            fprintf(out, "FELICITARI! Vei urca pe o scara :)\n");
        else
            fprintf(out, "UUUPS! Ai calcat pe un sarpe :(\n");
    }

    //daca scorul jucatorului devine mai mare decat 100, ne asiguram ca *pozitia va avea valoarea 100, pozitia finala de pe tabla din punct de vedere logic
    if (*pozitia > 100)
        *pozitia = 100;
    fprintf(out, "Jucatorul %d se afla pe pozitia %d.\n", nr_jucatorului, *pozitia);
    fprintf(out, "\n\n\n"); //aceste spatii determina o citire mai simpla a rezultatelor

    return *pozitia;
}

void sa_jucam_jocul(FILE *in, FILE *out)
{
    int pozitia_jucatorului_1 = 1, pozitia_jucatorului_2 = 1; //ambii jucatori pleaca de la aceeasi pozitie, adica cea de inceput

    while (pozitia_jucatorului_1 < 100 && pozitia_jucatorului_2 < 100) //cat timp pozitiile sunt mai mici decat cea maxima, acestea vor fi modificate prin aruncarea zarului
    {
        pozitia_jucatorului_1 = runda_noua(1, &pozitia_jucatorului_1, in, out); ///se modifica pozitia primului jucator
        if (pozitia_jucatorului_1 == 100) //se verifica daca este egala cu 100
        {
            fprintf(out, "Jucatorul numarul 1 a castigat!!!\n"); //in caz afirmativ, se afiseaza un mesaj corespunzator
            break; //se iese din bucla
        }

        pozitia_jucatorului_2 = runda_noua(2, &pozitia_jucatorului_2, in, out); ///se modifica pozitia celui de-al doilea jucator
        if (pozitia_jucatorului_2 == 100) //se verifica daca este egala cu 100
        {
            fprintf(out, "Jucatorul numarul 2 a castigat!!!\n"); //in caz afirmativ, se afiseaza un mesaj corespunzator
            break; //se iese din bucla
        }
    }
}

///asteapta ca utilizatorul sa apese tasta 'z' si furnizeaza feedback in functie de tastarea corecta
void asteapta_apasare_z(FILE *in, FILE *out)
{
    char input;
    do
    {

        input = fgetc(in); //citim un caracter din fisierul de intrare
        if (input == EOF) //verificam daca nu cumva fisierul de intrare e gol
        {
            fprintf(out, "Fișierul de intrare este gol.\n");
            exit(1);
        }
        else if (input != 'z' && input != '\n') //verificam daca caracterul citit nu este z sau ENTER
        {
            fprintf(out, "Ai apasat tasta '%c'. Te rog apasa 'z'!\n", input); //ii indicam utilizatorului ca ar trebui sa apese tasta z pentru a continua
        }
    }
    while (input != 'z'); //comtinuam bucla cat timp jucatorul nu apasa tasta 'z'
}
