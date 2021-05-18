#include "modelBEncondeSTM32.h"

modelBEncondeSTM32::modelBEncondeSTM32()
{

}


/**************************************
   Encode
 **************************************/
bool modelBEncondeSTM32::encode(char const* _buf)
{
    bool valid = false;
    char *sntc = strchr(_buf,'$');

    if(sntc)
    {
        //capture full sentence
        sntc = strtok(sntc,"\r\n");
        //verify_checksum
        valid = checksum(sntc);

        if(valid)
        {
            char *tok = strtok(sntc,"$,"); //capture header

            if(strcmp(tok,"GPGGA") == 0)
            {
                GPGGAThread(tok);
            }
            if(strcmp(tok,"GPRMC") == 0)
            {
                GPRMCThread(tok);
            }
            else if(strcmp(tok,"OCRC1") == 0)
            {
                OCRC1Thread(tok);
            }

            valid = true;
        }
        else
        {
//            Serial.println("invalid");
        }
    }
    return valid;

}

/***********************************
  getHeader()
***********************************/
String modelBEncondeSTM32::getHeader(void)
{
    return String(encode_header_ptr_esp);
}


/***********************************
   from GPS
 ***********************************/
/***********************************
  GPGGA
***********************************/
void modelBEncondeSTM32::GPGGAThread(char *tok)
{
    memset(GPGGA.parameter,0,sizeof GPGGA.parameter);
    //point to header
    encode_header_ptr_esp = tok;
    //capture parameter
    uint8_t count=0;
    do
    {
        tok = strtok(NULL,",*");
        GPGGA.parameter[count] = tok;
        count++;
    }while(count<GPGGA.size);

    UTCTime = String(GPGGA.parameter[0]);
    Lat = String(GPGGA.parameter[1]);
    NSInd = String(GPGGA.parameter[2]);
    Long = String(GPGGA.parameter[3]);
    EWInd = String(GPGGA.parameter[4]);

}

String modelBEncondeSTM32::getUtcTime(void)
{
	return UTCTime;
}
String modelBEncondeSTM32::getLat(void)
{
	return Lat;
}
String modelBEncondeSTM32::getNSInd(void)
{
	return NSInd;
}
String modelBEncondeSTM32::getLong(void)
{
	return Long;
}
String modelBEncondeSTM32::getEWInd(void)
{
	return EWInd;
}
int modelBEncondeSTM32::getPosFix(void)
{
	return GPGGA.parameter[5];
}
int modelBEncondeSTM32::getSatellites(void)
{
	return GPGGA.parameter[6];
}
String modelBEncondeSTM32::getHDOP(void)
{
	return GPGGA.parameter[7];
}
String modelBEncondeSTM32::getMSLAlt(void)
{
	return GPGGA.parameter[8];
}
String modelBEncondeSTM32::getUnitMSL(void)
{
	return GPGGA.parameter[9];
}
String modelBEncondeSTM32::getGeoidSep(void)
{
	return GPGGA.parameter[10];
}
String modelBEncondeSTM32::getUnitGeoid(void)
{
	return GPGGA.parameter[11];
}
String modelBEncondeSTM32::getAgeDiffCorr(void)
{
	return GPGGA.parameter[12];
}
String modelBEncondeSTM32::getDiffRefSta(void)
{
	return GPGGA.parameter[13];
}


/***********************************
   GPRMC
 ***********************************/
void modelBEncondeSTM32::GPRMCThread(char *tok)
{
     memset(GPRMC.parameter,0,sizeof GPRMC.parameter);
     //point to header
     encode_header_ptr_esp = tok;
     //capture parameter
     uint8_t count=0;
     do
     {
         tok = strtok(NULL,",*");
         GPRMC.parameter[count] = tok;
         count++;
     }while(count<GPRMC.size);

     UTCTime = String(GPRMC.parameter[0]);
     Lat = String(GPRMC.parameter[2]);
     NSInd = String(GPRMC.parameter[3]);
     Long = String(GPRMC.parameter[4]);
     EWInd = String(GPRMC.parameter[5]);
}

String modelBEncondeSTM32::getStatus(void){
	return String(GPRMC.parameter[1]);
}
String modelBEncondeSTM32::getSpdOGnd(void){
	return String(GPRMC.parameter[6]);
}
String modelBEncondeSTM32::getCourOGnd(void){
	return String(GPRMC.parameter[7]);
}
String modelBEncondeSTM32::getDate(void){
	return String(GPRMC.parameter[8]);
}
String modelBEncondeSTM32::getMagnetVar(void){
	return String(GPRMC.parameter[9]);
}
String modelBEncondeSTM32::getMode(void){
	return String(GPRMC.parameter[10]);
}

/***********************************
   from OpenCR
 ***********************************/
/***********************************
   OCRC1
 ***********************************/
void modelBEncondeSTM32::OCRC1Thread(char *tok)
{
    memset(OCRC1.parameter,0,sizeof OCRC1.parameter);
    encode_header_ptr_esp = tok;
    uint8_t count=0;
    do
    {
        tok = strtok(NULL,",*");
        OCRC1.parameter[count] = tok;
        count++;
    }while(count<OCRC1.size);
}

int modelBEncondeSTM32::getPowerB(void){
	return OCRC1.parameter[0];
}
int modelBEncondeSTM32::getHornB(void){
	return OCRC1.parameter[1];
}
int modelBEncondeSTM32::getSpdUp(void){
	return OCRC1.parameter[2];
}
int modelBEncondeSTM32::getSpdDwn(void){
	return OCRC1.parameter[3];
}
int modelBEncondeSTM32::getRgbR(void){
	return OCRC1.parameter[4];
}
int modelBEncondeSTM32::getRgbG(void){
	return OCRC1.parameter[5];
}
int modelBEncondeSTM32::getRgbB(void){
	return OCRC1.parmeter[6]
}


/***********************************
   Checksum
 ***********************************/
bool modelBEncondeSTM32::checksum(char const* _buf)
{
  bool valid = false;
  uint8_t index;

  //find '*'
  for(uint8_t i=6; _buf[i] != '\n'; i++)
  {
    if(_buf[i] == '*')
    {
        index = i;
        break;
    }
  }

  if(index) //if index has value
  {
      byte ckA = 0;
      for (uint8_t i = 1; i < index; i++)
      {
        ckA ^= byte(_buf[i]);
      }

      //convert char to byte
      byte ckB = 0;
      if (_buf[index + 2] >= 65 && _buf[index + 2] <= 70)
      {
          ckB = _buf[index + 2] - 64 + 9;
      }
      else if (_buf[index + 2] >= 48 && _buf[index + 2] <= 57)
      {
        ckB = _buf[index + 2] - 48;
      }

      if (_buf[index + 1] >= 65 && _buf[index + 1] <= 70)
      {
        ckB = ckB + (_buf[index + 1] - 64 + 9) * 16;
      }
      else if (_buf[index + 1] >= 48 && _buf[index + 1] <= 57)
      {
        ckB = ckB + (_buf[index + 1] - 48) * 16;
      }

      //compare
      if (ckA - ckB == 0)
        valid = true;
  }

  return valid;
}

