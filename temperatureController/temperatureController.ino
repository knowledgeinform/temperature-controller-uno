///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2019 The Johns Hopkins University Applied Physics Laboratory LLC (JHU/APL).  All Rights Reserved.
//
// This material may be only be used, modified, or reproduced by or for the U.S. Government pursuant to the license
// rights granted under the clauses at DFARS 252.227-7013/7014 or FAR 52.227-14. For any other permission, please
// contact the Office of Technology Transfer at JHU/APL: Telephone: 443-778-2792, Internet: www.jhuapl.edu/ott
//
// NO WARRANTY, NO LIABILITY. THIS MATERIAL IS PROVIDED "AS IS." JHU/APL MAKES NO REPRESENTATION OR WARRANTY WITH
// RESPECT TO THE PERFORMANCE OF THE MATERIALS, INCLUDING THEIR SAFETY, EFFECTIVENESS, OR COMMERCIAL VIABILITY, AND
// DISCLAIMS ALL WARRANTIES IN THE MATERIAL, WHETHER EXPRESS OR IMPLIED, INCLUDING (BUT NOT LIMITED TO) ANY AND ALL
// IMPLIED WARRANTIES OF PERFORMANCE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT OF
// INTELLECTUAL PROPERTY OR OTHER THIRD PARTY RIGHTS. ANY USER OF THE MATERIAL ASSUMES THE ENTIRE RISK AND LIABILITY
// FOR USING THE MATERIAL. IN NO EVENT SHALL JHU/APL BE LIABLE TO ANY USER OF THE MATERIAL FOR ANY ACTUAL, INDIRECT,
// CONSEQUENTIAL, SPECIAL OR OTHER DAMAGES ARISING FROM THE USE OF, OR INABILITY TO USE, THE MATERIAL, INCLUDING,
// BUT NOT LIMITED TO, ANY DAMAGES FOR LOST PROFITS.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Use local versions, as some are modified
#include "Thermocouple.h"
#include "MAX6675_Thermocouple.h"
#include "Cmd.h"
#include "JSON_utils.h"
#include "temperatureController.h"

static void PID_serialize(PID_params * obj, char ** fields, int fields_cnt, bool child)
{
  if (fields_cnt < 0) {
    return;
  }

  if (fields_cnt == 0) {
    JSONstart();
    //JSONitem("ID", &(obj->ID));
    char s[] PROGMEM = "p";
    JSONitem(s, &(obj->p));
    char s1[] PROGMEM = "i";
    JSONitem(s1, &(obj->i));
    char s2[] PROGMEM = "d";
    JSONitem(s2, &(obj->d));
    char s3[] PROGMEM = "rlow";
    JSONitem(s3, &(obj->rlow));
    char s4[] PROGMEM = "rhigh";
    JSONitem(s4, &(obj->rhigh));
    char s5[] PROGMEM = "DIR";
    JSONlastItem(s5, &(obj->DIR));
    JSONend(child);
  } else {
    // fields[1] exists --> see logic in Controller_serialize
    // currently, there are no sub-structures though...
  }
}

void PID_params::Serialize(PID_params * obj, char ** fields, int fields_cnt, bool child) {
  PID_serialize(obj, fields, fields_cnt, child);
}

void SetPID(PID_params *new_pp) {
  //Update the controller settings using the local values in a pid_params struct
  new_pp->pid->SetTunings(new_pp->p, new_pp->i, new_pp->d);
  new_pp->pid->SetOutputLimits(new_pp->rlow, new_pp->rhigh);
  new_pp->pid->SetControllerDirection(new_pp->DIR);
}

static void PID_assign(PID_params * obj, char ** fields, int fields_cnt, char * val)
{
  if (fields_cnt < 1) {
    return;
  }

  char * field;
  if (fields_cnt == 1) {
    field = fields[0];
    if (!strncmp(field, "p", 1)) {
      obj->p = atof(val);
    } else if (!strncmp(field, "i", 1)) {
      obj->i = atof(val);
    } else if (!strncmp(field, "d", 1)) {
      obj->d = atof(val);
    } else if (!strncmp(field, "rlow", 4)) {
      obj->rlow = atof(val);
    } else if (!strncmp(field, "rhigh", 4)) {
      obj->rhigh = atof(val);
    } else if (!strncmp(field, "DIR", 3)) {
      obj->DIR = !!atoi(val);
    }

    SetPID(obj);
    obj->Serialize(obj, NULL, 0, false);
  } else {
    // currently, there are no sub-structures
  }
}

void PID_params::assign(PID_params * obj, char ** fields, int fields_cnt, char * val) {
  PID_assign(obj, fields, fields_cnt, val);
}

static void Safety_serialize(Safety * obj, char ** fields, int fields_cnt, bool child)
{
  if (fields_cnt < 0) {
    return;
  }

  if (fields_cnt == 0) {
    JSONstart();
    //JSONitem("ID", &(obj->ID));
    JSONitem("EN", &(obj->EN));
    JSONitem("FSP", &(obj->FSP));
    JSONitem("ULAL", &(obj->ULAL));
    JSONlastItem("LLAL", &(obj->LLAL));
    JSONend(child);
  } else {
    // fields[1] exists --> see logic in Controller_serialize
    // currently, there are no sub-structures though...
  }
}

void Safety::Serialize(Safety * obj, char ** fields, int fields_cnt, bool child) {
  Safety_serialize(obj, fields, fields_cnt, child);
}

static void Safety_assign(Safety * obj, char ** fields, int fields_cnt, char * val)
{
  if (fields_cnt < 1) {
    return;
  }

  char * field;
  if (fields_cnt == 1) {
    field = fields[0];
    if (!strncmp(field, "EN", 2)) {
      obj->EN = !!atoi(val);
    } else if (!strncmp(field, "FSP", 3)) {
      obj->FSP = atof(val);
    } else if (!strncmp(field, "ULAL", 4)) {
      obj->ULAL = atof(val);
    } else if (!strncmp(field, "LLAL", 4)) {
      obj->LLAL = atof(val);
    }
    obj->Serialize(obj, NULL, 0, false);
  } else {
    // currently, there are no sub-structures
  }
}

void Safety::assign(Safety * obj, char ** fields, int fields_cnt, char * val) {
  Safety_assign(obj, fields, fields_cnt, val);
}

static void LoopVars_serialize(LoopVars * obj, char ** fields, int fields_cnt, bool child)
{
  if (fields_cnt < 0) {
    return;
  }

  if (fields_cnt == 0) {
    JSONstart();
    //JSONitem("ID", &(obj->ID));
    char s[] PROGMEM = "pvUnits";
    JSONitem(s, &(obj->pvUnits));
    char s1[] PROGMEM = "pv";
    JSONitem(s1, &(obj->pv));
    char s2[] PROGMEM = "sp";
    JSONitem(s2, &(obj->sp));
    char s3[] PROGMEM = "co";
    JSONlastItem(s3, &(obj->co));
    JSONend(child);
  } else {
    // fields[1] exists --> see logic in Controller_serialize
    // currently, there are no sub-structures though...
  }
}

void LoopVars::Serialize(LoopVars * obj, char ** fields, int fields_cnt, bool child) {
  LoopVars_serialize(obj, fields, fields_cnt, child);
}

static void LoopVars_assign(LoopVars * obj, char ** fields, int fields_cnt, char * val)
{
  if (fields_cnt < 1) {
    return;
  }

  char * field;
  if (fields_cnt == 1) {
    field = fields[0];
    if (!strncmp(field, "pvUnits", 7)) {
      obj->pvUnits = String(val);
    } else if (!strncmp(field, "sp", 2)) {
      obj->sp = atof(val);
    }
    obj->Serialize(obj, NULL, 0, false);
  } else {
    // currently, there are no sub-structures
  }
}

void LoopVars::assign(LoopVars * obj, char ** fields, int fields_cnt, char * val)
{
  LoopVars_assign(obj, fields, fields_cnt, val);
}

static void Loop_serialize(Loop * obj, char ** fields, int fields_cnt, bool child)
{
  if (fields_cnt < 0) {
    return;
  }

  if (fields_cnt == 0) {
    JSONstart();
    //JSONitem("ID", &(obj->ID));
    char s[] PROGMEM = "status";
    JSONitem(s, &(obj->status));
    char s1[] PROGMEM = "pid";
    JSONkey(s1);
    obj->pid.Serialize(&(obj->pid), NULL, 0, true);
    char s2[] PROGMEM = "loopVars";
    JSONkey(s2);
    obj->loopVars.Serialize(&(obj->loopVars), NULL, 0, true);
    char s3[] PROGMEM = "safety";
    JSONkey(s3);
    obj->safety.Serialize(&(obj->safety), NULL, 0, false);
    JSONend(child);
  } else {
    // fields[1] exists --> see logic in Controller_serialize
    if (!strncmp(fields[1], "pid", 3)) {
      obj->pid.Serialize(&(obj->pid), &(fields[1]), fields_cnt - 1, fields_cnt != 1);
    } else if (!strncmp(fields[1], "loopVars", 8)) {
      obj->loopVars.Serialize(&(obj->loopVars), &(fields[1]), fields_cnt - 1, fields_cnt != 1);
    } else if (!strncmp(fields[1], "safety", 6)) {
      obj->safety.Serialize(&(obj->safety), &(fields[1]), fields_cnt - 1, fields_cnt != 1);
    }
  }
}

void Loop::Serialize(Loop * obj, char ** fields, int fields_cnt, bool child) {
  Loop_serialize(obj, fields, fields_cnt, child);
}

static void Loop_assign(Loop * obj, char ** fields, int fields_cnt, char * val)
{
  if (fields_cnt < 1) {
    return;
  }

  char * field;
  if (fields_cnt == 1) {
    field = fields[0];
    // currently there's nothing settable
    obj->Serialize(obj, NULL, 0, false);
  } else {
    if (!strncmp(fields[0], "pid", 3)) {
      obj->pid.assign(&(obj->pid), &(fields[1]), fields_cnt - 1, val);
    } else if (!strncmp(fields[0], "loopVars", 8)) {
      obj->loopVars.assign(&(obj->loopVars), &(fields[1]), fields_cnt - 1, val);
    } else if (!strncmp(fields[0], "safety", 6)) {
      obj->safety.assign(&(obj->safety), &(fields[1]), fields_cnt - 1, val);
    }
  }
}

void Loop::assign(Loop * obj, char ** fields, int fields_cnt, char * val) {
  Loop_assign(obj, fields, fields_cnt, val);
}

static void Controller_serialize(Controller * obj, char ** fields, int fields_cnt, bool child)
{
  if (fields_cnt < 0) {
    return;
  }

  if (fields_cnt == 0) {
    // fields cnt now indicates whether a child or not
    JSONstart();
    //JSONitem("ID", &(obj->ID));
    JSONitem("status", &(obj->status));
    JSONitem("address", &(obj->address));
    JSONitem("numLoops", &(obj->numLoops));
    JSONkey("loops");
    JSONstartArray();
    size_t i = 0;
    for (i = 0; i+1 < ((*obj).numLoops); i++) {
      //char loopName[8];
      //sprintf(loopName, "loops%i",i);
      //JSONkey(loopName);
      obj->loops[i].Serialize(&(obj->loops[i]), NULL, 0, true);
    }
    obj->loops[i].Serialize(&(obj->loops[i]),  NULL, 0, false);
    JSONendArray(false);
    JSONend(child);
  } else {
    // start at one higher-index than fields_cnt because fields_cnt is pre-decremented
    // (i.e. decremented before calling this function)
    // Thus, if fields_cnt > 0, then there are at least 2 fields and so fields[1]
    // has certainly been allocated and will not seg-fault
    if (!strncmp(fields[1], "loops", 5)){
      int loopNum;
      loopNum = getLoopNumber(fields[1]);
      if (loopNum >= 0 && loopNum < obj->numLoops) {
        obj->loops[loopNum].Serialize(&(obj->loops[loopNum]), &(fields[1]), fields_cnt - 1, fields_cnt != 1);
      }
    }
  }
}

void Controller::Serialize(Controller * obj, char ** fields, int fields_cnt, bool child) {
  Controller_serialize(obj, fields, fields_cnt, child);
}

static void Controller_assign(Controller * obj, char ** fields, int fields_cnt, char * val)
{
  if (fields_cnt < 1) {
    return;
  }

  char * field;
  if (fields_cnt == 1) {
    field = fields[0];
    if (!strncmp(field, "address",7)){
      obj->address = atoi(val);
    }
    obj->Serialize(obj, NULL, 0, false);
  } else {
    if (!strncmp(fields[0], "loops", 5)){
      int loopNum;
      loopNum = getLoopNumber(fields[0]);
      if (loopNum >= 0 && loopNum < obj->numLoops) {
        obj->loops[loopNum].assign(&(obj->loops[loopNum]), &(fields[1]), fields_cnt - 1, val);
      }
    }
  }
}

void Controller::assign(Controller * obj, char ** fields, int fields_cnt, char * val) {
  Controller_assign(obj, fields, fields_cnt, val);
}

ControllerManager cm;

void initializeOutputPins() {
  int * outputs = new int[6];
  outputs[0] = controllerOutput0;
  outputs[1] = controllerOutput1;
  outputs[2] = controllerOutput2;
  outputs[3] = controllerOutput3;
  outputs[4] = controllerOutput4;
  outputs[5] = controllerOutput5;
  for(int i = 0; i < sizeof(outputs) / sizeof(int); i++) {
    // Serial.println(outputs[i]);
    pinMode(outputs[i], OUTPUT);
  }
  cm.outputs = outputs;
}

void initializeInputPins() {
  // Using interrupt 0 on digital pin 2.
  pinMode(2, INPUT);
  digitalWrite(2, LOW);
}

void initializeThermocouples(){
  // put your setup code here, to run once:
  // Instantiation:
  /**
  SCK_PIN - SCK digital port number.
  CS_PIN - CS digital port number.
  SO_PIN - SO digital port number.
  */

  MAX6675_Thermocouple tcsArray[] = {MAX6675_Thermocouple(SCK_PIN, CS_PIN0, SO_PIN),
    MAX6675_Thermocouple(SCK_PIN, CS_PIN1, SO_PIN),
    MAX6675_Thermocouple(SCK_PIN, CS_PIN2, SO_PIN),
    MAX6675_Thermocouple(SCK_PIN, CS_PIN3, SO_PIN),
    MAX6675_Thermocouple(SCK_PIN, CS_PIN4, SO_PIN),
    MAX6675_Thermocouple(SCK_PIN, CS_PIN5, SO_PIN)};
  TCs tcs PROGMEM = {sizeof(tcsArray) / sizeof(MAX6675_Thermocouple),
    tcsArray};
    cm.tcs = tcs;
}

void initializeController() {
  Controller * c = new Controller();
  c->status = F("Okay");
  c->address = 1;
  Loop* loops PROGMEM = new Loop[cm.tcs.length];
  for(int i = 0; i < cm.tcs.length; i++) {
    loops[i].status = F("Okay");
    // intiailize as PI controller
    loops[i].pid.p = 1;
    loops[i].pid.i = 5;
    loops[i].pid.d = 0;
    loops[i].pid.rlow = 0; // range-low
    loops[i].pid.rhigh = 3000; // range-high
    loops[i].pid.DIR = DIRECT; // direction
    loops[i].pid.windowStartTime = millis();

    loops[i].loopVars.pv = 0; // process-variable
    loops[i].loopVars.pvUnits = F("F"); // process-variable units
    loops[i].loopVars.sp = 0; // set-point
    loops[i].loopVars.co = 0; // controller-output

    loops[i].pid.pid = new PID(&(loops[i].loopVars.pv), &(loops[i].loopVars.co), &(loops[i].loopVars.sp), loops[i].pid.p, loops[i].pid.i, loops[i].pid.d, loops[i].pid.DIR);
    loops[i].pid.pid->SetOutputLimits(loops[i].pid.rlow, loops[i].pid.rhigh);
    loops[i].pid.pid->SetMode(AUTOMATIC);

    loops[i].safety.EN = false; // do not EN safety on start-up
    loops[i].safety.FSP = 0;
    loops[i].safety.ULAL = 0;
    loops[i].safety.LLAL = 0;
  }
  c->loops = loops;
  c->numLoops = cm.tcs.length;
  cm.c = c;
}


void initializePins() {
  initializeOutputPins();
  initializeInputPins();
  initializeThermocouples();
  // return tcs;
}

int getLoopNumber(char *tok) {
  int loopNum;
  loopNum = -1;
  // start of tok is assumed to be loops based off function name
  if (strchr(tok,']') != NULL) {
    loopNum = atoi(strchr(tok,'[') + 1);
    if (loopNum < 0 || loopNum > 5){
      Serial.println(F("Invalid loop number"));
    }
  }
  return loopNum;
}

void GET_(int arg_cnt, char **args){
  //Serial user requested controller data.
  //Parse namespace style string (. delimited) to determine field and return appropriate data

  if (arg_cnt < 2) {
    Serial.println("c");
    return;
  }

  results r;
  if (arg_cnt >= 2) {
    // parse the requested address
    r = cmdLineParse(args[1], ".");
  }
  int r_cnt = r.argc;
  char** r_argv = r.argv;
  if (r.argc < 1) {
    return;
  }
  if (!strcmp(r_argv[0], "c")){
    // serializing at 0 because there's no guarantee that there's another
    // element in the array. But subtracting 1 from r_cnt to indicate that
    // the 0th element has been "used" from the argv array

    // the logic behind the "child" boolean is as follows:
    // child == true indicates that this is a child object and should have a comma following
    // the last entry in the json
    // child == false indicates that this the top-level object and should not have a comma
    // following the last entry in the json
    // Thus, if r_cnt == 1, then there are no more r_argv's except for the 0th one
    // which has already been identified as "c". Therefore, this is *NOT* a child
    // which means child should be set to false.
    cm.c->Serialize(cm.c, &(r_argv[0]), r_cnt - 1, r_cnt != 1);

  }
}


void SET_(int arg_cnt, char **args){
  // format:
  // set obj.subobj.field val
  // thus, there must be always be exactly 3 arguments
  if (arg_cnt != 3) {
    // unrecognized input is ignored
    return;
  }
  results r;
  // parse the requested address
  r = cmdLineParse(args[1], ".");
  if (r.argc < 2) {
    // the controller itself is not settable so there must be at least 2 arguments
    // from the parsed address
    return;
  }
  int r_cnt = r.argc;
  char** r_argv = r.argv;

  //Check for menu selections (address, pid, loopVars, safety) and process if found
  char* val = args[arg_cnt - 1];
  if (!strncmp(r_argv[0], "c", 1) && strlen(r_argv[0]) == 1) {
    cm.c->assign(cm.c, &(r_argv[1]), r_cnt - 1, val);
  } else {
    // error handling
  }
}

void setup() {
  Serial.setTimeout(30); // modbus-rtu timeout is 3.5xchar time, which equals 4 ms at 9600 baud; thus, 30 ms is more than adequate
  Serial.begin(57600);
  Serial.println("Helloworld!");
  initializePins();
  initializeController();
  //attachInterrupt(digitalPinToInterrupt(2), serialInterrupt, CHANGE);

  cmdInit(&Serial);
  cmdAdd("get", GET_);
  cmdAdd("set", SET_);
}

static int i = 0;
void loop() {
  cmdPoll();

  //We also want the trap firing to have low jitter. So it gets attention between each heater iteration.
  cm.c->loops[i].loopVars.pv = cm.tcs.tcArray[i].readFahrenheit();
  if (isnan(cm.c->loops[i].loopVars.pv)) {
    cm.c->loops[i].loopVars.pv = 0;
  } else {
    if (!cm.c->loops[i].pid.pid->Compute()) {
      //Serial.println("NOT COMPUTING");
    }
    if (millis() - cm.c->loops[i].pid.windowStartTime >= (unsigned long) cm.c->loops[i].pid.rhigh) { //time to shift the Relay Window
      cm.c->loops[i].pid.windowStartTime = millis();
    }
    if (millis() - cm.c->loops[i].pid.windowStartTime < cm.c->loops[i].loopVars.co) {
//      digitalWrite(cm.outputs[i], HIGH);
    } else {
//      digitalWrite(cm.outputs[i], LOW);
    }
  }
  i = (i + 1) % cm.tcs.length;

}
