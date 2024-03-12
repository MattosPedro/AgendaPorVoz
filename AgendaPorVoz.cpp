#include <iostream>
#include <fstream>
#include <string>
#include <speechapi_cxx.h>

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

// CRIANDO STRUCT LEMBRETE
struct Lembrete
{
    string titulo;
    string texto;
    string data;
    string hora;
};
// PROCEDIMENTO QUE REQUISITA DA API A TRANSFORMAÇÃO DE UM TEXTO EM FALA
void texto_em_fala(std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechSynthesizer> requisicao_textofala, string Texto)
{
    cout << Texto + "\n";
    requisicao_textofala->SpeakTextAsync(Texto).get(); // REQUISIÇÃO DA SINTETIZAÇÃO DE TEXTO EM FALA
}
// FUNÇÃO QUE REQUISITA DA API O RECONHECIMENTO DE UMA FALA E A TRANSFORMAÇÃO DESSA FALA EM UM TEXTO
string fala_em_texto(std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognizer> requisicao_falatexto)
{
    auto resultado = requisicao_falatexto->RecognizeOnceAsync().get(); // REQUISIÇÃO DO RECONHEIMENTO DE FALA EM TEXTO
    cout << resultado->Text + "\n";
    return resultado->Text; // CONVERSÃO DO RESULTADO DO RECONHECIMENTO EM TEXTO
}

// INT MAIN 
int main()
{
    auto autenticacao = SpeechConfig::FromSubscription("4ee1c5ebd0564f8ea8b2dd4800ff38ef", "brazilsouth"); // DECLARAÇÃO DA AUTENTICAÇÃO DO RECURSO
    autenticacao->SetSpeechRecognitionLanguage("pt-BR");                                                   //  CONFIGURAÇÃO DA AUTENTICAÇÃO PARA O RECONHECIMENTO DE FALA EM PORTUGUÊS
    autenticacao->SetSpeechSynthesisLanguage("pt-BR");                                                     //  CONFIGURAÇÃO DA AUTENTICAÇÃO PARA A SINTETIZAÇÃO DE FALA EM PORTUGUÊS
    autenticacao->SetSpeechSynthesisVoiceName("pt-BR-FranciscaNeural");                                    // CONFIGURAÇÃO DE UMA VOZ ESPECÍFICA: pt-BR-AntonioNeural, pt-BR-FranciscaNeural
    //  OBS: A PERSONALIZAÇÃO DA VOZ NÃO É OBRIGATÓRIA. SE A LINHA ACIMA FOR COMENTADA, O SERVIÇO ASSUMIRÁ UMA VOZ POR DEFAULT.
    //  O NOME DAS VOZES PODEM SER ALTERADOS COM O TEMPO. CASO SEU PROGRAMA NÃO ESTEJA SINTETIZANDO A VOZ ESCOLHIDA, TENTE COMENTAR A CONFIGURAÇÃO
    //  OU PESQUISE NA DOCUMENTAÇÃO DA MICROSOFT POR VOZES DISPONÍVEIS: https://docs.microsoft.com/pt-br/azure/cognitive-services/speech-service/language-support
    auto audio_config = AudioConfig::FromDefaultMicrophoneInput();                        // DECLARAÇÃO DA ENTRADA DO MICROFONE
    auto requisicao_textofala = SpeechSynthesizer::FromConfig(autenticacao);              //  DEFINIÇÃO DO OBJETO REQUISICAO_TEXTOFALA
    auto requisicao_falatexto = SpeechRecognizer::FromConfig(autenticacao, audio_config); //  REDEFINIÇÃO DO OBJETO REQUISICAO_FALATEXTO COM AS NOVAS CONFIGURAÇÕES
// TRY-CATCH PARA TRATAR EXECUÇÕES QUE SERÃO EXECUDAS A SEGUIR
    try
    {
        // DECLARAÇÃO DE NOME
        string usuario;
        // DECLARAÇÃO DO ARQUIVO DE USUARIO
        ifstream usuario_arquivo;
        // CRIANDO ARQUIVO
        usuario_arquivo.open("usuario.txt");
        // ESTRUTURA VALUDANDO ABERTURA E CRIAÇÃO DO ARQUIVO
        if (usuario_arquivo.is_open())
        {
            usuario_arquivo >> usuario;
            usuario_arquivo.close();
            texto_em_fala(requisicao_textofala, "Bem vindo " + usuario + "!");
        }
        // DANDO INICIO AO ARQUIVO, PEDINDO PRIMEIRAMENTE O NOME DO USUARIO
        else
        {
            texto_em_fala(requisicao_textofala, "Bem vindo!");
            texto_em_fala(requisicao_textofala, "Para continuar, me informe seu nome!");
            usuario = fala_em_texto(requisicao_falatexto);
            ofstream usuario_arquivo;
            usuario_arquivo.open("usuario.txt");
            usuario_arquivo << usuario;
            usuario_arquivo.close();
            // CONVERSANDO COM O USUARIO E O CHAMANDO PELO NOME
            texto_em_fala(requisicao_textofala, "Olá " + usuario + "!");
            texto_em_fala(requisicao_textofala, "Nome registrado!");
        }
        // DECLARANDO VETOR-LEMBRETE (LIMITE)
        Lembrete lembrete[15];
        // DECLARAÇÃO DE ARQUIVO DE LEMBRETE
        ifstream lembrete_arquivo;
        lembrete_arquivo.open("lembrete.txt");
        // ESTRUTURA DE VALIDAÇÃO DO ARQUIVO LEMBRETE
        if (lembrete_arquivo.is_open())
        {
            for (int i = 0; i < 15; i++)
            {
                std::getline(lembrete_arquivo, lembrete[i].titulo);
                std::getline(lembrete_arquivo, lembrete[i].texto);
                std::getline(lembrete_arquivo, lembrete[i].data);
                std::getline(lembrete_arquivo, lembrete[i].hora);
            }
            lembrete_arquivo.close();
        }
        else
        {
            for (int i = 0; i < 15; i++)
            {
                lembrete[i].titulo = "";
                lembrete[i].texto = "";
                lembrete[i].data = "";
                lembrete[i].hora = "";
            }
        }
        string opcao = "";
        // BOOLEANO PARA VALIDAR SE PODE CONTINUAR O PROCEDIMENTO
        bool continuar = true;
        // ESTRUTURA DE REPETICAO PARA PEDIR INFORMAÇÃO AO USUARIO  
        while (continuar)
        {
            // ESTRUTURA DE 'CASE' MOSTRANDO AO USUARIO AS OPÇÕES DE USO DO PROGRAMA E PEDINDO ALGUM VALOR
            texto_em_fala(requisicao_textofala, "Ola " + usuario + "!" + " Irei lhe mostar as opções");
            texto_em_fala(requisicao_textofala, "Para criar um lembrete, diga 'criar lembrete'");
            texto_em_fala(requisicao_textofala, "Para ver os lembretes, diga 'ver lembretes'");
            texto_em_fala(requisicao_textofala, "Para ver os detalhes de um lembrete, diga 'ver detalhes'");
            texto_em_fala(requisicao_textofala, "Para alterar um lembrete, diga 'alterar lembrete'");
            texto_em_fala(requisicao_textofala, "Para excluir um lembrete, diga 'excluir lembrete'");
            texto_em_fala(requisicao_textofala, "Para sair, diga 'sair'");
            texto_em_fala(requisicao_textofala, "Diga a opção desejada!");
            // SALVANDO VALOR
            opcao = fala_em_texto(requisicao_falatexto);
            //ESTRUTURA DE VALIDACAO SE FOR PARA CRIAR
            if (opcao == "Criar lembrete.")
            {
                // PEDINDO INF AO USUARIO REFERENTE AO O QUE ELE QUER SALVAR
                texto_em_fala(requisicao_textofala, "Diga o título do lembrete");
                string titulo = fala_em_texto(requisicao_falatexto);
                texto_em_fala(requisicao_textofala, "Diga o texto do lembrete");
                string texto = fala_em_texto(requisicao_falatexto);
                texto_em_fala(requisicao_textofala, "Diga a data do lembrete");
                string data = fala_em_texto(requisicao_falatexto);
                texto_em_fala(requisicao_textofala, "Diga a hora do lembrete");
                string hora = fala_em_texto(requisicao_falatexto);
                // VALIDANDO SE NAO ATINGIU OS LIMITES DE LEMBRETES AINDA
                for (int i = 0; i < 15; i++)
                {
                    if (lembrete[i].titulo == "")
                    {
                        lembrete[i].titulo = titulo;
                        lembrete[i].texto = texto;
                        lembrete[i].data = data;
                        lembrete[i].hora = hora;
                        break;
                    }
                    // SE TIVER PASSADO DOS LIMITES, NÃO IRA SALVAR
                    else if (i == 14)
                    {
                        texto_em_fala(requisicao_textofala, "Não foi possível criar o lembrete pois o limite de lembretes foi atingido");
                    }
                }
                ofstream lembrete_arquivo;
                lembrete_arquivo.open("lembrete.txt");
                //SALVANDO NO ARQUIVO NOS DETERMINADOS LUGARES
                for (int i = 0; i < 15; i++)
                {
                    lembrete_arquivo << lembrete[i].titulo << endl;
                    lembrete_arquivo << lembrete[i].texto << endl;
                    lembrete_arquivo << lembrete[i].data << endl;
                    lembrete_arquivo << lembrete[i].hora << endl;
                }
                //SALVO COM SUCESSO
                lembrete_arquivo.close();
                texto_em_fala(requisicao_textofala, "Lembrete criado!");
            }
            //ESTRUTURA DE VALIDACAO SE FOR PARA VER LEMBRETES
            else if (opcao == "Ver lembretes." || opcao == "Lembrentes.")
            {
                // ESTRUTURA DE REPETIÇAO PARA MOSTRAR OS LEMBRETES QUE TEM
                texto_em_fala(requisicao_textofala, "Os lembretes são:");
                for (int i = 0; i < 15; i++)
                {
                    if (lembrete[i].titulo != "")
                    {
                        texto_em_fala(requisicao_textofala, lembrete[i].titulo);
                    }
                }
            }
            //ESTRUTURA DE VALIDACAO SE FOR PARA VER DETALHES DOS LEMBRETES
            else if (opcao == "Ver detalhes.") {
                // PEDIR INF AO USUARIO SOBRE QUAL LEMBRETE ELE QUER VER O DETALHE
                texto_em_fala(requisicao_textofala, "Diga o título do lembrete");
                string titulo = fala_em_texto(requisicao_falatexto);
                // VALIDANDO UM POR UM PARA IR MOSTRANDO AS INFORMAÇOES DE CADA LEMBRETE SALVO
                for (int i = 0; i < 15; i++)
                {
                    if (lembrete[i].titulo == titulo)
                    {
                        texto_em_fala(requisicao_textofala, "Título: " + lembrete[i].titulo);
                        texto_em_fala(requisicao_textofala, "Texto: " + lembrete[i].texto);
                        texto_em_fala(requisicao_textofala, "Data: " + lembrete[i].data);
                        texto_em_fala(requisicao_textofala, "Hora: " + lembrete[i].hora);
                        break;
                    }
                    // SE DESEJAR VER ALGUM ALEM DOS QUE TEM, SERA ENVIADO MENSAGEM DE QUE NAO FOI ENCONTRADO O LEMBRETE DESEJADO
                    else if (i == 14)
                    {
                        texto_em_fala(requisicao_textofala, "Não foi possível encontrar o lembrete");
                    }
                }

            }
            //ESTRUTURA DE VALIDACAO SE FOR PARA ALTERAR LEMBRETES
            else if (opcao == "Alterar lembrete.")
            {
                // PEDINDO LEMBRETE QUE VAI SER ALTERADO
                texto_em_fala(requisicao_textofala, "Diga o título do lembrete que deseja alterar");
                string titulo = fala_em_texto(requisicao_falatexto);
                // ESTRUTURA DE REPETICAO PARA VER UM POR UM, E IR NO ITEM SELECIONADO PARA ALTERAR CORRETAMENTE
                for (int i = 0; i < 15; i++)
                {
                    // SE ACHAR, ALTERAR IDENS DESEJADOS
                    if (lembrete[i].titulo == titulo)
                    {
                        texto_em_fala(requisicao_textofala, "Diga o texto do lembrete");
                        string texto = fala_em_texto(requisicao_falatexto);
                        texto_em_fala(requisicao_textofala, "Diga a data do lembrete");
                        string data = fala_em_texto(requisicao_falatexto);
                        texto_em_fala(requisicao_textofala, "Diga a hora do lembrete");
                        string hora = fala_em_texto(requisicao_falatexto);
                        lembrete[i].titulo = titulo;
                        lembrete[i].texto = texto;
                        lembrete[i].data = data;
                        lembrete[i].hora = hora;
                        ofstream lembrete_arquivo;
                        // SALVANDO EM CIMA PARA ALTERAR
                        lembrete_arquivo.open("lembrete.txt");
                        // COLOCANDO CERTINHO NO ARQUIVO
                        for (int i = 0; i < 15; i++)
                        {
                            lembrete_arquivo << lembrete[i].titulo << endl;
                            lembrete_arquivo << lembrete[i].texto << endl;
                            lembrete_arquivo << lembrete[i].data << endl;
                            lembrete_arquivo << lembrete[i].hora << endl;
                        }
                        // SALVO COM SUCESSO
                        lembrete_arquivo.close();
                        texto_em_fala(requisicao_textofala, "Lembrete alterado!");
                        break;
                    }
                    // SE TIVER IDO ALÉM DO LIMITE, RETORNAR MENSAGEM DE QUE NAO FOI ENCONTRADO
                    else if (i == 14)
                    {
                        texto_em_fala(requisicao_textofala, "Lembrete não encontrado!");
                    }
                }
            }
            //ESTRUTURA DE VALIDACAO SE FOR PARA EXCLUIR LEMBRETES
            else if (opcao == "Excluir lembrete.")
            {
                // PEDINDO INF SOBRE QUAL SERA EXLUIDO
                texto_em_fala(requisicao_textofala, "Diga o título do lembrete que deseja excluir");
                string titulo = fala_em_texto(requisicao_falatexto);
                // IRÁ VER UM POR UM PARA ACHAR LEMBRETE CORRETO
                for (int i = 0; i < 15; i++)
                {
                    //SUBSTITUINDO AS INFORMAÇOES PARA 'VAZIO' E EXLCUIR 
                    if (lembrete[i].titulo == titulo)
                    {
                        lembrete[i].titulo = "";
                        lembrete[i].texto = "";
                        lembrete[i].data = "";
                        lembrete[i].hora = "";
                        ofstream lembrete_arquivo;
                        lembrete_arquivo.open("lembrete.txt");
                        // EXCLUIR NO ARQUIVO CERTINHO
                        for (int i = 0; i < 15; i++)
                        {
                            lembrete_arquivo << lembrete[i].titulo << endl;
                            lembrete_arquivo << lembrete[i].texto << endl;
                            lembrete_arquivo << lembrete[i].data << endl;
                            lembrete_arquivo << lembrete[i].hora << endl;
                        }
                        // SUCESSO PARA EXCLUSAO
                        lembrete_arquivo.close();
                        texto_em_fala(requisicao_textofala, "Lembrete excluido!");
                        break;
                    }
                }
            }
            //ESTRUTURA DE VALIDACAO SAIR DO PROGRAMA
            else if (opcao == "Sair.")
            {
                // PROGRAMA SE DESPEDE
                texto_em_fala(requisicao_textofala, "Tchau!");
                continuar = false;
            }
            else
            {
                // CASO NAO ENTENDER, REPETIR
                texto_em_fala(requisicao_textofala, "Não entendi, tente novamente");
            }
        }
    }
    //CATCH DE EXESSÃO PARA RETORNAR NO ARQUIVO E TENTAR REAPLICAR PROGRAMA
    catch (exception e)
    {
        cout << e.what();
    }
    return 0;
}
