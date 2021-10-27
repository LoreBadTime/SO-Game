#include <stdio.h>

int main() {

    float ImportoOggetto;
    float SCCONTO;
    float scontoapp;
    float ImportoScontato;
    float ImportoTotale

        printf("\nInserire prezzo in euro dell'articolo\n");
        scanf("%f",&ImportoOggetto);
        printf("\nInserire lo sconto in %%\n");
        scanf("%f",&SCONTO);

            scontoapp = SCONTO/100;
            ImportoScontato = ImportoOggetto * scontoapp;
            ImportoTotale = ImportoOggetto - ImportoScontato;

                printf("\nIl prezzo dell'oggetto e': %.2f euro.\n", ImportoOggetto);
                printf("Lo sconto (%.0f%%) detrae dal costo totale: %.2f euro.\n", SCONTO, ImportoScontato);
                printf("Il prezzo finale scontato e': %.2f euro.\n", ImportoTotale);

    return 0;
}
