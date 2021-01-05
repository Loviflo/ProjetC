#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
//#include <MYSQL/mysql.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

int nMaxProfiles=12;
char profiles[25][15]; //Tableau de profiles
int nProfiles=0; //Nombre de profiles
int iProfiles;
char strProfile[15]=""; //Nom du profil s�lectionn�
float weight;
float size;
float imc;
int menu = 0;
int w,h;
int loopIMC;
char data[100];
int returnKey = 0;
float nSize,nWeight,nIMC;
int iLoop ;
TTF_Font *font = NULL;
TTF_Font *fontIMC = NULL;
SDL_Color colorText = {200,160,100,255};
SDL_Color colorTextIMC = {0,0,0,255};
SDL_Color colorTextFooter = {200,50,78,255};

SDL_Color colorMenu = {34,56,67,255};
SDL_Color colorMenuClicked = {255,255,255,255};
SDL_Color colorMenuHover = {100,100,100,255};
SDL_Color colorInput = {255,255,0,255};
SDL_Color colorProfiles = {255,120,0,255};
SDL_Color colorBackground = {233,139,102,255};
SDL_Surface *texte = NULL;
SDL_Texture *label1;
SDL_Texture *label2;
SDL_Texture *label3;
SDL_Texture *label4;
SDL_Texture *label5;
SDL_Texture *labelIMC1;
SDL_Texture *labelIMC2;
SDL_Texture *labelIMC3;
SDL_Texture *labelIMC4;
SDL_Renderer* renderer; // D�claration du renderer
SDL_Window* window;
SDL_Point mouse;
SDL_Rect rectangle[5]; // D�claration du tableau contenant les cases blanches
SDL_Rect textRectProfils[10];
SDL_Rect menuRect1;
SDL_Rect menuRect2;
SDL_Rect menuRect3;
SDL_Rect menuRect4;
SDL_Rect menuRect5;
SDL_Rect textRectLabelAff;
SDL_Rect textRectInputIMC;
SDL_Rect textRectInputProfil;
SDL_Rect textRectFooter;



void AfficheTexte(char *strTexte,SDL_Color Couleur, int xPos,int yPos,int iSpeed) {

    int iLoop;
    int iStep;
    SDL_Surface *sTexte;
    SDL_Texture *tTexte;
    int width,height;
    SDL_Rect textRectOrigin,textRectDestination;
    sTexte = TTF_RenderText_Blended( fontIMC, strTexte, Couleur );
    tTexte = SDL_CreateTextureFromSurface(renderer,sTexte);
    SDL_QueryTexture(tTexte, NULL, NULL, &width, &height);
    textRectDestination.x=xPos;textRectDestination.y=yPos;textRectDestination.w=width;textRectDestination.h=height;
    //Rectangle de la texture � copier
    textRectOrigin.x=0;textRectOrigin.y=0;
    if (iSpeed == 0) {
        iStep=width;
    }
    else{
        iStep=iSpeed;
    }
    for (iLoop=0;iLoop < width;iLoop=iLoop+iStep)
    {
        if (iLoop+iStep > width){
            iStep=width-iLoop;
        }
        textRectDestination.x=xPos+iLoop;
        textRectDestination.w=iStep;
        textRectOrigin.w=iStep;textRectOrigin.h=height;textRectOrigin.x=iLoop;
        SDL_RenderCopy(renderer, tTexte, &textRectOrigin, &textRectDestination);
        SDL_RenderPresent(renderer);
    }
}

void formProfile(){

    SDL_SetRenderDrawColor(renderer,colorMenuClicked.r,colorMenuClicked.g,colorMenuClicked.b,colorMenuClicked.a);
    SDL_RenderFillRect(renderer,&menuRect1);
    SDL_RenderCopy(renderer,label1,NULL,&menuRect1);
    menu=1;

    AfficheTexte("Cr�ation profil : ",colorTextIMC,320,150,6);
    SDL_SetRenderDrawColor(renderer,colorInput.r,colorInput.g,colorInput.b,colorInput.a);
    textRectInputProfil.x=490;textRectInputProfil.y=150;textRectInputProfil.w=250;textRectInputProfil.h=29;
    SDL_RenderDrawRect(renderer,&textRectInputProfil);
    SDL_RenderPresent(renderer);
    strcpy(data,"");
    SDL_StartTextInput();
    returnKey = 0;
    for (iProfiles=0;iProfiles<nMaxProfiles;iProfiles++){
        textRectProfils[iProfiles].x=40;
        textRectProfils[iProfiles].y=98+iProfiles*50;
        textRectProfils[iProfiles].w=250;
        textRectProfils[iProfiles].h=35;
    }
    for (iProfiles=0;iProfiles<nProfiles;iProfiles++){
        SDL_SetRenderDrawColor(renderer,colorProfiles.r,colorProfiles.g,colorProfiles.b,colorProfiles.a);
        SDL_RenderFillRect(renderer,&textRectProfils[iProfiles]);
        AfficheTexte(profiles[iProfiles],colorTextIMC,50,100+50*iProfiles,10);
    }
}

void formIMC(){

//Rectangle de la texture � copier
    textRectLabelAff.x=0;textRectLabelAff.y=0;

// Affichage premi�re question
AfficheTexte("Entrez votre �ge :",colorTextIMC,50,100,7);

// Affichage deuxi�me question
AfficheTexte("Entrez votre poids (en kg) :",colorTextIMC,50,150,7);

// Affichage troisi�me question
AfficheTexte("Entrez votre taille (en cm) :",colorTextIMC,50,200,7);

// Affichage r�ponse



    loopIMC = 1;
    returnKey = 0;

    SDL_SetRenderDrawColor(renderer,255,255,0,120);
    textRectInputIMC.x=360;textRectInputIMC.y=98;textRectInputIMC.w=50;textRectInputIMC.h=29;
    SDL_RenderDrawRect(renderer,&textRectInputIMC);
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
        textRectFooter.x=0;textRectFooter.y=740;textRectFooter.w=790;textRectFooter.h=60;
        SDL_RenderFillRect(renderer,&textRectFooter);
        if (strcmp(strProfile,"") != 0){
            AfficheTexte("Vous �tes connect� en tant que ",colorTextFooter,260,750,0);
            AfficheTexte(strProfile,colorTextFooter,600,750,0);
        }
        SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{

    TTF_Init();
    fontIMC = TTF_OpenFont("arial.ttf",24);
    /* Initialisation simple */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK) < 0) // Initialisation de la SDL
    {
        printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
        return -1;
    }

{
    // Cr�ation de la fen�tre :
        window = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 790 , 800 , 0);
        if(window == NULL) // Gestion des erreurs
        {
            printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
            return -1;
        }

    // Cr�ation du renderer :
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Cr�ation du renderer
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

        font = TTF_OpenFont("arial.ttf",100);

        texte = TTF_RenderText_Blended( font, "Menu", colorText );
        label1 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( font, "IMC", colorText );
        label2 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( font, "Menu 3", colorText );
        label3 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( font, "Menu 4", colorText );
        label4 = SDL_CreateTextureFromSurface(renderer, texte);

        texte = TTF_RenderText_Blended( font, "Menu 5", colorText );
        label5 = SDL_CreateTextureFromSurface(renderer, texte);


        redrawMenu();

        formProfile();
        if (window)
        {
            char cont = 1; /* D�termine si on continue la boucle principale */
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
                        if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
                            {
                                if (menu == 2 ){
                                    if(strlen(data) > 0 && atoi(data) !=0) {
                                        returnKey = 1;
                                        } else {
                                            strcpy(data,"");
                                            redrawMenu();
                                            formIMC();
                                        }
                                }
                                if (menu ==1 && strlen(data) > 0){
                                    returnKey = 1;
                                }
                            }
                    }

                    if(event.type == SDL_TEXTINPUT && menu == 2)
                    {
                        if (strchr("0123456789.",event.text.text[0])!=NULL && (strlen(data) < 3)){
                           strcat(data, event.text.text);
                        }

                    }
                    if(event.type == SDL_TEXTINPUT && menu == 1 && nProfiles < nMaxProfiles)
                    {
                        if (strchr("AZERTYUIOPQSDFGHJKLMWXCVBNabcdefghijklmanopqrstuvwxyz-",event.text.text[0])!=NULL && (strlen(data) < 15)){
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
                            formProfile();
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
                        for (iProfiles=0;iProfiles<nProfiles;iProfiles++){
                            if (SDL_PointInRect(&mouse, &textRectProfils[iProfiles])){
                                strcpy(strProfile,profiles[iProfiles]);
                                SDL_SetRenderDrawColor(renderer,34,56,67,255);
                                SDL_RenderFillRect(renderer,&textRectFooter);
                                AfficheTexte("Vous �tes connect� en tant que ",colorTextFooter,260,750,0);
                                AfficheTexte(profiles[iProfiles],colorTextFooter,600,750,0);
                            }
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

                }
                if (menu == 1){
                    AfficheTexte(data,colorTextIMC,495,150,0);
                    if (returnKey == 1 && nProfiles < nMaxProfiles){
                        strcpy(profiles[nProfiles],data);
                        strcpy(data,"");
                        returnKey = 0;
                        SDL_SetRenderDrawColor(renderer,255,120,0,255);
                        SDL_RenderFillRect(renderer,&textRectProfils[nProfiles]);
                        AfficheTexte(profiles[nProfiles],colorTextIMC,50,100+50*nProfiles,5);
                        nProfiles++;
                        SDL_SetRenderDrawColor(renderer,233,139,102,255); // Couleur menu
                        SDL_RenderFillRect(renderer,&textRectInputProfil);
                        SDL_SetRenderDrawColor(renderer,255,255,0,120); //Couleur encadr� saisie
                        SDL_RenderDrawRect(renderer,&textRectInputProfil);
                        SDL_RenderPresent(renderer);

                    //redrawMenu();
                    //formProfile();
                    }

                }

                if (menu == 2 && loopIMC < 4){

                    AfficheTexte(data,colorTextIMC,365,50+50*loopIMC,0);

                    if (returnKey == 1)
                        {
                        if (loopIMC == 1)
                            {
                            //strcpy(strAge,data);
                            //vider data
                            strcpy(data,"");

                            textRectInputIMC.x=360;textRectInputIMC.y=148;textRectInputIMC.w=50;textRectInputIMC.h=29;
                            SDL_SetRenderDrawColor(renderer,255,255,0,120);
                            SDL_RenderDrawRect(renderer,&textRectInputIMC);
                            SDL_RenderPresent(renderer);
                            returnKey = 0;
                            }
                        if (loopIMC == 2)
                            {
                            //strcpy(strPoids,data);
                            nWeight=atoi(data);
                            strcpy(data,"");
                            textRectInputIMC.x=360;textRectInputIMC.y=198;textRectInputIMC.w=50;textRectInputIMC.h=29;
                            SDL_SetRenderDrawColor(renderer,255,255,0,120);
                            SDL_RenderDrawRect(renderer,&textRectInputIMC);
                            SDL_RenderPresent(renderer);
                            returnKey = 0;
                            }
                        if (loopIMC == 3)
                            {
                            //strcpy(strTaille,data);
                            nSize=atoi(data);

                            nIMC=10000*nWeight/(nSize*nSize);
                            sprintf(data,"%.2f",nIMC);
                            AfficheTexte("Votre IMC est de :",colorTextIMC,50,300,1);
                            AfficheTexte(data,colorTextIMC,250,300,1);
                            strcpy(data,"");
                            SDL_StopTextInput();
                            }
                        loopIMC=loopIMC+1;
                        }
                }
            }

            SDL_DestroyWindow(window);

        } else {
            fprintf(stderr,"Erreur de cr�ation de la fen�tre: %s\n",SDL_GetError());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_CloseFont(font); /* Doit �tre avant TTF_Quit() */
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
       printf("Une erreur s'est produite lors de la connexion � la BDD !\n");
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
