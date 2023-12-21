
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip

#include <SPI.h> // sûrement pas utilisée à voir

#define PIN_BP1 0x10

int g_int_quantity = 1;

// fonction pour encodeur
const byte pinEncodeurA = 2;
const byte pinEncodeurB = 3;

bool compte = 0;

const unsigned long dureeAntiRebond = 10;

void encodeurTickA()
{
  static unsigned long dateDernierChangement = 0;

  unsigned long date = millis();
  if ((date - dateDernierChangement) > dureeAntiRebond)
  {
    if (digitalRead(pinEncodeurB) == LOW)
    {
      compte = true;
    }
    else
    {
      compte = false;
    }
    dateDernierChangement = date;
  }
}
// fin fonction pour encodeur

// Etat du bouton cablé sur PORT Dx = PDx
bool g_bool_BP = !((PIND & PIN_BP1) == PIN_BP1);

TFT_eSPI tft = TFT_eSPI(); // instanciation pour pouvoir utiliser les méthodes de TFT_eSPI

typedef enum
{
  INIT,
  CHOICE,
  CLCLASSIC,
  EDITMODE,
  QUANTITYSELECT,
  LIQUIDCLADD,
  LIQUIDCLADDPLUS,
  LIQUIDREMOVE,
  LIQUIDREMOVEMINUS,
  TWOPUMPSMODE,
  COCACOLACUP,
  SERVICE,
  ARRETURGENCE,
  FINISHED,
  WAITNEWCUP
} TEtat; // TOUS LES DIFFERENTS ETAT (YEN AURA FORCEMENT PLUS POUR FAIRE LA SELECTION DU MODE CHOISI PAR EXEMPLE)
// Variables : état courant et état précédent
TEtat g_etat_courant, g_etat_old;
// Initialisation du graphe
void setup()
{
  DDRD = 00000000;

  // Etat initial
  g_etat_courant = INIT;
  g_etat_old = g_etat_courant;
  tft.init();
  tft.setRotation(0);
  

  // CODE POUR LECODEUR
  Serial.begin(9600);

  pinMode(pinEncodeurA, INPUT);
  pinMode(pinEncodeurB, INPUT);

  attachInterrupt(0, encodeurTickA, FALLING);
  // FIN CODE POUR LENCODEUR
  //  Draw clock face
  //  tft.fillCircle(64, 64, 61, TFT_BLUE);
  //  tft.fillCircle(64, 64, 57, TFT_BLACK);
  //  tft.fillCircle(65, 65, 3, TFT_RED);
}

void loop()
{
  switch (g_etat_courant)
  {

  case INIT:
    if (g_bool_BP)
    {
      g_etat_courant = CHOICE; // si on pose un verre (donc verre détecté) et qu'on appuie sur le bouton valider alors on passe au choix
    }
    break;

  case CHOICE:

    g_etat_courant = CLCLASSIC;

    break;

  case CLCLASSIC:
    if (compte == true)
    { // si le choix est fait et qu'on appuie sur le bouton valider on passe à létape suivante (graphe à améliorer pour faire fonctionner cette partie)
      g_etat_courant = EDITMODE;
    }
    /*
    else if (compte == false)
    {
      g_etat_courant = COCACOLACUP;
    }
    */
    break;

  case EDITMODE:
  /*
    if (compte == true)
    { // si le choix est fait et qu'on appuie sur le bouton valider on passe à létape suivante (graphe à améliorer pour faire fonctionner cette partie)
      g_etat_courant = TWOPUMPSMODE;
    }
    else 
    */
    if (compte == false)
    {
      g_etat_courant = CLCLASSIC;
    }
    else if (g_bool_BP)
    {
      g_etat_courant = QUANTITYSELECT;
    }
    break;

  case QUANTITYSELECT:
    if (compte == true)
    { // si le choix est fait et qu'on appuie sur le bouton valider on passe à létape suivante (graphe à améliorer pour faire fonctionner cette partie)
      g_etat_courant = LIQUIDCLADD;
      
    }
    else if (compte == false)
    {
      g_etat_courant = LIQUIDREMOVE;
    }
  case LIQUIDCLADD:
    if (compte == true)
    { // si le choix est fait et qu'on appuie sur le bouton valider on passe à létape suivante (graphe à améliorer pour faire fonctionner cette partie)
      g_etat_courant = LIQUIDCLADDPLUS;
    }
    else if (compte == false)
    {
      g_etat_courant = LIQUIDREMOVE;
    }

  case LIQUIDREMOVE:
    if (compte == true)
    { // si le choix est fait et qu'on appuie sur le bouton valider on passe à létape suivante (graphe à améliorer pour faire fonctionner cette partie)
      g_etat_courant = LIQUIDCLADD;
    }
    else if (compte == false)
    {
      g_etat_courant = LIQUIDREMOVEMINUS;
    }
   case LIQUIDCLADDPLUS:
    if (compte == true)
    { // si le choix est fait et qu'on appuie sur le bouton valider on passe à létape suivante (graphe à améliorer pour faire fonctionner cette partie)
      g_etat_courant = LIQUIDCLADD;
    }
    else if (compte == false)
    {
      g_etat_courant = LIQUIDREMOVE;
    }

  case LIQUIDREMOVEMINUS:
    if (compte == true)
    { // si le choix est fait et qu'on appuie sur le bouton valider on passe à létape suivante (graphe à améliorer pour faire fonctionner cette partie)
      g_etat_courant = LIQUIDCLADD;
    }
    else if (compte == false)
    {
      g_etat_courant = LIQUIDREMOVE;
    }
    /*
   case SERVICE:
     if ()
     { // si le bouton utilisé précedemment pour le choix est pressé alors on passe à l'arrêt d'urgence
       g_etat_courant = ARRETURGENCE;
     }
     else if ()
     { // si le poids du verre est atteint alors on peut passer  à l'état fini
       g_etat_courant = FINISHED;
     }
     break;

   case FINISHED:
     if ()
     { // si on enlève le verre on peut passer a l'état ou un verre est remis
       g_etat_courant = WAITNEWCUP;
     }
     break;

   case WAITNEWCUP:
     if ()
     { // si on mets un verre(vide) avant le délai imparti de mise en veille(10min peut être) alors on peut passer directement au choix à nouveau
       g_etat_courant = CHOICE;
     }
     else if ()
     {
       g_etat_courant = INIT; // si on dépasse le délai alors on passe en mode veille ou l'utilisateur devra appuyer sur le bouton et poser un verre pour faire un choix
     }
     break;
     */
  } // fin du selon des états

  if (g_etat_courant != g_etat_old)
  {                              // si on vient de changer d’état,
    g_etat_old = g_etat_courant; // actualiser l’état
    switch (g_etat_courant)
    {

    case INIT:
      tft.fillCircle(64, 64, 57, TFT_RED); // expérimental
      break;

    case CHOICE:
    tft.setTextColor(TFT_WHITE);
      tft.fillCircle(64, 64, 57, TFT_PINK);

      break;

    case CLCLASSIC:
    tft.setTextColor(TFT_WHITE);
      tft.drawString("CLASSIQUE", 64, 64, 2);

      break;
    case EDITMODE:
    tft.setTextColor(TFT_WHITE);
      tft.drawString("EDIT 1-370ml", 64, 64, 2);

      break;
    case QUANTITYSELECT:
    tft.setTextColor(TFT_WHITE);
      tft.drawString("select now between 1-370ml", 64, 64, 2);

      break;
    case LIQUIDCLADD:
      tft.setTextColor(TFT_WHITE);
      g_int_quantity++;
      tft.drawString("gourmand", 64, 64, 2);
      tft.drawString(String(g_int_quantity), 64, 64, 2);

      break;
    case LIQUIDREMOVE:
      tft.setTextColor(TFT_WHITE);
      g_int_quantity--;
      tft.drawString("radin", 64, 64, 2);
      tft.drawString(String(g_int_quantity), 64, 64, 2);

      break;

    case LIQUIDCLADDPLUS:
      tft.setTextColor(TFT_WHITE);
      g_int_quantity++;
      tft.drawString("gourmand+", 64, 64, 2);
      tft.drawString(String(g_int_quantity), 64, 64, 2);

      break;
    case LIQUIDREMOVEMINUS:
      tft.setTextColor(TFT_WHITE);
      g_int_quantity--;
      tft.drawString("radin-", 64, 64, 2);
      tft.drawString(String(g_int_quantity), 64, 64, 2);

      break;

      /*
      case SERVICE:
        // la pompe pisse dans le verre selon cequi a été choisis voir même les 2 pompes
        break;

      case ARRETURGENCE:
        // on arrête la pompe car elle a pissé partout
        tft.fillCircle(64, 64, 61, TFT_BLUE); /// expérimentale
        break;

      case FINISHED:
        // on allume les leds en clignotant vert car l'utilisateur peut enfin prendre son verre et boire
        break;

      case WAITNEWCUP:
        // on attend que l'utilisateur mettre son verre en allumant les leds d'une certaine couleur en clignotant

        break;
      */
    }
  } // fin pilotage sorties
}
