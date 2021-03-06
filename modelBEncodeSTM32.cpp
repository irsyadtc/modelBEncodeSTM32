#include "modelBEncodeSTM32.h"

modelBEncodeSTM32::modelBEncodeSTM32()
{
    Lat = "0";
    NSInd = "0";
    Long = "0";
    EWInd = "0";
}


/**************************************
   Encode
 **************************************/
bool modelBEncodeSTM32::encodeGPS(char const* _buf)
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

            if(strcmp(tok,"GPGLL") == 0)
            {
            	GPGLLThread(tok);
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
String modelBEncodeSTM32::getHeader(void)
{
    return String(encode_header_ptr_stm);
}


/***********************************
   from GPS
 ***********************************/


/***********************************
   GPGLL
 ***********************************/
 void modelBEncodeSTM32::GPGLLThread(char *tok)
{
    memset(GPGLL.parameter,0,sizeof GPGLL.parameter);
    //point to header
    encode_header_ptr_stm = tok;
    //capture parameter
    uint8_t count=0;
    do
    {
        tok = strtok(NULL,",*");

        uint8_t i = 0;
        while(tok[i] != 0)
        {
            GPGLL.parameter[count][i] = tok[i];
            i++;
        }
        GPGLL.parameter[count][i] = '\0';
        count++;
    }while(count<GPGLL.size);

    //verify & store value
    Lat = verifyLat(GPGLL.parameter[0]);
    NSInd = String(GPGLL.parameter[1]);
    Long = verifyLong(GPGLL.parameter[2]);
    EWInd = String(GPGLL.parameter[3]);
    UTCTime = String(GPGLL.parameter[4]);
    Status = String(GPGLL.parameter[5]);

}

String modelBEncodeSTM32::getLat(void)
{
	return Lat;
}
String modelBEncodeSTM32::getNSInd(void)
{
	return NSInd;
}
String modelBEncodeSTM32::getLong(void)
{
	return Long;
}
String modelBEncodeSTM32::getEWInd(void)
{
	return EWInd;
}
String modelBEncodeSTM32::getUtcTime(void)
{
	return UTCTime;
}
String modelBEncodeSTM32::getStatus(void){
	return Status;
}



/***********************************
   for verify
 ***********************************/
 uint8_t modelBEncodeSTM32::getParameterSize(char *tok)
 {
 	uint8_t size_ = 0;
  for(uint8_t i=0; i<150; i++)
  {
    if(tok[i]==0)
    {
      size_ = i;
      break;
    }
  }
  return size_;
 }

String modelBEncodeSTM32::verifyLat(char *tok)
 {
 	float lat_ = String(tok).toFloat();

 	if(lat_ != 0)
    {
        //valid
        return String(tok);
    }
    else
        {
        return Lat;
    }
 }


 String modelBEncodeSTM32::verifyLong(char *tok)
 {
 	float lat_ = String(tok).toFloat();

 	if(lat_ != 0)
    {
        //valid
        return String(tok);
    }
    else
        {
        return Long;
    }
 }
/***********************************
   from OpenCR
 ***********************************/

 /**************************************
   Encode
 **************************************/
bool modelBEncodeSTM32::encodeOCR(char const* _buf)
{
    bool valid = false;
    char *sntc= (char*)_buf;

    if(sntc)
    {
        //capture full sentence
        char *tok = strtok(sntc,"\r\n");

        		if(strcmp(tok,"reset") == 0)
            {
            	resetThread(tok);
            }

    }
    return valid;

}

/***********************************
   OCRC1
 ***********************************/
void modelBEncodeSTM32::OCRC1Thread(char *tok)
{
    memset(OCRC1.parameter,0,sizeof OCRC1.parameter);
    encode_header_ptr_stm = tok;
    uint8_t count=0;
    do
    {
        tok = strtok(NULL,",*");

        uint8_t i = 0;
        while(tok[i] != 0)
        {
            OCRC1.parameter[count][i] = tok[i];
            i++;
        }
        OCRC1.parameter[count][i] = '\0';
        count++;
    }while(count<OCRC1.size);
}

int modelBEncodeSTM32::getPowerB(void){
	return atoi(OCRC1.parameter[0]);
}
int modelBEncodeSTM32::getHornB(void){
	return atoi(OCRC1.parameter[1]);
}
int modelBEncodeSTM32::getSpdUp(void){
	return atoi(OCRC1.parameter[2]);
}
int modelBEncodeSTM32::getSpdDwn(void){
	return atoi(OCRC1.parameter[3]);
}
int modelBEncodeSTM32::getRgbR(void){
	return atoi(OCRC1.parameter[4]);
}
int modelBEncodeSTM32::getRgbG(void){
	return atoi(OCRC1.parameter[5]);
}
int modelBEncodeSTM32::getRgbB(void){
	return atoi(OCRC1.parameter[6]);
}

/***********************************
   reset
 ***********************************/
void modelBEncodeSTM32::resetThread(char *tok)
{
    encode_header_ptr_stm = tok;
}


/***********************************
   Checksum
 ***********************************/
bool modelBEncodeSTM32::checksum(char const* _buf)
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


