#include <stdio.h>
#include <stdlib.h>

#define PROFESSORES_OFFSET -1
#define ESCOLAS_OFFSET 99
#define NUMERO_PROFESSORES 100
#define NUMERO_ESCOLAS 50

struct Escola {
    int id;
    int vagas;
    int habilitacao1;
    int habilitacao2;
    int vagasOcupadas;
    int professor1;
    int professor2;
};

struct Professor {
    int id;
    int habilitacao;
    int escola;
    int preferencias[4];
};

// Globais
int professoresOcupados = 0;
int escolasComProf = 0;
struct Professor Professores[100];
struct Escola Escolas[50];

void ocupaEscola(int id) {
    if (Escolas[id].vagas == 1) {
        Escolas[id].vagasOcupadas++;
        escolasComProf++;
    } else {
        if (Escolas[id].vagasOcupadas > 0) {
            Escolas[id].vagasOcupadas++;
        } else {
            Escolas[id].vagasOcupadas++;
            escolasComProf++;
        }
    }
}

void algoritmoGaleShapley() {
    int loop = 1;

    while (loop) {
        loop = 0;

        for (int i = 0; i < NUMERO_PROFESSORES; i++) {
            struct Professor profAtual = Professores[i];
            // professor nao alocado
            if(profAtual.escola == 0) {
                // iterar sobre as preferencias dele
                for (int j = 0; j < 4; j++) {
                    // alguns professores tem menos que 5 preferencias
                    // se a preferencia for 0, rompe loop
                    if (profAtual.preferencias[j] == 0) break;

                    struct Escola escolaPref = Escolas[profAtual.preferencias[j] -1];
                    if (profAtual.habilitacao >= escolaPref.habilitacao1) {
                        if (Escolas[profAtual.preferencias[j] -1].professor1 == 0) {
                            Escolas[profAtual.preferencias[j]-1].professor1 = profAtual.id;
                            Professores[i].escola = escolaPref.id;
                            professoresOcupados++;
                            ocupaEscola(escolaPref.id);
                            loop = 1;
                            printf("professor %d ocupou escola %d na vaga 1\n", profAtual.id, escolaPref.id);
                            printf("preferencia: %d escola %d\n", profAtual.preferencias[j], Escolas[profAtual.preferencias[j]-1].id);
                            printf("professor %d: escola = %d\n escola %d: professor1 %d\n", Professores[i].id, Professores[i].escola, Escolas[profAtual.preferencias[j]-1].id, Escolas[profAtual.preferencias[j]-1].professor1);
                            break;
                        } else if (profAtual.habilitacao > Professores[Escolas[profAtual.preferencias[j] -1].professor1 - 1].habilitacao) {
                            printf("professor %d ocupou escola %d no lugar de %d na vaga 1\n", profAtual.id, escolaPref.id, Professores[Escolas[profAtual.preferencias[j] -1].professor1 -1].id);
                            Professores[i].escola = escolaPref.id;
                            Professores[Escolas[profAtual.preferencias[j]-1].professor1 -1].escola = 0;
                            Escolas[profAtual.preferencias[j]-1].professor1 = profAtual.id;
                            loop = 1;
                            printf("professor %d: escola = %d\n escola %d: professor1 %d\n", Professores[i].id, Professores[i].escola, Escolas[profAtual.preferencias[j]-1].id, Escolas[profAtual.preferencias[j]-1].professor1);
                            break;
                        }
                    }
                    if (profAtual.habilitacao >= escolaPref.habilitacao2 && escolaPref.vagas > 1) {
                        if (Escolas[profAtual.preferencias[j] - 1].professor2 == 0) {
                            Escolas[profAtual.preferencias[j] - 1].professor2 = profAtual.id;
                            Professores[i].escola = escolaPref.id;
                            professoresOcupados++;
                            ocupaEscola(escolaPref.id);
                            printf("professor %d ocupou escola %d na vaga 2\n", profAtual.id, escolaPref.id);
                            printf("professor %d: escola = %d\n escola %d: professor2 %d\n", Professores[i].id, Professores[i].escola, Escolas[profAtual.preferencias[j] - 1].id, Escolas[profAtual.preferencias[j] - 1].professor2);
                            loop = 1;
                            break;
                        } else if (profAtual.habilitacao > Professores[Escolas[profAtual.preferencias[j] - 1].professor2 - 1].habilitacao) {
                            printf("professor %d ocupou escola %d no lugar de %d na vaga 2\n", profAtual.id, escolaPref.id, Professores[Escolas[profAtual.preferencias[j] -1].professor1-1].id);
                            Professores[i].escola = escolaPref.id;
                            Professores[Escolas[profAtual.preferencias[j] - 1].professor2 - 1].escola = 0;
                            Escolas[profAtual.preferencias[j] - 1].professor2 = profAtual.id;
                            loop = 1;
                            printf("professor %d: escola = %d\n escola %d: professor2 %d\n", Professores[i].id, Professores[i].escola, Escolas[profAtual.preferencias[j]-1].id, Escolas[profAtual.preferencias[j]-1].professor2);
                            break;
                        }
                    }
                }
            }
        }

        if (!loop) break;
    }
}

void imprimeProfessores() {
    for (int i = 0; i < NUMERO_PROFESSORES; i++) {
        struct Professor profAtual = Professores[i];

        if (profAtual.escola == 0) {
            // printf("Professor %d sem escola\n", profAtual.id);
        } else {
            printf("Professor %d -> Escola %d\n", profAtual.id, profAtual.escola);
        }
    }

    printf("Professores Ocupados: %d\n", professoresOcupados);
    printf("Escolas com ao menos 1 professor: %d\n", escolasComProf);
}

int lerArquivo() {
    FILE *fp = fopen("data/entradaProj3TAG.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }

    for (int i = 0; i < NUMERO_PROFESSORES; i++) {
        int professor, hab_professor, e1, e2, e3, e4;
        fscanf(fp, "(P%d, %d): (E%d, E%d, E%d, E%d)\n", &professor, &hab_professor, &e1, &e2, &e3, &e4);
        Professores[i].escola = 0;
        Professores[i].id = professor;
        Professores[i].habilitacao = hab_professor;
        Professores[i].preferencias[0] = e1;
        Professores[i].preferencias[1] = e2;
        Professores[i].preferencias[2] = e3;
        Professores[i].preferencias[3] = e4;
    }

    for (int i = 0; i < NUMERO_ESCOLAS; i++) {
        int escola, vaga1, vaga2;
        fscanf(fp, "(E%d):(%d):(%d)\n", &escola, &vaga1, &vaga2);

        Escolas[i].id = escola;
        Escolas[i].habilitacao1 = vaga1;
        Escolas[i].habilitacao2 = vaga2;

        if (vaga2 == 0) {
            Escolas[i].vagas = 1;
        } else {
            Escolas[i].vagas = 2;
        }

        Escolas[i].vagasOcupadas = 0;
    }

    return 1;
}

int main(void) {
    if (!lerArquivo()) return 1;

    algoritmoGaleShapley();
    imprimeProfessores();

    return 0;
}
