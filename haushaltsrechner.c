#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BENUTZER_MAXSIZE 2

const char* FILE_FORMAT_IN = "(%[^,], %f, %f)\n";
const char* FILE_FORMAT_OUT = "(%s, %f, %f)\n";

typedef struct {
    char name[20];
    float ausgaben;
    float schulden;
} s_benutzer;

int loadfile(s_benutzer*);
void savefile(s_benutzer*, int);
void menu(s_benutzer*, int*);
void neuerNutzer(s_benutzer*, int*);
void neuerEinkauf(s_benutzer*, int);
void abrechnung(s_benutzer*, int);


int main() {

    printf("Willkommen im Haushaltsrechner\n\n");
    s_benutzer benutzer[BENUTZER_MAXSIZE] = {0};
    int nutzer_count = 0;
    nutzer_count = loadfile(benutzer);
    menu(benutzer, &nutzer_count);

    return 0;
}

int loadfile(s_benutzer *nutzer) {

    FILE* file = fopen("nutzer.txt", "r");

    if(file == NULL) {
        perror("Fehler beim Oeffnen der Datei zum Lesen.");
        return 0;
    }

    int i = 0;

    while(i < BENUTZER_MAXSIZE && fscanf(file, FILE_FORMAT_IN, nutzer[i].name, &nutzer[i].ausgaben, &nutzer[i].schulden) == 3) {
        printf("Nutzer %d: %s\nAusgaben: %.2f EUR\nSchulden: %.2f EUR\n\n", i + 1, nutzer[i].name, nutzer[i].ausgaben, nutzer[i].schulden);
        i++;
    }

    if (i == 0) {
        printf("Bitte neue Nutzer erstellen.\n\n");
    }

    fclose(file);
    return i;
}

void savefile(s_benutzer *nutzer, int size) {

    FILE* file = fopen("nutzer.txt", "w");

    if(file == NULL) {
        printf("Fehler beim Oeffnen der Datei zum Schreiben.\n\n");
        return;
    }

    for(int i = 0; i < size; i++) {
        fprintf(file, FILE_FORMAT_OUT, nutzer[i].name, nutzer[i].ausgaben, nutzer[i].schulden);
    }

    fclose(file);
}

void menu(s_benutzer *nutzer, int *anzahl) {

    int auswahl;

    do {
        printf("1 - Neuen Benutzer erstellen\n"
               "2 - Neuer Einkauf\n"
               "3 - Abrechnung durchfuehren\n"
               "4 - Speichern & Beenden\n\n");
                
        scanf("%d", &auswahl);

        switch(auswahl) {
            case 1:
                printf("\nFunktion: Neuen Benutzer erstellen\n");
                neuerNutzer(nutzer, anzahl);
                break;
            case 2:
                printf("\nFunktion: Neuer Einkauf\n");
                neuerEinkauf(nutzer, *anzahl);
                break;
            case 3:
                printf("\nFunktion: Abrechnung durchfuehren\n");
                abrechnung(nutzer, *anzahl);
                break;
            case 4:
                printf("\nProgramm wird beendet.\n");
                savefile(nutzer, *anzahl);
                break;
            default:
                printf("\nFehler bei der Auswahl. Bitte erneut versuchen.\n");
        }

    } while(auswahl != 4);
}

void neuerNutzer(s_benutzer *nutzer, int *anzahl) {

    if(*anzahl >= BENUTZER_MAXSIZE) {
        printf("Maximale Anzahl an Nutzern erreicht.\n\n");
        return;
    }

    printf("\nName des Nutzers: ");
    scanf("%19s", nutzer[*anzahl].name);
    nutzer[*anzahl].ausgaben = 0;
    nutzer[*anzahl].schulden = 0;
    printf("\nNeuer Nutzer %s wurde erstellt.\n\n", nutzer[*anzahl].name);

    (*anzahl)++;
}

void neuerEinkauf(s_benutzer *nutzer, int anzahl) {

    int auswahl1, auswahl2;

    if(anzahl == 0) {
        printf("\nKeine Nutzer vorhanden.\n\n");
        return;
    }

    printf("\n");

    for(int i = 0; i < anzahl; i++) {
        printf("%d - %s\n", i + 1, nutzer[i].name);
    }

    printf("\n");
    scanf("%d", &auswahl1);

    do {
        printf( "\n1 - Gemeinsamer Einkauf\n"
                "2 - Neue Schulden\n"
                "3 - Fertig\n\n");

        scanf("%d", &auswahl2);

        float wert = 0;

        if(auswahl2 == 1) {
            printf("\nWert: ");
            scanf("%f", &wert);
            nutzer[auswahl1 - 1].ausgaben += wert;
        }

        else if(auswahl2 == 2) {
            printf("\nWert: ");
            scanf("%f", &wert);
            nutzer[auswahl1 - 1].schulden += wert;
        }

    } while(auswahl2 != 3);
}

void abrechnung(s_benutzer *nutzer, int anzahl) {
    
    if(anzahl <= 0) {
        printf("\nNicht genügend Nutzer vorhanden.\n");
        return;
    }

    char datum[11];
    float ausgaben_tmp, schulden_tmp, total = 0;

    printf("\nBitte Datum eingeben: ");
    scanf("%s", datum);

    ausgaben_tmp = (nutzer[0].ausgaben - nutzer[1].ausgaben) / 2;
    schulden_tmp = nutzer[0].schulden - nutzer[1].schulden;
    total = ausgaben_tmp - schulden_tmp;

    FILE* file = fopen("abrechnungen.txt", "a");

     if(file == NULL) {
        printf("Fehler beim Oeffnen der Datei.\n\n");
        return;
    }

    if(total >= 0) {
        fprintf(file, "Abrechnung %s: %.2f EUR (%s -> %s)\n", datum, total, nutzer[1].name, nutzer[0].name);
        printf("\nAbrechnung %s: %.2f EUR (%s -> %s)\n\n", datum, total, nutzer[1].name, nutzer[0].name);
    }

    else if (total < 0) {
        total = -(total);
        fprintf(file, "Abrechnung %s: %.2f EUR (%s -> %s)\n", datum, total, nutzer[0].name, nutzer[1].name);
        printf("\nAbrechnung %s: %.2f EUR (%s -> %s)\n\n", datum, total, nutzer[0].name, nutzer[1].name);
    }

    for(int i = 0; i < BENUTZER_MAXSIZE; i++) {
        nutzer[i].ausgaben = 0;
        nutzer[i].schulden = 0;
    }

    fclose(file);
}