#ifndef modelBEncondeSTM32_h
#define modelBEncondeSTM32_h

#include "Arduino.h"

class modelBEncondeSTM32
{
  public:
    modelBEncondeSTM32();
    bool encode(char const* b);
    String getHeader(void);
    uint8_t parse(char const* b, uint8_t sz);

    //from gps ////////////////////
    // This library requires a different approach from
    // the modelAEncode libraries due to redundant parameters from different message ID
    void GPGGAThread(char *tok);
    void GPRMCThread(char *tok);

    //GPGGA
    String getUtcTime(void);	//r
    String getLat(void);		//r
    String getNSInd(void);		//r
    String getLong(void);		//r
    String getEWInd(void);		//r
    int getPosFix(void);
    int getSatellites(void);
    String getHDOP(void);
    String getMSLAlt(void);
    String getUnitMSL(void);
    String getGeoidSep(void);
    String getUnitGeoid(void);
    String getAgeDiffCorr(void);
    String getDiffRefSta(void);

    //GPRMC
	String getStatus(void);
	String getSpdOGnd(void);
	String getCourOGnd(void);
	String getDate(void);
	String getMagnetVar(void);
	String getMode(void);

    //from OpenCR
	void OCRC1Thread(char *tok);
	int getPowerB(void);
	int getHornB(void);
	int getSpdUp(void);
	int getSpdDwn(void);
	int getRgbR(void);
	int getRgbG(void);
	int getRgbB(void);

    bool checksum(char const* _buf);

  private:
    char *encode_header_ptr_stm;
    String UTCTime;
    String Lat;
    String NSInd;
    String Long;
    String EWInd;

    //from gps //////////////////
    struct gpgga
    {
      uint8_t size = 14;
      char *parameter[14];
    };

    struct gprmc
    {
      uint8_t size = 11;
      char *parameter[11];
    };

    //from openCR /////////////////////
    struct ocrc1
    {
        uint8_t size = 7;
        char *parameter[7];
    };

    gpgga GPGGA;
    gprmc GPRMC;
    ocrc1 OCRC1;

};

#endif