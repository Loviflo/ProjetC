#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

MYSQL mysql;

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
int x;
int loopIMC;
char data[100];
int returnKey = 0;
float nSize,nWeight,nIMC,nAge;
int iLoop;
int movement = 1;
int theme = 1;
int inputProfileX = 220;
int inputProfileY = 100;
int initMysql = 1;
MYSQL mysql2;
TTF_Font *fontButton = NULL;
TTF_Font *fontText = NULL;
SDL_Color colorTextMenuSelected;
SDL_Color colorTextMenuUnselected;
SDL_Color colorTextLight;
SDL_Color colorTextDark;
//SDL_Color colorTextFooter = {200,50,78,255};
//SDL_Color colorFooter = {70,80,112,255};
SDL_Color colorMenu;
SDL_Color colorMenuClicked;
SDL_Color colorMenuHover = {128,128,128,255};
SDL_Color colorInput;
SDL_Color colorProfiles;
SDL_Color colorBackground;

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
SDL_Texture *imgICMTexture;
SDL_Renderer* renderer; // D�claration du renderer
SDL_Window *window;
SDL_Point mouse;
SDL_Rect rectangle[5]; // D�claration du tableau contenant les cases blanches
SDL_Rect textRectProfils[10];
SDL_Rect menuRect1;
SDL_Rect menuRect2;
SDL_Rect menuRect3;
SDL_Rect menuRect4;
SDL_Rect menuRect5;
SDL_Rect textRectLabelAff;
SDL_Rect textRectInputIMC[3];
SDL_Rect textRectInputProfil;
SDL_Rect textRectFooter;
SDL_Rect settingsRectYes;
SDL_Rect settingsRectNo;
SDL_Rect settingsRectTheme1;
SDL_Rect settingsRectTheme2;
SDL_Rect settingsRectTheme3;
SDL_Rect settingsRectTheme4;
SDL_Rect settingsRectTheme5;
SDL_Rect settingsRectTheme6;

void eraseText(SDL_Rect rectErase,SDL_Color rectColor){
    SDL_SetRenderDrawColor(renderer,rectColor.r,rectColor.g,rectColor.b,rectColor.a); // Background fontColor
    rectErase.x++;rectErase.y++;rectErase.w=rectErase.w-2;rectErase.h=rectErase.h-2;
    SDL_RenderFillRect(renderer,&rectErase);
    SDL_RenderPresent(renderer);
}

void displayTextButton(char *strText,SDL_Color fontColor,TTF_Font *font,SDL_Rect rect){
    SDL_Surface *sTexte;
    SDL_Texture *tTexte;
    int width,height;
    SDL_Rect textRectDestination;
    sTexte = TTF_RenderText_Blended( fontText, strText, fontColor );
    tTexte = SDL_CreateTextureFromSurface(renderer,sTexte);
    SDL_QueryTexture(tTexte, NULL, NULL, &width, &height);
    textRectDestination.x=rect.x+rect.w/2-width/2;
    textRectDestination.y=rect.y+rect.h/2-height/2;
    textRectDestination.w=width;
    textRectDestination.h=height;
    SDL_RenderCopy(renderer, tTexte, NULL, &textRectDestination);
    SDL_FreeSurface(sTexte);
    SDL_DestroyTexture(tTexte);

    //SDL_RenderPresent(renderer);
}

void displayText(char *strText,SDL_Color fontColor, int xPos,int yPos,int iSpeed) {

    int iLoop;
    int iStep;
    SDL_Surface *sTexte;
    SDL_Texture *tTexte;
    int width,height;
    SDL_Rect textRectOrigin,textRectDestination;
    sTexte = TTF_RenderText_Blended( fontText, strText, fontColor );
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
    SDL_FreeSurface(sTexte);
    SDL_DestroyTexture(tTexte);
}

void displayIMG(SDL_Surface surface,int xPos,int yPos) {

}

void redrawMenu(){
    SDL_SetRenderDrawColor(renderer,colorBackground.r,colorBackground.g,colorBackground.b,colorBackground.a); // fontColor
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,colorMenu.r,colorMenu.g,colorMenu.b,colorMenu.a);
    SDL_RenderFillRect(renderer,&menuRect1);
    SDL_RenderFillRect(renderer,&menuRect2);
    SDL_RenderFillRect(renderer,&menuRect3);
    SDL_RenderFillRect(renderer,&menuRect4);
    SDL_RenderFillRect(renderer,&menuRect5);
    displayTextButton("Connexion",colorTextMenuUnselected,fontButton,menuRect1);
    displayTextButton("IMC",colorTextMenuUnselected,fontButton,menuRect2);
    displayTextButton("Statistiques",colorTextMenuUnselected,fontButton,menuRect3);
    displayTextButton("Profil",colorTextMenuUnselected,fontButton,menuRect4);
    displayTextButton("Param�tres",colorTextMenuUnselected,fontButton,menuRect5);
    SDL_SetRenderDrawColor(renderer,colorMenu.r,colorMenu.g,colorMenu.b,colorMenu.a); // fontColor
    textRectFooter.x=0;textRectFooter.y=740;textRectFooter.w=800;textRectFooter.h=60;
    SDL_RenderFillRect(renderer,&textRectFooter);
    if (strcmp(strProfile,"") != 0){
        displayText("Vous �tes connect� en tant que ",colorTextDark,312,755,0*movement);
        displayText(strProfile,colorTextDark,600,755,0*movement);
    } else {
        displayText("Vous n'�tes pas connect�",colorTextDark,312,755,0*movement);
    }
    SDL_RenderPresent(renderer);
}

void mouseMotion(char *text, SDL_Rect rect, int option) {
    if (option == 0) {
        SDL_SetRenderDrawColor(renderer,colorMenuHover.r,colorMenuHover.g,colorMenuHover.b,colorMenuHover.a);
        SDL_RenderFillRect(renderer,&rect);
        displayTextButton(text,colorTextMenuUnselected,fontButton,rect);
        SDL_RenderPresent(renderer);
    } else if (option == 1) {
        SDL_SetRenderDrawColor(renderer,colorMenu.r,colorMenu.g,colorMenu.b,colorMenu.a);
        SDL_RenderFillRect(renderer,&rect);
        displayTextButton(text,colorTextMenuUnselected,fontButton,rect);
        SDL_RenderPresent(renderer);
    } else {
        SDL_SetRenderDrawColor(renderer,colorMenuClicked.r,colorMenuClicked.g,colorMenuClicked.b,colorMenuClicked.a);
        SDL_RenderFillRect(renderer,&rect);
        displayTextButton(text,colorTextMenuSelected,fontButton,rect);
        SDL_RenderPresent(renderer);
    }
}

void themes(int theme){
    if(theme == 1){
        colorMenu.r = 57; colorMenu.g = 62; colorMenu.b = 65; colorMenu.a = 255;
        colorMenuClicked.r = 217; colorMenuClicked.g = 208; colorMenuClicked.b = 222; colorMenuClicked.a = 255;
        colorInput.r = 255; colorInput.g = 147; colorInput.b = 79; colorInput.a = 255;
        colorProfiles.r = 188; colorProfiles.g = 141; colorProfiles.b = 160; colorProfiles.a = 255;
        colorBackground.r = 180; colorBackground.g = 194; colorBackground.b = 146; colorBackground.a = 255;
        colorTextMenuSelected.r = 0; colorTextMenuSelected.g = 0; colorTextMenuSelected.b = 0; colorTextMenuSelected.a = 255;
        colorTextMenuUnselected.r = 255; colorTextMenuUnselected.g = 255; colorTextMenuUnselected.b = 255; colorTextMenuUnselected.a = 255;
        colorTextLight.r = 0; colorTextLight.g = 0; colorTextLight.b = 0; colorTextLight.a = 255;
        colorTextDark.r = 255; colorTextDark.g = 255; colorTextDark.b = 255; colorTextDark.a = 255;
    }
    if(theme == 2){
        colorMenu.r = 58; colorMenu.g = 46; colorMenu.b = 57; colorMenu.a = 255;
        colorMenuClicked.r = 30; colorMenuClicked.g = 85; colorMenuClicked.b = 92; colorMenuClicked.a = 255;
        colorInput.r = 241; colorInput.g = 81; colorInput.b = 82; colorInput.a = 255;
        colorProfiles.r = 39; colorProfiles.g = 154; colorProfiles.b = 241; colorProfiles.a = 255;
        colorBackground.r = 201; colorBackground.g = 206; colorBackground.b = 189; colorBackground.a = 255;
        colorTextMenuSelected.r = 255; colorTextMenuSelected.g = 255; colorTextMenuSelected.b = 255; colorTextMenuSelected.a = 255;
        colorTextMenuUnselected.r = 255; colorTextMenuUnselected.g = 255; colorTextMenuUnselected.b = 255; colorTextMenuUnselected.a = 255;
        colorTextLight.r = 0; colorTextLight.g = 0; colorTextLight.b = 0; colorTextLight.a = 255;
        colorTextDark.r = 255; colorTextDark.g = 255; colorTextDark.b = 255; colorTextDark.a = 255;
    }
    if(theme == 3){
        colorMenu.r = 236; colorMenu.g = 229; colorMenu.b = 240; colorMenu.a = 255;
        colorMenuClicked.r = 45; colorMenuClicked.g = 30; colorMenuClicked.b = 47; colorMenuClicked.a = 255;
        colorInput.r = 247; colorInput.g = 179; colorInput.b = 43; colorInput.a = 255;
        colorProfiles.r = 132; colorProfiles.g = 108; colorProfiles.b = 91; colorProfiles.a = 255;
        colorBackground.r = 116; colorBackground.g = 119; colorBackground.b = 107; colorBackground.a = 255;
        colorTextMenuSelected.r = 255; colorTextMenuSelected.g = 255; colorTextMenuSelected.b = 255; colorTextMenuSelected.a = 255;
        colorTextMenuUnselected.r = 0; colorTextMenuUnselected.g = 0; colorTextMenuUnselected.b = 0; colorTextMenuUnselected.a = 255;
        colorTextLight.r = 255; colorTextLight.g = 255; colorTextLight.b = 255; colorTextLight.a = 255;
        colorTextDark.r = 0; colorTextDark.g = 0; colorTextDark.b = 0; colorTextDark.a = 255;
    }
    if(theme == 4){
        colorMenu.r = 17; colorMenu.g = 27; colorMenu.b = 54; colorMenu.a = 255;
        colorMenuClicked.r = 55; colorMenuClicked.g = 1; colorMenuClicked.b = 43; colorMenuClicked.a = 255;
        colorInput.r = 135; colorInput.g = 23; colorInput.b = 36; colorInput.a = 255;
        colorProfiles.r = 82; colorProfiles.g = 185; colorProfiles.b = 208; colorProfiles.a = 255;
        colorBackground.r = 250; colorBackground.g = 238; colorBackground.b = 0; colorBackground.a = 255;
        colorTextMenuSelected.r = 255; colorTextMenuSelected.g = 255; colorTextMenuSelected.b = 255; colorTextMenuSelected.a = 255;
        colorTextMenuUnselected.r = 255; colorTextMenuUnselected.g = 255; colorTextMenuUnselected.b = 255; colorTextMenuUnselected.a = 255;
        colorTextLight.r = 0; colorTextLight.g = 0; colorTextLight.b = 0; colorTextLight.a = 255;
        colorTextDark.r = 255; colorTextDark.g = 255; colorTextDark.b = 0; colorTextDark.a = 255;
    }
    if(theme == 5){
        colorMenu.r = 85; colorMenu.g = 67; colorMenu.b = 72; colorMenu.a = 255;
        colorMenuClicked.r = 110; colorMenuClicked.g = 68; colorMenuClicked.b = 255; colorMenuClicked.a = 255;
        colorInput.r = 255; colorInput.g = 90; colorInput.b = 95; colorInput.a = 255;
        colorProfiles.r = 184; colorProfiles.g = 146; colorProfiles.b = 255; colorProfiles.a = 255;
        colorBackground.r = 247; colorBackground.g = 178; colorBackground.b = 173; colorBackground.a = 255;
        colorTextMenuSelected.r = 255; colorTextMenuSelected.g = 255; colorTextMenuSelected.b = 255; colorTextMenuSelected.a = 255;
        colorTextMenuUnselected.r = 255; colorTextMenuUnselected.g = 255; colorTextMenuUnselected.b = 255; colorTextMenuUnselected.a = 255;
        colorTextLight.r = 0; colorTextLight.g = 0; colorTextLight.b = 0; colorTextLight.a = 255;
        colorTextDark.r = 255; colorTextDark.g = 255; colorTextDark.b = 255; colorTextDark.a = 255;
    }
    if(theme == 6){
        colorMenu.r = 102; colorMenu.g = 199; colorMenu.b = 244; colorMenu.a = 255;
        colorMenuClicked.r = 84; colorMenuClicked.g = 56; colorMenuClicked.b = 220; colorMenuClicked.a = 255;
        colorInput.r = 247; colorInput.g = 146; colorInput.b = 86; colorInput.a = 255;
        colorProfiles.r = 104; colorProfiles.g = 80; colorProfiles.b = 68; colorProfiles.a = 255;
        colorBackground.r = 225; colorBackground.g = 205; colorBackground.b = 181; colorBackground.a = 255;
        colorTextMenuSelected.r = 255; colorTextMenuSelected.g = 255; colorTextMenuSelected.b = 255; colorTextMenuSelected.a = 255;
        colorTextMenuUnselected.r = 0; colorTextMenuUnselected.g = 0; colorTextMenuUnselected.b = 0; colorTextMenuUnselected.a = 255;
        colorTextLight.r = 0; colorTextLight.g = 0; colorTextLight.b = 0; colorTextLight.a = 255;
        colorTextDark.r = 0; colorTextDark.g = 0; colorTextDark.b = 0; colorTextDark.a = 0;
    }
}

void formProfile(){ // Premier menu

    SDL_SetRenderDrawColor(renderer,colorMenuClicked.r,colorMenuClicked.g,colorMenuClicked.b,colorMenuClicked.a);
    SDL_RenderFillRect(renderer,&menuRect1);
    displayTextButton("Connexion",colorTextMenuSelected,fontButton,menuRect1);
    menu=1;

    displayText("Cr�ation du profil : ",colorTextLight,50,100,7*movement);
    SDL_SetRenderDrawColor(renderer,colorInput.r,colorInput.g,colorInput.b,colorInput.a);
    textRectInputProfil.x=inputProfileX;textRectInputProfil.y=inputProfileY - 2;textRectInputProfil.w=260;textRectInputProfil.h=29;
    SDL_RenderDrawRect(renderer,&textRectInputProfil);
    SDL_RenderPresent(renderer);
    strcpy(data,"");
    SDL_StartTextInput();
    returnKey = 0;
    displayText("S�lection du profil",colorTextLight,500,100,7*movement);
    for (iProfiles=0;iProfiles<nMaxProfiles;iProfiles++){
        textRectProfils[iProfiles].x=500;
        textRectProfils[iProfiles].y=145+iProfiles*50;
        textRectProfils[iProfiles].w=260;
        textRectProfils[iProfiles].h=35;
    }
    for (iProfiles=0;iProfiles<nProfiles;iProfiles++){
        SDL_SetRenderDrawColor(renderer,colorProfiles.r,colorProfiles.g,colorProfiles.b,colorProfiles.a);
        SDL_RenderFillRect(renderer,&textRectProfils[iProfiles]);
        displayText(profiles[iProfiles],colorTextLight,510,150+50*iProfiles,10*movement);
    }

}

void formIMC(){ // Deuxi�me menu



    SDL_SetRenderDrawColor(renderer,colorMenuClicked.r,colorMenuClicked.g,colorMenuClicked.b,colorMenuClicked.a);
    SDL_RenderFillRect(renderer,&menuRect2);
    displayTextButton("IMC",colorTextMenuSelected,fontButton,menuRect2);
    SDL_RenderPresent(renderer);
    menu = 2;

// Affichage premi�re question
    displayText("Entrez votre �ge :",colorTextLight,50,100,7*movement);

// Affichage deuxi�me question
    displayText("Entrez votre poids (en kg) :",colorTextLight,50,150,7*movement);

// Affichage troisi�me question
    displayText("Entrez votre taille (en cm) :",colorTextLight,50,200,7*movement);

    loopIMC = 1;
    returnKey = 0;

    SDL_SetRenderDrawColor(renderer,colorInput.r,colorInput.g,colorInput.b,colorInput.a);
    textRectInputIMC[0].x=360;textRectInputIMC[0].y=98;textRectInputIMC[0].w=50;textRectInputIMC[0].h=29;
    textRectInputIMC[1].x=360;textRectInputIMC[1].y=148;textRectInputIMC[1].w=50;textRectInputIMC[1].h=29;
    textRectInputIMC[2].x=360;textRectInputIMC[2].y=198;textRectInputIMC[2].w=50;textRectInputIMC[2].h=29;
    SDL_RenderDrawRect(renderer,&textRectInputIMC[0]);
    SDL_RenderPresent(renderer);
    //strcpy(data,"");
    SDL_StartTextInput();


}

void statistics(){ // Troisi�me menu

    int nLoopStats = 1;
    SDL_SetRenderDrawColor(renderer,colorMenuClicked.r,colorMenuClicked.g,colorMenuClicked.b,colorMenuClicked.a);
    SDL_RenderFillRect(renderer,&menuRect3);
    displayTextButton("Statistiques",colorTextMenuSelected,fontButton,menuRect3);
    SDL_RenderPresent(renderer);
    menu = 3;
/*
    //mysql_real_connect(&mysql,"localhost","root","root","projetc",3306,NULL,0);
    mysql_query(&mysql, "SELECT nom_utilisateur, poids, taille, IMC FROM corps LIMIT 12");
    //D�claration des objets
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    unsigned int num_champs = 0;

    //On met le jeu de r�sultat dans le pointeur result
    result = mysql_use_result(&mysql);
    num_champs = mysql_num_fields(result);
    //Tant qu'il y a encore un r�sultat ...
    while ((row = mysql_fetch_row(result))){
        for(int i = 0; i < num_champs; i++)
        {
            //On ecrit toutes les valeurs
        SDL_SetRenderDrawColor(renderer,colorProfiles.r,colorProfiles.g,colorProfiles.b,colorProfiles.a);
        SDL_RenderFillRect(renderer,&textRectProfils[nProfiles]);
        displayText(row[i],colorTextLight,50+80*i,150+50*(nLoopStats),7*movement);
        }
        nLoopStats++;
    }
    mysql_free_result(result);
*/
}

void profile(){ // Quatri�me menu

    SDL_SetRenderDrawColor(renderer,colorMenuClicked.r,colorMenuClicked.g,colorMenuClicked.b,colorMenuClicked.a);
    SDL_RenderFillRect(renderer,&menuRect4);
    displayTextButton("Profil",colorTextMenuSelected,fontButton,menuRect4);
    SDL_RenderPresent(renderer);
    menu = 4;

}

void settings(){ // Cinqui�me menu
    mouseMotion("Param�tres",menuRect5,2);
    menu = 5;

    displayText("Activer le d�filement ?", colorTextLight,50,100,7*movement);

    SDL_SetRenderDrawColor(renderer,colorMenu.r,colorMenu.g,colorMenu.b,colorMenu.a);
    settingsRectYes.x=100;settingsRectYes.y=150;settingsRectYes.w=200;settingsRectYes.h=50;
    settingsRectNo.x=320;settingsRectNo.y=150;settingsRectNo.w=200;settingsRectNo.h=50;
    SDL_RenderFillRect(renderer,&settingsRectYes);
    SDL_RenderFillRect(renderer,&settingsRectNo);
    displayTextButton("Oui",colorTextMenuUnselected,fontButton,settingsRectYes);
    displayTextButton("Non",colorTextMenuUnselected,fontButton,settingsRectNo);
    //SDL_RenderPresent(renderer);

    if (movement == 1) {
        mouseMotion("Oui",settingsRectYes,2);
    } else {
        mouseMotion("Non",settingsRectNo,2);
    }

    displayText("Th�mes", colorTextLight, 50,300,7*movement);

    SDL_SetRenderDrawColor(renderer,colorMenu.r,colorMenu.g,colorMenu.b,colorMenu.a);
    settingsRectTheme1.x=100;settingsRectTheme1.y=350;settingsRectTheme1.w=170;settingsRectTheme1.h=50;
    settingsRectTheme2.x=315;settingsRectTheme2.y=350;settingsRectTheme2.w=170;settingsRectTheme2.h=50;
    settingsRectTheme3.x=530;settingsRectTheme3.y=350;settingsRectTheme3.w=170;settingsRectTheme3.h=50;
    settingsRectTheme4.x=100;settingsRectTheme4.y=450;settingsRectTheme4.w=170;settingsRectTheme4.h=50;
    settingsRectTheme5.x=315;settingsRectTheme5.y=450;settingsRectTheme5.w=170;settingsRectTheme5.h=50;
    settingsRectTheme6.x=530;settingsRectTheme6.y=450;settingsRectTheme6.w=170;settingsRectTheme6.h=50;
    SDL_RenderFillRect(renderer,&settingsRectTheme1);
    SDL_RenderFillRect(renderer,&settingsRectTheme2);
    SDL_RenderFillRect(renderer,&settingsRectTheme3);
    SDL_RenderFillRect(renderer,&settingsRectTheme4);
    SDL_RenderFillRect(renderer,&settingsRectTheme5);
    SDL_RenderFillRect(renderer,&settingsRectTheme6);
    displayTextButton("Vert laurier",colorTextMenuUnselected,fontButton,settingsRectTheme1);
    displayTextButton("Gris cendr�",colorTextMenuUnselected,fontButton,settingsRectTheme2);
    displayTextButton("Nickel",colorTextMenuUnselected,fontButton,settingsRectTheme3);
    displayTextButton("Cyberpunk",colorTextMenuUnselected,fontButton,settingsRectTheme4);
    displayTextButton("Melon",colorTextMenuUnselected,fontButton,settingsRectTheme5);
    displayTextButton("Amande",colorTextMenuUnselected,fontButton,settingsRectTheme6);
    // SDL_RenderPresent(renderer);

    if (theme == 1) {
        mouseMotion("Vert Laurier",settingsRectTheme1,2);
    }
    if (theme == 2){
        mouseMotion("Gris cendr�",settingsRectTheme2,2);
    }
    if (theme == 3) {
        mouseMotion("Nickel",settingsRectTheme3,2);
    }
    if (theme == 4) {
        mouseMotion("Cyberpunk",settingsRectTheme4,2);
    }
    if (theme == 5) {
        mouseMotion("Melon",settingsRectTheme5,2);
    }
    if (theme == 6) {
        mouseMotion("Amande",settingsRectTheme6,2);
    }
}

int main(int argc, char** argv)
{
    mysql_init(&mysql);

    //my_bool reconnect = 0;
    //mysql_options(&mysql,MYSQL_OPT_RECONNECT,&reconnect);
    mysql_real_connect(&mysql,"localhost","root","root","projetc",3306,NULL,0);
    // R�cup�ration des utilisateurs
    mysql_query(&mysql, "SELECT * FROM profil_util");
    //D�claration des objets
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    //On met le jeu de r�sultat dans le pointeur result
    result = mysql_use_result(&mysql);
            //Tant qu'il y a encore un r�sultat ...
    nProfiles = 0;
    while ((row = mysql_fetch_row(result))) {
        unsigned long *lengths;
        lengths = mysql_fetch_lengths(result);
        //On ecrit toutes les valeurs
        strcpy(profiles[nProfiles],row[0]);
        nProfiles++;
    }
    mysql_free_result(result);

    TTF_Init();
    fontText = TTF_OpenFont("arial.ttf",20);
    fontButton = TTF_OpenFont("arial.ttf",20);
    SDL_Surface *logo = IMG_Load("Yuka.jpg");
    SDL_Surface *imgIMC = IMG_Load("unnamed.jpg");

    /* Initialisation simple */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK) < 0) // Initialisation de la SDL
    {
        printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
        return -1;
    }

{
// Cr�ation de la fen�tre :
    window = SDL_CreateWindow("Yuka" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 800 , 800 , 0);
    if(window == NULL) // Gestion des erreurs
    {
        printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
        return -1;
    }
    SDL_SetWindowIcon(window,logo);
    SDL_FreeSurface(logo);

// Cr�ation du renderer :
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Cr�ation du renderer
    if(renderer == NULL) // Gestion des erreurs
    {
        printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(renderer,colorBackground.r,colorBackground.g,colorBackground.b,colorBackground.a); // fontColor
    SDL_RenderClear(renderer);
    //SDL_SetRenderDrawColor(renderer,34,56,67,255);

    menuRect1.x = 0; menuRect1.y = 0; menuRect1.w = 160; menuRect1.h = 50;

    menuRect2.x = 160; menuRect2.y = 0; menuRect2.w = 160; menuRect2.h = 50;

    menuRect3.x = 320; menuRect3.y = 0; menuRect3.w = 160; menuRect3.h = 50;

    menuRect4.x = 480; menuRect4.y = 0; menuRect4.w = 160; menuRect4.h = 50;

    menuRect5.x = 640; menuRect5.y = 0; menuRect5.w = 160; menuRect5.h = 50;

    themes(theme);
    redrawMenu();

    formProfile();
    if (window)
    {
        char cont = 1; /* D�termine si on continue la boucle principale */
        SDL_Event event;
        while ( cont != 0 ) {
            while ( SDL_PollEvent(&event) ) {
                if(event.type == SDL_KEYDOWN) {
                    if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) {
                        cont = 0;
                    }
                    if ( event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && menu == 1) {
                        if (strlen(data)>0) {
                            data[strlen(data)-1]='\0';
                            eraseText(textRectInputProfil,colorBackground);
                            displayText(data,colorTextLight,inputProfileX + 10,inputProfileY,0*movement);
                        }
                    }
                    if ( event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && menu == 2) {
                        if (strlen(data)>0){
                            data[strlen(data)-1]='\0';
                            eraseText(textRectInputIMC[loopIMC-1],colorBackground);
                            displayText(data,colorTextLight,365,50+50*loopIMC,0*movement);
                        }
                    }

                    if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN) {
                        if (menu == 2 ){
                            if(strlen(data) > 0 && atoi(data) !=0) {
                                returnKey = 1;
                            }
                        }
                        if (menu ==1 && strlen(data) > 0) {
                            returnKey = 1;
                        }
                    }
                }

                if(event.type == SDL_TEXTINPUT && menu == 2) {
                    if (strchr("0123456789.",event.text.text[0])!=NULL && (strlen(data) < 3)){
                        strcat(data, event.text.text);
                        displayText(data,colorTextLight,365,50+50*loopIMC,0*movement);
                    }
                }
                if(event.type == SDL_TEXTINPUT && menu == 1 && nProfiles < nMaxProfiles) {
                    if (strchr("AZERTYUIOPQSDFGHJKLMWXCVBNabcdefghijklmanopqrstuvwxyz-",event.text.text[0])!=NULL && (strlen(data) < 15)){
                        strcat(data, event.text.text);
                        eraseText(textRectInputProfil,colorBackground);
                        displayText(data,colorTextLight,inputProfileX + 10,inputProfileY,0*movement);
                    }

                }

                if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                    mouse.x = event.button.x;
                    mouse.y = event.button.y;
                    if(SDL_PointInRect(&mouse, &menuRect1)) {
                        redrawMenu();
                        formProfile();
                    }
                    if(SDL_PointInRect(&mouse, &menuRect2) && strcmp(strProfile,"") != 0) {
                        redrawMenu();
                        formIMC();
                    }
                    if(SDL_PointInRect(&mouse, &menuRect3) && strcmp(strProfile,"") != 0) {
                        redrawMenu();
                        statistics();
                    }
                    if(SDL_PointInRect(&mouse, &menuRect4) && strcmp(strProfile,"") != 0) {
                        redrawMenu();
                        profile();
                    }
                    if(SDL_PointInRect(&mouse, &menuRect5)){
                        redrawMenu();
                        settings();
                    }
                    if(menu == 5){
                        if(SDL_PointInRect(&mouse, &settingsRectYes)){
                            movement = 1;
                            redrawMenu();
                            settings();
                        }
                        if (SDL_PointInRect(&mouse, &settingsRectNo)){
                            movement = 0;
                            redrawMenu();
                            settings();
                        }
                        if (SDL_PointInRect(&mouse, &settingsRectTheme1)){
                            theme = 1;
                            themes(theme);
                            redrawMenu();
                            settings();
                        }
                        if (SDL_PointInRect(&mouse, &settingsRectTheme2)){
                            theme = 2;
                            themes(theme);
                            redrawMenu();
                            settings();
                        }
                        if (SDL_PointInRect(&mouse, &settingsRectTheme3)){
                            theme = 3;
                            themes(theme);
                            redrawMenu();
                            settings();
                        }
                        if (SDL_PointInRect(&mouse, &settingsRectTheme4)){
                            theme = 4;
                            themes(theme);
                            redrawMenu();
                            settings();
                        }
                        if (SDL_PointInRect(&mouse, &settingsRectTheme5)){
                            theme = 5;
                            themes(theme);
                            redrawMenu();
                            settings();
                        }
                        if (SDL_PointInRect(&mouse, &settingsRectTheme6)){
                            theme = 6;
                            themes(theme);
                            redrawMenu();
                            settings();
                        }
                    }

                    for (iProfiles=0;iProfiles<nProfiles;iProfiles++){
                        if (SDL_PointInRect(&mouse, &textRectProfils[iProfiles]) && menu == 1){
                            strcpy(strProfile,profiles[iProfiles]);
                            SDL_SetRenderDrawColor(renderer,colorMenu.r,colorMenu.g,colorMenu.b,colorMenu.a);
                            SDL_RenderFillRect(renderer,&textRectFooter);
                            displayText("Vous �tes connect� en tant que ",colorTextDark,312,755,0*movement);
                            displayText(profiles[iProfiles],colorTextDark,600,755,0*movement);
                        }
                    }

                }

                if(event.type == SDL_MOUSEMOTION){
                    mouse.x = event.motion.x;
                    mouse.y = event.motion.y;
                    if(menu != 1){
                        if(SDL_PointInRect(&mouse,&menuRect1)){
                            mouseMotion("Connexion",menuRect1,0);
                        } else {
                            mouseMotion("Connexion",menuRect1,1);
                        }
                    }
                    if(menu != 2){
                        if(SDL_PointInRect(&mouse,&menuRect2)){
                            mouseMotion("IMC",menuRect2,0);
                        } else {
                            mouseMotion("IMC",menuRect2,1);
                        }
                    }
                    if(menu != 3){
                        if(SDL_PointInRect(&mouse,&menuRect3)){
                            mouseMotion("Statistiques",menuRect3,0);
                        } else {
                            mouseMotion("Statistiques",menuRect3,1);
                        }
                    }
                    if(menu != 4){
                        if(SDL_PointInRect(&mouse,&menuRect4)){
                            mouseMotion("Profil",menuRect4,0);
                        } else {
                            mouseMotion("Profil",menuRect4,1);
                        }
                    }
                    if(menu != 5){
                        if(SDL_PointInRect(&mouse,&menuRect5)){
                            mouseMotion("Param�tres",menuRect5,0);
                        } else {
                            mouseMotion("Param�tres",menuRect5,1);
                        }
                    }
                    if (menu == 5) {
                        if(movement != 1) {
                            if(SDL_PointInRect(&mouse,&settingsRectYes)){
                                mouseMotion("Oui",settingsRectYes,0);
                            } else {
                                mouseMotion("Oui",settingsRectYes,1);
                            }
                        }
                        if (movement != 0) {
                            if(SDL_PointInRect(&mouse,&settingsRectNo)){
                                mouseMotion("Non",settingsRectNo,0);
                            } else {
                                mouseMotion("Non",settingsRectNo,1);
                            }
                        }
                        if (theme != 1) {
                            if(SDL_PointInRect(&mouse,&settingsRectTheme1)){
                                mouseMotion("Vert laurier",settingsRectTheme1,0);
                            } else {
                                mouseMotion("Vert laurier",settingsRectTheme1,1);
                            }
                        }
                        if (theme != 2) {
                            if(SDL_PointInRect(&mouse,&settingsRectTheme2)){
                                mouseMotion("Gris cendr�",settingsRectTheme2,0);
                            } else {
                                mouseMotion("Gris cendr�",settingsRectTheme2,1);
                            }
                        }
                        if (theme != 3) {
                            if(SDL_PointInRect(&mouse,&settingsRectTheme3)){
                                mouseMotion("Nickel",settingsRectTheme3,0);
                            } else {
                                mouseMotion("Nickel",settingsRectTheme3,1);
                            }
                        }
                        if (theme != 4) {
                            if(SDL_PointInRect(&mouse,&settingsRectTheme4)){
                                mouseMotion("Cyberpunk",settingsRectTheme4,0);
                            } else {
                                mouseMotion("Cyberpunk",settingsRectTheme4,1);
                            }
                        }
                        if (theme != 5) {
                            if(SDL_PointInRect(&mouse,&settingsRectTheme5)){
                                mouseMotion("Melon",settingsRectTheme5,0);
                            } else {
                                mouseMotion("Melon",settingsRectTheme5,1);
                            }
                        }
                        if (theme != 6) {
                            if(SDL_PointInRect(&mouse,&settingsRectTheme6)){
                                mouseMotion("Amande",settingsRectTheme6,0);
                            } else {
                                mouseMotion("Amande",settingsRectTheme6,1);
                            }
                        }
                    }
                }

            }
            if (menu == 1){

                if (returnKey == 1 && nProfiles < nMaxProfiles){
                    strcpy(profiles[nProfiles],data);
                    strcpy(data,"");
                    returnKey = 0;
                    SDL_SetRenderDrawColor(renderer,colorProfiles.r,colorProfiles.g,colorProfiles.b,colorProfiles.a);
                    SDL_RenderFillRect(renderer,&textRectProfils[nProfiles]);
                    displayText(profiles[nProfiles],colorTextLight,510,150+50*nProfiles,5*movement);
                    char requete[150] = "";
                    sprintf(requete, "INSERT INTO profil_util VALUES ('%s')",profiles[nProfiles]);
                    if(mysql_query(&mysql, requete) != 0){
                        printf(mysql_error(&mysql));
                    }
                    strcpy(requete,"");
                    //suppression caract�res
                    eraseText(textRectInputProfil,colorBackground);
                    nProfiles++;
                }
            }

            if (menu == 2 && loopIMC < 4){


                if (returnKey == 1)
                    {
                    if (loopIMC == 1)
                        {
                        //strcpy(strAge,data);
                        //vider data
                        nAge = atoi(data);
                        strcpy(data,"");
                        SDL_SetRenderDrawColor(renderer,colorInput.r,colorInput.g,colorInput.b,colorInput.a);
                        SDL_RenderDrawRect(renderer,&textRectInputIMC[loopIMC]);
                        SDL_RenderPresent(renderer);
                        returnKey = 0;
                        }
                    if (loopIMC == 2)
                        {
                        //strcpy(strPoids,data);
                        nWeight = atoi(data);
                        strcpy(data,"");
                        SDL_SetRenderDrawColor(renderer,colorInput.r,colorInput.g,colorInput.b,colorInput.a);
                        SDL_RenderDrawRect(renderer,&textRectInputIMC[loopIMC]);
                        SDL_RenderPresent(renderer);
                        returnKey = 0;
                        }
                    if (loopIMC == 3)
                        {
                        //strcpy(strTaille,data);
                        nSize = atoi(data);

                        nIMC=10000*nWeight/(nSize*nSize);
                        sprintf(data,"%.2f",nIMC);
                        displayText("Votre IMC est de :",colorTextLight,50,300,3*movement);
                        displayText(data,colorTextLight,250,300,1*movement);
                        if (nIMC <= 18.5) {
                            displayText("Vous �tes en insuffisance pond�rale !!",colorTextLight,60,350,3*movement);
                            imgICMTexture = SDL_CreateTextureFromSurface(renderer,imgIMC);
                            SDL_FreeSurface(imgIMC);
                            //SDL_QueryTexture(imgICMTexture, NULL, NULL, &width, &height);
                            SDL_RenderPresent(renderer);
                        } else if (nIMC > 18.5 && nIMC <= 25 ) {
                            displayText("Vous avez une corpulence normale",colorTextLight,60,350,3*movement);
                        } else if (nIMC > 25  && nIMC <= 30 ) {
                            displayText("Vous �tes en surpoid, faites attention",colorTextLight,60,350,3*movement);
                        } else if (nIMC > 30  && nIMC <= 35 ) {
                            displayText("Vous �tes en �tat d'ob�sit� mod�r�e, faites attention",colorTextLight,60,350,3*movement);
                        } else if (nIMC > 35 && nIMC <= 40) {
                            displayText("Vous �tes en �tat d'ob�sit� s�v�re, il y a urgence !!!",colorTextLight,60,350,3*movement);
                        } else {
                            displayText("Vous �tes en �tat d'ob�sit� morbide, il faut vraiment faire quelque chose !!!",colorTextLight,60,350,3*movement);
                        }

                        char requete3[150] = "";
                        sprintf(requete3, "INSERT INTO corps VALUES('%s',%f,%f,%.2f,%.1f)",strProfile,nWeight,nSize,nIMC,nAge);
                        printf(requete3);

                        //mysql_init(&mysql2);

                        mysql_real_connect(&mysql,"localhost","root","root","projetc",3306,NULL,0);

                           if(mysql_query(&mysql, requete3) != 0){
                            printf(mysql_error(&mysql));
                            }
                        strcpy(requete3,"");


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

mysql_close(&mysql);
SDL_DestroyRenderer(renderer);
SDL_Quit();
TTF_CloseFont(fontButton); /* Doit �tre avant TTF_Quit() */
SDL_FreeSurface(texte);
TTF_Quit();
    return 0;
}
