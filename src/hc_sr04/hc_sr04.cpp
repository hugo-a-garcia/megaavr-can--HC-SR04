#include "hc_sr04.h"
#include "Arduino.h"
#include "../libcanard/canard_dsdl.h"

#define TRIGGER_PIN 9
#define ECHO_PIN 10

static float duration, distance;

void publishUltrasoundDistance(CanardInstance canard_instance, const float distance)
{
    static CanardTransferID transfer_id;
    uint8_t payload[4] = {0, 0, 0, 0};

    //Serilize the distance
    canardDSDLSetF32(payload, 0, distance);

    const CanardTransfer transfer = {
        timestamp_usec : 0,
        priority : CanardPriorityNominal,
        transfer_kind : CanardTransferKindMessage,
        port_id : UltrasoundMessageSubjectID,
        remote_node_id : CANARD_NODE_ID_UNSET,
        transfer_id : transfer_id,
        payload_size : 4, // sizeof(payload),
        payload : &payload[0],
    };
    ++transfer_id;
    (void)canardTxPush(&canard_instance, &transfer);
}
 
void setup_hc_sr04() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}
 
float loop_hc_sr04(CanardInstance canard_instance) {
   
  // Write a pulse to the HC-SR04 Trigger Pin
  
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Measure the response from the HC-SR04 Echo Pin
 
  duration = pulseIn(ECHO_PIN, HIGH, 1000000);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  
  distance = (duration / 2) * 0.0343;
  
  // Send results to Serial Monitor
  publishUltrasoundDistance(canard_instance, distance);
  Serial.print("Distance = ");
  if (distance >= 400 || distance <= 2) {
     Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  Serial.flush();

  return distance;
}
