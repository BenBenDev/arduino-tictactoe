//
//
//     _|_|_|_|_|  _|            _|_|_|_|_|                    _|_|_|_|_|
//         _|            _|_|_|      _|      _|_|_|    _|_|_|      _|      _|_|      _|_|
//         _|      _|  _|            _|    _|    _|  _|            _|    _|    _|  _|_|_|_|
//         _|      _|  _|            _|    _|    _|  _|            _|    _|    _|  _|
//         _|      _|    _|_|_|      _|      _|_|_|    _|_|_|      _|      _|_|      _|_|_|
//

int joueur;  // vaut 1 ou 2 = joueur en cours
int place[9]; //les 9 cases du plateau
int btn; // bouton en cours
//   0   1   2
//   3   4   5
//   6   7   8      et valeur=0 non joué/1 joueur1/2 joueur2



void setup() {

    //déclare les boutons   9 premiers
    for  (int btni = 0 ; btni < 9 ; btni++) {
        pinMode (btni + 0, INPUT);
    }

    //déclare les LEDs      18 suivants
    for  (int ledi = 0 ; ledi < 18 ; ledi++) {
        pinMode (ledi + 9, OUTPUT);
    }
    // val               => pin du BOUTON
    // val + 9 * joueur  => pin LED à allumer pour le joueur
    // exemple  :
    //  val=3   bouton pin 3
    //          LED pin 3 + 9*1 = place[12] pour joueur 1
    //          LED pin 3 + 9*2 = place[21] pour joueur 2

    Serial.begin(9600);
}


void loop() {
    int finPartie = false;
    joueur = 1;
    Serial.println ("Debut de la partie");
    debutPartie();

    while (finPartie == false) {
        boolean isValid = false;
        do {
            //attente de l'appui d'un bouton
            btn = lectureBouton();
            isValid = verif(btn, joueur);
            if (!isValid) {
                mauvaiseReponse();
            } else {
                allume(btn, joueur);
            }
        } while (!isValid);
        //le joueur prend la place sur le bouton
        place[btn] = joueur;
        afficheJeu();

        // test des led allumées...
        finPartie = testJeu();
        if (!finPartie) {
            Serial.println ("changement de joueur");
            joueur = (joueur == 1) ? 2 : 1 ;
        }
    }
}


void debutPartie() {
    // snake to welcome you ;)
    for (int i = 0 ; i < 2 ; i++) {
        int yy = 1;
        do {
            for (int ii = 0 ; ii < 9 ; ii++) {
                digitalWrite(ii * yy, 1);
                delay(100);
                digitalWrite(ii * yy, 0);
                delay(50);
            }
            yy++;
        } while (yy < 2);
    }
}


int lectureBouton() {
    int buttonState = 0;
    int val = 0;
    // lecture de l'état de tous les boutons
    for (int ii = 1; ii < 10; ii++) {
        buttonState = digitalRead(ii);
        if (buttonState == 1) {
            //ce bouton est appuyé
            val = ii;
            break;
        }
    }
    Serial.print ("Bouton : ");
    Serial.println (val);
    return val;  // renvoie le numero du bouton appuyé
}

boolean verif(int pos, int joueur) {
    return (place[pos] == 0 ) ? true : false;
}

void mauvaiseReponse() {
    // flash de mauvaise réponse
    for (int i = 0 ; i < 2 ; i++) {
        int yy = 1;
        do {
            for (int ii = 0 ; ii < 9 ; ii++) {
                digitalWrite(ii * yy, 1);
            }
            delay(200);
            for (int ii = 0 ; ii < 9 ; ii++) {
                digitalWrite(ii * yy, 0);
            }
            delay(100);
            yy++;
        } while (yy < 2);
    }
}

void allume(int btn, int joueur) {
    int ii;
    for (ii = 0 ; ii < 3; ii++) {
        digitalWrite(ii, 1); delay(100);
        digitalWrite(ii, 0); delay(50);
    }
    digitalWrite(ii, 1);
}

void afficheJeu() {
    for (int ii = 0 ; ii < 9; ii++) {
        if (place[ii] != 0) {
            //allume la led correspondant au joueur
            digitalWrite(ii * joueur , 1);
        }
    }
}

boolean testJeu() {
    boolean alignement = false;
    if (( (place[0] == place[1]) && (place[1] == place[2]) && (place[2] == joueur) )   ||
        ( (place[3] == place[4]) && (place[4] == place[5]) && (place[5] == joueur) )   ||
        ( (place[6] == place[7]) && (place[7] == place[8]) && (place[8] == joueur) )   ||
        ( (place[0] == place[3]) && (place[3] == place[6]) && (place[6] == joueur) )   ||
        ( (place[1] == place[4]) && (place[4] == place[7]) && (place[7] == joueur) )   ||
        ( (place[2] == place[5]) && (place[5] == place[8]) && (place[8] == joueur) )   ||
        ( (place[0] == place[4]) && (place[4] == place[8]) && (place[8] == joueur) )   ||
        ( (place[2] == place[4]) && (place[4] == place[6]) && (place[6] == joueur) ) )
    {
        alignement = true ;
        Serial.print ("Gooood joueur ");
        Serial.println (joueur);
    }  //  * # * # * # *  G A G N É ! ! * # * # * # * }
    return alignement;
}