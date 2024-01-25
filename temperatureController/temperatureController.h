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


#ifndef TEMP_CONTROLLER_H
#define TEMP_CONTROLLER_H

#include <PID_v1.h>

/**
Pin definitions
*/

// output pins
#define controllerOutput0 52
#define controllerOutput1 50
#define controllerOutput2 48
#define controllerOutput3 46
#define controllerOutput4 44
#define controllerOutput5 42

// chip-select pins
#define CS_PIN0 53
#define CS_PIN1 51
#define CS_PIN2 49
#define CS_PIN3 47
#define CS_PIN4 45
#define CS_PIN5 43

// Clock and output pins
#define SCK_PIN   41
#define SO_PIN    39
#define LTRAP_PIN 3
#define STRAP_PIN 4

typedef struct PID_tag
{
  String  ID;
  double  p;
  double  i;
  double  d;
  double  rlow;
  double  rhigh;
  unsigned long windowStartTime;
  int     DIR;
  PID*    pid;
  void    Serialize(PID_tag *, char **, int, bool);
  void    assign(PID_tag *, char **, int, char *);
} PID_params;

typedef struct Safety_tag
{
  String      ID;
  bool        EN;
  float       FSP;
  float       ULAL;
  float       LLAL;
  void        Serialize(Safety_tag *, char **, int, bool);
  void        assign(Safety_tag *, char **, int, char *);
} Safety;

typedef struct LoopVars_tag
{
  String  ID;
  String  pvUnits;
  double  pv;
  double  sp;
  double  co;
  void    Serialize(LoopVars_tag *, char **, int, bool);
  void    assign(LoopVars_tag *, char **, int, char *);
} LoopVars;

typedef struct Loop_tag
{
  String      ID;
  String      status;
  PID_params  pid;
  LoopVars    loopVars;
  Safety      safety;
  void        Serialize(Loop_tag *, char **, int, bool);
  void        assign(Loop_tag *, char **, int, char *);
} Loop;

typedef struct Controller_tag
{
  String  ID;
  String  status;
  int     address;
  size_t  numLoops;
  Loop*   loops;
  void    Serialize(Controller_tag *, char **, int, bool);
  void    assign(Controller_tag *, char **, int, char *);
} Controller;

typedef struct TCs_tag
{
  size_t                length;
  MAX6675_Thermocouple* tcArray;
} TCs;

typedef struct ControllerManager_tag
{
  TCs           tcs;
  Controller*   c;
  int*          outputs;
} ControllerManager;

#endif // TEMP_CONTROLLER_H
