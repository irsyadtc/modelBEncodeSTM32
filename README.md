############################################################
#	Model B NMEA format message parser for STM32 Blue Pill
############################################################

## Description

This is a library dedicated to encode the content of NMEA messages receives by STM32 Blue Pill

## Setup
 
 Microcontrollers:
 - STM32F103C8T6 Blue Pill
 Connected to:
 - UBlox Neo 6 Module
 - OpenCR 1.0


## Methods

### From GPS
#### void GPGGAThread(char *tok); 
This method encodes GPGGA contents

#### void GPRMCThread(char *tok);
This method encodes GPRMC contents

#### void GPGLLhread(char *tok);
This method encodes GPGLL contents

#### GPGGA
- String getUtcTime(void);
- String getLat(void);
- String getNSInd(void);
- String getLong(void);
- String getEWInd(void);
- int getPosFix(void);
- int getSatellites(void);
- String getHDOP(void);
- String getMSLAlt(void);
- String getUnitMSL(void);
- String getGeoidSep(void);
- String getUnitGeoid(void);
- String getAgeDiffCorr(void);
- String getDiffRefSta(void);

#### GPRMC
UTC time, latitude, north-south indicator, longitude, east-west indicator get methods for GPRMC are the same with GPGGA
- String getStatus(void);
- String getSpdOGnd(void);
- String getCourOGnd(void);
- String getDate(void);
- String getMagnetVar(void);
- String getMode(void);

### GPGLL

### from OpenCR
#### void OCRC1Thread(char *tok);
This method encodes OCRC1 contents

- int getPowerB(void);
- int getHornB(void);
- int getSpdUp(void);
- int getSpdDwn(void);
- int getRgbR(void);
- int getRgbG(void);
- int getRgbB(void);

## Note
 - 
 - 
 - 

## STM32-OpenCR messages
### Transmit
Message | Description |
--------|-------------|
enc,1,2,3,4 | Encoder message
| 1: Right encoder instantaneous tick
| 2: Left encoder instantaneous tick
| 3: Right encoder accumulated tick
| 4: Left encoder accumulated tick
|
gps,1,2,3,4 | GPS latitude and longitude message
| 1: Latitude DDMM.MMMM (e.g. 4124.8963 = 41d 24.8963')
| 2: North-south indicator
| 3: Longitude DDDMM.MMMM (e.g. 08151.6838 = 81d 51.6838')
| 4: East-west indicator

### Receive
Message | Description |
--------|-------------|
reset | reset encoder

