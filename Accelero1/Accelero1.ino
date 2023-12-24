#include <Arduino_LSM6DS3.h>


//La fonction checkIntegrity vérifie que la carte n'a pas été bougé lors de la capture d'images
//On renvoit true si le mouvement est perçu, sinon false.
bool checkIntegrity () {

  //Paramètre des valeurs par défaut:
  int degreesX = 0;
  int degreesY = 0;
  int degreesXplus = 0;
  int degreesYplus =0;

  float x, y, z;

  //On auvre la lecture des données de l'accéléromètres
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }

  //Conversion en degré (°)
  x = 100*x;
  degreesX = x;
  y = 100*y;
  degreesY = y;

  // Ici on cherche à vérifié que entre 2 positions précédentes, 
  // il n'y a pas une distance supérieur à 5° sur les axes X et Y

  //On place la valeur de la première donnée capturé (degreesX ou degreesY)
  //dans degreesXplus ou degreesYplus si elles valent 0
  if ((degreesXplus == 0) || (degreesYplus == 0)) {
    degreesXplus = degreesX;
    degreesYplus = degreesY;
  }

  //On vérifit que la différence absolu entre la première mesure d'angle et celle juste après ne dépasse
  //pas 5°. Cette valeur a été déterminé expérimentalement pour garantir un véritable mouvement volontaire.
  //On remet alors les valeurs degreesXplus et degreesYplus à 0 pour éviter l'envoi répété de notification 
  //si la carte reste en position "dégradé".
  if ((abs(degreesXplus - degreesX) >= 5) || (abs(degreesYplus - degreesY) >= 5)) {
    degreesXplus = 0;
    degreesYplus = 0;
    return true;
  }
  return false;
}


//Structure d'utilisation de la fonction précédente
void setup() {
  // Serial.begin(9600);
  // while (!Serial);

  // if (!IMU.begin()) {
  //   Serial.println("Failed to initialize IMU!");
  //   while (1);
  // }
}

//Interprétation toutes les 0.5s de l'inclinaison de la carte
void loop() {
  Serial.print(checkIntegrity());
  delay(500);
}
