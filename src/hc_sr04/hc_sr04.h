#include "../libcanard/canard.h"

#ifndef HC_SR04_H
#define HC_SR04_H

static const uint16_t UltrasoundMessageSubjectID = 1610;

void publishUltrasoundDistance(CanardInstance canard_instance, const float distance);
void setup_hc_sr04();
float loop_hc_sr04(CanardInstance canard_instance);

#endif /* HC_SR04_H */

