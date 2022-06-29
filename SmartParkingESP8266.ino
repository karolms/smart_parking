#include <Arduino.h>

#include<Arduino.h>
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP8266
 *
 * Copyright (c) 2022 mobizt
 *
 */

/** This example will show how to authenticate as a user with Email and password.
 *
 * You need to enable Email/Password provider.
 * In Firebase console, select Authentication, select Sign-in method tab,
 * under the Sign-in providers list, enable Email/Password provider.
 *
 * From this example, the user will be granted to access the specific location that matches
 * the user uid.
 *
 * This example will modify the database rules to set up the security rule which need to
 * guard the unauthorized access with the user Email.
 */

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#include <SoftwareSerial.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "CA Suites 348"
#define WIFI_PASSWORD "348casuites"

/** 2. Define the API key
 *
 * The API key (required) can be obtained since you created the project and set up
 * the Authentication in Firebase console. Then you will get the API key from
 * Firebase project Web API key in Project settings, on General tab should show the
 * Web API Key.
 *
 * You may need to enable the Identity provider at https://console.cloud.google.com/customer-identity/providers
 * Select your project, click at ENABLE IDENTITY PLATFORM button.
 * The API key also available by click at the link APPLICATION SETUP DETAILS.
 *
 */
#define API_KEY "AIzaSyC3aKF8EolxZkUz06NkmDVP-NjLKt2eo1E"

/* 3. Define the user Email and password that already registerd or added in your project */
#define USER_EMAIL "teste@gmail.com"
#define USER_PASSWORD "123456"


/* 4. If work with RTDB, define the RTDB URL */
#define DATABASE_URL "https://sd-cesar-c642a-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/** 5. Define the database secret (optional)
 *
 * This database secret needed only for this example to modify the database rules
 *
 * If you edit the database rules yourself, this is not required.
 */
#define DATABASE_SECRET "mYTe5s1grtgt0vqf9uLPyUV40jH1G5xAABGFFIVK"
//dyXJOIVnlbWrvf9DCQ6bksarf662

//Pinos de comunicacao serial com a ST Núcleo
#define Pin_ST_NUCLEO_RX    5  //Pino D1 da placa Node MCU
#define Pin_ST_NUCLEO_TX    4  //Pino D2 da placa Node MCU

/* 6. Define the Firebase Data object */
FirebaseData fbdo;

/* 7. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 8. Define the FirebaseConfig data for config data */
FirebaseConfig config;

SoftwareSerial SSerial(Pin_ST_NUCLEO_RX, Pin_ST_NUCLEO_TX);

unsigned long dataMillis = 0;

bool setVagas(int vagas)
{
   return Firebase.setInt(fbdo, "/vagas", vagas); 
}

bool setIntensidade(int intensidade)
{
   return Firebase.setString(fbdo, "/intensidade", descricaoIntensidade(intensidade)); 
}

String descricaoIntensidade(int intensidade) {
  if (intensidade > 1000) {
    return "alta";
  } else if (intensidade > 500) {
    return "media";
  } else {
    return "baixa";
  }
}

void setup()
{
    pinMode(BUILTIN_LED,OUTPUT);
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    //auth.user.email = USER_EMAIL;
    //auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;

    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);

    String base_path = "/";

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    int maxVagas = Firebase.getInt(fbdo, "/maxVagas");
    setVagas(maxVagas);

    /** Now modify the database rules (if not yet modified)
     *
     * The user, <user uid> in this case will be granted to read and write
     * at the certain location i.e. "/UsersData/<user uid>".
     *
     * If you database rules has been modified, please comment this code out.
     *
     * The character $ is to make a wildcard variable (can be any name) represents any node key
     * which located at some level in the rule structure and use as reference variable
     * in .read, .write and .validate rules
     *
     * For this case $userId represents any <user uid> node that places under UsersData node i.e.
     * /UsersData/<user uid> which <user uid> is user UID.
     *
     * Please check your the database rules to see the changes after run the below code.
     */

    /** path for user data is now "/UsersData/<user uid>"
     * The user UID can be taken from auth.token.uid
     */

    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    SSerial.begin(115200);
  
    Serial.println("Serial by hardware!");
  
    // set the data rate for the SoftwareSerial port
    SSerial.println("Serial by software!");
}

void loop()
{
    String message = SSerial.readStringUntil('\n');
    Serial.println(message);

    if (message[0] == 'P')
    {
      message.remove(0, 1);
      int potenciometro = message.toInt();
      bool didSet = setVagas(potenciometro);
      Serial.printf("Set vagas... %s\n", didSet ? "ok" : fbdo.errorReason().c_str());
    } else if (message[0] == 'I') {
      message.remove(0, 1);
      int intensidade = message.toInt();
      bool didSet = setIntensidade(intensidade);
      Serial.printf("Set intensidade... %s\n", didSet ? "ok" : fbdo.errorReason().c_str());
    }
}
