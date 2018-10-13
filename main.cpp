#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

// C++ (gcc 6.3)

/**
 * Procura um char em um std::set de char
 */
inline bool procuraCharSet(std::set<char> vet, char chave) {
    return vet.find(chave) != vet.end();
}

/**
 * Mostra a matriz de set
 */
void imprimirMatrizSet(std::vector<std::vector<std::set<char>>> M, int linhas, int colunas) {
    std::set<char> ::iterator iterador;
    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < colunas; c++) {
            if (M[l][c].size() > 0) {
                std::cout << " ";
                for (iterador = M[l][c].begin(); iterador != M[l][c].end(); ++iterador) {
                    std::cout << *iterador;
                }
                std::cout << "\t";
            } else {
                std::cout << " -\t";
            }
            printf("");
        }
        printf("\n");
    }
}

/**
 * Procura a menor subcadeia em um set
 */
inline std::set<char> verificaMenorSubcadeiaSet(std::string cadeia, std::vector<std::vector<char>> regras, int caracterCadeia) {
    std::set<char> resp;
    for (int i = 0; i < regras.size(); i++) {
        if (regras[i][1] == cadeia[caracterCadeia]) {
            resp.insert(regras[i][0]);
        }
    }
    return resp;
}

/**
 * Matriz bidimensional de vector<vector<set<char>>>
 * 
 *  Verifica menorSubcadeia.
 */
inline bool validaCadeia(std::string raiz, std::vector<char> naoTerminais, std::vector<char> terminais, std::vector<std::vector<char>> regras, std::string cadeia, std::set<char> rt[], char rnt[][255], std::vector<std::vector<char>> regrasTerminais, std::vector<std::vector<char>> regrasNaoTerminais) {
    //Tamanho da cadeia
    int n = cadeia.size();
    int qrnt = regrasNaoTerminais.size();

    //Tabela de busca
    std::vector<std::vector<std::set<char>>> tabela(n, std::vector<std::set<char>>(n));

    //Preenche a diagonal principal
    for (int i = 0; i < n; i++) {
        tabela[i][i] = rt[cadeia[i]];
    }

//    imprimirMatrizSet(tabela, n, n);

    for (int i = 2; i <= n; i++) {
        for (int inicio = 0; inicio <= n - i; inicio++) {
            int fim = inicio + i - 1;
            for (int meio = inicio; meio <= fim - 1; meio++) {
                for (int m = 0; m < qrnt; m++) {
                    if (procuraCharSet(tabela[inicio][meio], regrasNaoTerminais[m][1])) {
                        if (procuraCharSet(tabela[meio + 1][fim], regrasNaoTerminais[m][2])) {
                            //Adiciona o não terminal da regra na lista 
                            tabela[inicio][fim].insert(regrasNaoTerminais[m][0]);
                        }
                    }
                }
            }
        }
    }
    //imprimirMatriz(tabela, n, n);

    return procuraCharSet(tabela[0][n - 1], raiz[0]);
}

inline void testarCadeias(int instancia, std::string raiz, std::vector<char> naoTerminais, std::vector<char> terminais, std::vector<std::vector<char>> regras, std::vector<std::string> cadeias, std::set<char> rt[], char rnt[][255], std::vector<std::vector<char>> regrasTerminais, std::vector<std::vector<char>> regrasNaoTerminais) {
    //mensagem de saída
    std::cout << "Instancia " << instancia << std::endl;
    for (int i = 0; i < cadeias.size(); i++) {

        if (validaCadeia(raiz, naoTerminais, terminais, regras, cadeias[i], rt, rnt, regrasTerminais, regrasNaoTerminais)) {
            std::cout << cadeias[i] << " e uma palavra valida" << std::endl;
        } else {
            std::cout << cadeias[i] << " nao e uma palavra valida" << std::endl;
        }
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    //Comente a duas linhas abaixo para submeter ao spoj
    //Declara o arquivo a ser aberto
    std::ifstream cin("et.in");

    cin.is_open();


    //Contador de instacias
    int instancia = 1;

    std::string linha;
    //getline(cin, linha);
    while (getline(cin, linha)) {

        //Dados da gramática
        //simbolo inicial
        std::string raiz;

        //Vetor de char para os não terminais
        std::vector<char> naoTerminais;

        //Vetor de char para os terminais        
        std::vector<char> terminais;

        //Declara a tabela das regras terminais e instancia as listas

        std::set<char> rt[255];

        //Declara a tabela das regras nao terminais
        char rnt[255][255];

        std::vector<std::vector <char>> regras;
        std::vector<std::vector <char>> regrasNaoTerminais;
        std::vector<std::vector <char>> regrasTerminais;
        std::vector<std::string> cadeias;

        //atribuição raiz           
        raiz = linha;

        //leitura dos não terminais
        getline(cin, linha);
        for (int i = 0; i < linha.size(); i++) {
            naoTerminais.push_back(linha[i]);
        }
        linha = "";

        //leitura dos terminais
        getline(cin, linha);
        for (int i = 0; i < linha.size(); i++) {
            terminais.push_back(linha[i]);
        }
        linha = "";

        //leitura das regras        
        getline(cin, linha);
        // A -> AB      A -> B         
        // 0123456      012345
        // 0    56      0    5
        while ((linha[0] != '#') && (linha[5] != '#')) {
            //Terminal
            if (linha.size() == 6) {
                std::vector<char> regra;
                regra.push_back(linha[0]);
                regra.push_back(linha[5]);
                regras.push_back(regra);

                regrasTerminais.push_back(regra);
                //Insere as regras terminais no conjunto
                rt[linha[5]].insert(linha[0]);
            } else {
                std::vector<char> regra;
                regra.push_back(linha[0]);
                regra.push_back(linha[5]);
                regra.push_back(linha[6]);
                regras.push_back(regra);
                //Lista dos regras não terminais
                regrasNaoTerminais.push_back(regra);
                //marca a matriz dos não terminais
                rnt[linha[5]][linha[6]] = linha[0];
            }
            getline(cin, linha);
        }

        linha = "";

        //cadeias
        getline(cin, linha);
        while (linha != "#") {
            cadeias.push_back(linha);
            getline(cin, linha);
        }
        linha = "";

        //Testa a cadeia
        testarCadeias(instancia, raiz, naoTerminais, terminais, regras, cadeias, rt, rnt, regrasTerminais, regrasNaoTerminais);

        //Limpa terminais
        for (int i = 0; i < regrasTerminais.size(); i++) {
            rt[regrasTerminais[i][1]].clear();
        }

        //Limpa nao terminais                
        for (int i = 0; i < regrasNaoTerminais.size(); i++) {
            rnt[regrasNaoTerminais[i][1]][regrasNaoTerminais[i][2]] = -1;
        }

        naoTerminais.clear();
        terminais.clear();
        regras.clear();
        regrasTerminais.clear();
        regrasNaoTerminais.clear();
        cadeias.clear();

        instancia += 1;
    }
    return 0;
}
