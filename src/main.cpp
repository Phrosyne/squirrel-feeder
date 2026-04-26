#define ENABLE_USER_AUTH
#define ENABLE_FIRESTORE

#include <FirebaseClient.h>
#include "ExampleFunctions.h" // Provides the functions used in the examples.
#include "esp_wpa2.h"
#include "hush.h"
#include "WiFi.h"
#include "esp_sleep.h"

SSL_CLIENT ssl_client;

using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);



UserAuth user_auth(API_KEY, user, pass, 3000);
FirebaseApp app;

Firestore::Documents Docs;

AsyncResult firestoreResult;

void setup()
{
  Serial.begin(115200);

  Serial.print("Connecting");

  WiFi.disconnect(true); // Disconnect from any previous networks
  WiFi.mode(WIFI_STA);

  // Configure WPA2 Enterprise Enterprise Identity and Password
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)id, strlen(id));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)id, strlen(id));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));

  // Enable WPA2 Enterprise
  esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println(F("Connected!"));

  set_ssl_client_insecure_and_buffer(ssl_client);

  Serial.println("Initializing app...");
  initializeApp(aClient, app, getAuth(user_auth), auth_debug_print, " authTask");

  unsigned long ms = millis();

  while (!app.ready())
  {
    app.loop();
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Signed in!!");
  Serial.println(app.getToken());

  int weight = 80085;

  Document<Values::Value> doc;
  doc.add("foodLevel", Values::Value(Values::IntegerValue(weight))); // replace
  // doc.add("last_updated", Values::Value(Values::StringValue(String(millis()))));

  PatchDocumentOptions patchOptions(DocumentMask("foodLevel"), DocumentMask(), Precondition());

  Serial.println("Patching document...");
  String documentPath = "food/arbor"; // collection/document

  
  bool success = Docs.patch(aClient, Firestore::Parent(FIREBASE_PROJECT_ID, "(default)"), documentPath, patchOptions, doc);

  if (success) {
    Serial.println(success ? "Doc patched" : "Timeout");
    Serial.flush();

    // uint64_t sleep_duration = 1000000;
    // esp_sleep_enable_timer_wakeup(sleep_duration);
    // esp_deep_sleep_start();
  }


}

void loop() {}
