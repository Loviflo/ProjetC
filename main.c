#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

float weight;
float size;
float imc;
int menu = 0;
int w,h;
int boucleIMC;
char data[20];
int returnKey = 0;
float nTaille,nPoids,nIMC;
TTF_Font *police = NULL;
TTF_Font *fontIMC = NULL;
SDL_Color CouleurTexte = {200,160,100};
SDL_Color CouleurTexteIMC = {0,0,0};
SDL_Surface *texte = NULL;
SDL_Texture *label1;
SDL_Texture *label2;
SDL_Texture *label3;
SDL_Texture *label4;
SDL_Texture *label5;
SDL_Renderer* renderer; // Déclaration du renderer
SDL_Window* window;
SDL_Rect rectangle[5]; // Déclaration du tableau contenant les cases blanches
SDL_Point mouse;
SDL_Rect menuRect1;
SDL_Rect menuRect2;
SDL_Rect menuRect3;
SDL_Rect menuRect4;
SDL_Rect menuRect5;
SDL_Texture *labelIMC1;
SDL_Texture *labelIMC2;
SDL_Texture *labelIMC3;
SDL_Texture *inputIMC;
SDL_Texture *outputIMC;
SDL_Rect textRectLabelIMC1;
SDL_Rect textRectLabelIMC2;
SDL_Rect textRectLabelIMC3;
SDL_Rect textRectSaisieIMC;
SDL_Rect textRectInputIMC;
SDL_Rect textRectOutputIMC;


void formIMC(){

    fontIMC = TTF_OpenFont("arial.ttf",24);

// Affichage première question
    texte = TTF_RenderText_Blended( fontIMC, "Entrez votre âge : ", CouleurTexteIMC );
    labelIMC1 = SDL_CreateTextureFromSurface(renderer,texte);
    SDL_QueryTexture(labelIMC1, NULL, NULL, &w, &h);
    textRectLabelIMC1.x=50;textRectLabelIMC1.y=100;textRectLabelIMC1.w=w;textRectLabelIMC1.h=h;

// Affichage deuxième question
    texte = TTF_RenderText_Blended( fontIMC, "Entrez votre poids : ", CouleurTexteIMC );
    labelIMC2 = SDL_CreateTextureFromSurface(renderer,texte);
    SDL_QueryTexture(labelIMC2, NULL, NULL, &w, &h);
    textRectLabelIMC2.x=50;textRectLabelIMC2.y=150;textRectLabelIMC2.w=w;textRectLabelIMC2.h=h;

// Affichage troisième question
    texte = TTF_RenderText_Blended( fontIMC, "Entrez votre taille : ", CouleurTexteIMC );
    labelIMC3 = SDL_CreateTextureFromSurface(renderer,texte);
    SDL_QueryTexture(labelIMC3, NULL, NULL, &w, &h);
    textRectLabelIMC3.x=50;textRectLabelIMC3.y=200;textRectLabelIMC3.w=w;textRectLabelIMC3.h=h;

    SDL_RenderCopy(renderer, labelIMC1, NULL, &textRectLabelIMC1);
    SDL_RenderCopy(renderer, labelIMC2, NULL, &textRectLabelIMC2);
    SDL_RenderCopy(renderer, labelIMC3, NULL, &textRectLabelIMC3);

    boucleIMC = 1;
    returnKey = 0;

    SDL_SetRenderDrawColor(renderer,255,255,0,120);
    textRectSaisieIMC.x=260;textRectSaisieIMC.y=98;textRectSaisieIMC.w=150;textRectSaisieIMC.h=29;
    SDL_RenderDrawRect(renderer,&textRectSaisieIMC);
    SDL_RenderPresent(renderer);
    //strcpy(data,"");
    SDL_StartTextInput();

}

void redrawMenu(){
        SDL_SetRenderDrawColor(renderer,233,139,102,255); // Couleur
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,34,56,67,255);
        SDL_RenderFillRect(renderer,&menuRect1);
        SDL_RenderFillRect(renderer,&menuRect2);
        SDL_RenderFillRect(renderer,&menuRect3);
        SDL_RenderFillRect(renderer,&menuRect4);
        SDL_RenderFillRect(renderer,&menuRect5);
        SDL_RenderCopy(renderer,label1,NULL,&menuRect1);
        SDL_RenderCopy(renderer,label2,NULL,&menuRect2);
        SDL_RenderCopy(renderer,label3,NULL,&menuRect3);
        SDL_RenderCopy(renderer,label4,NULL,&menuRect4);
        SDL_RenderCopy(renderer,label5,NULL,&menuRect5);
        SDL_RenderPresent(renderer);
                SDL_RenderClear(renderer);

}

int main(int argc, char** argv)
{

    TTF_Init();

    /* Initialisation simple */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK) < 0) // Initialisation de la SDL
    {
        printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
        return -1;
    }

{
    // Création de la fenêtre :
        window = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 790 , 800 , 0);
        if(window == NULL) // Gestion des erreurs
        {
            printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
            return -1;
        }

    // Création du renderer :
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Création du renderer
        if(renderer == NULL) // Gestion des erreurs
        {
            printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
            return -1;
        }

        SDL_SetRenderDrawColor(renderer,233,139,102,255); // Couleur
        SDL_RenderClear(renderer);
        //SDL_SetRenderDrawColor(renderer,34,56,67,255);

        menuRect1.x = 0;
        menuRect1.y = 0;
        menuRect1.w = 150;
        menuRect1.h = 50;

        menuRect2.x = 160;
        menuRect2.y = 0;
        menuRect2.w = 150;
        menuRect2.h = 50;

        menuRect3.x = 320;
        menuRect3.y = 0;
        menuRect3.w = 150;
        menuRect3.h = 50;

        menuRect4.x = 480;
        menuRect4.y = 0;
        menuRect4.w = 150;
        menuRect4.h = 50;

        menuRect5.x = 640;
        menuRect5.y = 0;
        menuRect5.w = 150;
        menuRect5.h = 50;

        police = TTF_OpenFont("arial.ttf",100);

        texte = TTF_RenderText_Blended( police, "Menu", CouleurTexte );
        label1 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( police, "IMC", CouleurTexte );
        label2 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( police, "Menu 3", CouleurTexte );
        label3 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( police, "Menu 4", CouleurTexte );
        label4 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( police, "Menu 5", CouleurTexte );
        label5 = SDL_CreateTextureFromSurface(renderer, texte);

        redrawMenu();

        SDL_RenderPresent(renderer);

        if (window)
        {
            char cont = 1; /* Détermine si on continue la boucle principale */
            SDL_Event event;
            while ( cont != 0 )
            {
                while ( SDL_PollEvent(&event) )
                {

                    if(event.type == SDL_KEYDOWN)
                    {
                        if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
                            {
                                cont = 0;
                            }
                        if (event.key.keysym.sym == SDLK_KP_ENTER)
                            {
                                returnKey = 1;
                            }
                    }

                    if(event.type == SDL_TEXTINPUT)
                    {
                        if (strchr("0123456789.",event.text.text[0])!=NULL){
                           strcat(data, event.text.text);
                        }

                    }

                    if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT){
                        mouse.x = event.button.x;
                        mouse.y = event.button.y;
                        if(SDL_PointInRect(&mouse, &menuRect1)){
                            redrawMenu();
                            SDL_SetRenderDrawColor(renderer,255,255,255,255);
                            SDL_RenderFillRect(renderer,&menuRect1);
                            SDL_RenderCopy(renderer,label1,NULL,&menuRect1);
                            SDL_RenderPresent(renderer);
                            menu = 1;
                        }
                        if(SDL_PointInRect(&mouse, &menuRect2)){
                            redrawMenu();
                            SDL_SetRenderDrawColor(renderer,255,255,255,255);
                            SDL_RenderFillRect(renderer,&menuRect2);
                            SDL_RenderCopy(renderer,label2,NULL,&menuRect2);
                            SDL_RenderPresent(renderer);
                            formIMC();
                            menu = 2;
                        }
                        if(SDL_PointInRect(&mouse, &menuRect3)){
                            redrawMenu();
                            SDL_SetRenderDrawColor(renderer,255,255,255,255);
                            SDL_RenderFillRect(renderer,&menuRect3);
                            SDL_RenderCopy(renderer,label3,NULL,&menuRect3);
                            SDL_RenderPresent(renderer);
                            menu = 3;
                        }
                        if(SDL_PointInRect(&mouse, &menuRect4)){
                            redrawMenu();
                            SDL_SetRenderDrawColor(renderer,255,255,255,255);
                            SDL_RenderFillRect(renderer,&menuRect4);
                            SDL_RenderCopy(renderer,label4,NULL,&menuRect4);
                            SDL_RenderPresent(renderer);
                            menu = 4;
                        }
                        if(SDL_PointInRect(&mouse, &menuRect5)){
                            redrawMenu();
                            SDL_SetRenderDrawColor(renderer,255,255,255,255);
                            SDL_RenderFillRect(renderer,&menuRect5);
                            SDL_RenderCopy(renderer,label5,NULL,&menuRect5);
                            SDL_RenderPresent(renderer);
                            menu = 5;
                        }
                    }

                    if(event.type == SDL_MOUSEMOTION){
                        mouse.x = event.motion.x;
                        mouse.y = event.motion.y;
                        if(menu != 1){
                            if(SDL_PointInRect(&mouse,&menuRect1)){
                                SDL_SetRenderDrawColor(renderer,100,100,100,255);
                                SDL_RenderFillRect(renderer,&menuRect1);
                                SDL_RenderCopy(renderer,label1,NULL,&menuRect1);
                                SDL_RenderPresent(renderer);
                            } else {
                                SDL_SetRenderDrawColor(renderer,34,56,67,255);
                                SDL_RenderFillRect(renderer,&menuRect1);
                                SDL_RenderCopy(renderer,label1,NULL,&menuRect1);
                                SDL_RenderPresent(renderer);
                            }
                        }
                        if(menu != 2){
                            if(SDL_PointInRect(&mouse,&menuRect2)){
                                SDL_SetRenderDrawColor(renderer,100,100,100,255);
                                SDL_RenderFillRect(renderer,&menuRect2);
                                SDL_RenderCopy(renderer,label2,NULL,&menuRect2);
                                SDL_RenderPresent(renderer);
                            } else {
                                SDL_SetRenderDrawColor(renderer,34,56,67,255);
                                SDL_RenderFillRect(renderer,&menuRect2);
                                SDL_RenderCopy(renderer,label2,NULL,&menuRect2);
                                SDL_RenderPresent(renderer);
                            }
                        }
                        if(menu != 3){
                            if(SDL_PointInRect(&mouse,&menuRect3)){
                                SDL_SetRenderDrawColor(renderer,100,100,100,255);
                                SDL_RenderFillRect(renderer,&menuRect3);
                                SDL_RenderCopy(renderer,label3,NULL,&menuRect3);
                                SDL_RenderPresent(renderer);
                            } else {
                                SDL_SetRenderDrawColor(renderer,34,56,67,255);
                                SDL_RenderFillRect(renderer,&menuRect3);
                                SDL_RenderCopy(renderer,label3,NULL,&menuRect3);
                                SDL_RenderPresent(renderer);
                            }
                        }
                        if(menu != 4){
                            if(SDL_PointInRect(&mouse,&menuRect4)){
                                SDL_SetRenderDrawColor(renderer,100,100,100,255);
                                SDL_RenderFillRect(renderer,&menuRect4);
                                SDL_RenderCopy(renderer,label4,NULL,&menuRect4);
                                SDL_RenderPresent(renderer);
                            } else {
                                SDL_SetRenderDrawColor(renderer,34,56,67,255);
                                SDL_RenderFillRect(renderer,&menuRect4);
                                SDL_RenderCopy(renderer,label4,NULL,&menuRect4);
                                SDL_RenderPresent(renderer);
                            }
                        }
                        if(menu != 5){
                            if(SDL_PointInRect(&mouse,&menuRect5)){
                                SDL_SetRenderDrawColor(renderer,100,100,100,255);
                                SDL_RenderFillRect(renderer,&menuRect5);
                                SDL_RenderCopy(renderer,label5,NULL,&menuRect5);
                                SDL_RenderPresent(renderer);
                            } else {
                                SDL_SetRenderDrawColor(renderer,34,56,67,255);
                                SDL_RenderFillRect(renderer,&menuRect5);
                                SDL_RenderCopy(renderer,label5,NULL,&menuRect5);
                                SDL_RenderPresent(renderer);
                            }
                        }
                    }

                    if(event.key.keysym.scancode == SDL_SCANCODE_F){
                        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }


                }
                if (menu == 2 && boucleIMC < 4){

                    texte = TTF_RenderText_Blended( fontIMC, data, CouleurTexteIMC );
                    inputIMC = SDL_CreateTextureFromSurface(renderer,texte);
                    SDL_QueryTexture(inputIMC, NULL, NULL, &w, &h);
                    textRectInputIMC.x=265;textRectInputIMC.y=50+50*boucleIMC;textRectInputIMC.w=w;textRectInputIMC.h=h;
                    SDL_RenderCopy(renderer, inputIMC, NULL, &textRectInputIMC);
                    SDL_RenderPresent(renderer);

                    if (returnKey == 1)
                        {
                        if (boucleIMC == 1)
                            {
                            //strcpy(strAge,data);
                            //vider data
                            strcpy(data,"");

                            textRectSaisieIMC.x=260;textRectSaisieIMC.y=148;textRectSaisieIMC.w=150;textRectSaisieIMC.h=29;
                            SDL_SetRenderDrawColor(renderer,255,255,0,120);
                            SDL_RenderDrawRect(renderer,&textRectSaisieIMC);
                            SDL_RenderPresent(renderer);
                            returnKey = 0;
                            }
                        if (boucleIMC == 2)
                            {
                            //strcpy(strPoids,data);
                            nPoids=atoi(data);
                            strcpy(data,"");
                            textRectSaisieIMC.x=260;textRectSaisieIMC.y=198;textRectSaisieIMC.w=150;textRectSaisieIMC.h=29;
                            SDL_SetRenderDrawColor(renderer,255,255,0,120);
                            SDL_RenderDrawRect(renderer,&textRectSaisieIMC);
                            SDL_RenderPresent(renderer);
                            returnKey = 0;
                            }
                        if (boucleIMC == 3)
                            {
                            //strcpy(strTaille,data);
                            nTaille=atoi(data);

                            nIMC=10000*nPoids/(nTaille*nTaille);
                            sprintf(data,"%.2f",nIMC);
                            texte = TTF_RenderText_Blended( fontIMC, data, CouleurTexteIMC );
                            strcpy(data,"");
                            outputIMC = SDL_CreateTextureFromSurface(renderer,texte);
                            SDL_QueryTexture(outputIMC, NULL, NULL, &w, &h);
                            textRectOutputIMC.x=230;textRectOutputIMC.y=300;textRectOutputIMC.w=w;textRectOutputIMC.h=h;
                            SDL_RenderCopy(renderer, outputIMC, NULL, &textRectOutputIMC);
                            SDL_RenderPresent(renderer);
                            SDL_StopTextInput();

                            }
                        boucleIMC=boucleIMC+1;
                        }
                }
            }

            SDL_DestroyWindow(window);

        } else {
            fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
    SDL_FreeSurface(texte);
    TTF_Quit();

/*
    MYSQL mysql;
    mysql_init(&mysql);
    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(&mysql,"localhost","root","root","projetc",0,NULL,0))
       {
            //Fermeture de MySQL
           mysql_close(&mysql);
       }
    else
    {
       printf("Une erreur s'est produite lors de la connexion à la BDD !\n");
    }

    printf("Rentrer votre poids : ");
    scanf("%f", &weight);
    printf("Rentrer votre taille : ");
    scanf("%f", &size);
    size = size / 100;
    imc = weight/(size*size);
    printf("Votre IMC : %.1f", imc);
*/
    return 0;
}
