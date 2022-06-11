// Definitions to set K197 info
// apply to a byte data[9] 
/*
 *          bit 7 ...................bit 0
 * r[0]: -,BAT,RCL,AC,dB, STO,REL,AUTO
 * r[1]: 6c,6b,6d,6g,6a,,6e,6f
 * r[2]: 5c,5b,5d,5g,5a,5dp,5e,5f
 * r[3]: 4c,4b,4d,4g,4a,4dp,4e,4f
 * r[4]: 3c,3b,3d,3g,3a,3dp,3e,3f
 * r[5]: 2c,2b,2d,2g,2a,2dp,2e,2f
 * r[6]: 1c,1b,1d,1g,1a,1dp,1e,1f
 * r[7]: ,,m(A),k,V,µ,M,m(V)
 * r[8]: ,,,RMT,,,A,?,C
 * 
 */
 
#ifndef __K197_SET_H
#define __K197_SET_H

#define MODES_BYTE  0
#define UNITS_BYTE  7
#define UNIT2_BYTE  8

#define MODE_BIT_NONE 0x0f
#define MODE_BIT_AUTO 0
#define MODE_BIT_REL  1
#define MODE_BIT_STO  2
#define MODE_BIT_dB   3
#define MODE_BIT_AC   4
#define MODE_BIT_RCL  5
#define MODE_BIT_BAT  6
#define MODE_BIT_MINUS  7

#define UNITS_BIT_NONE  0x0f
#define UNITS_BIT_mV    0
#define UNITS_BIT_M     1
#define UNITS_BIT_micro 2
#define UNITS_BIT_V     3
#define UNITS_BIT_K     4
#define UNITS_BIT_mA    5

#define UNIT2_BIT_NONE  0x0f
#define UNIT2_BIT_C      0
#define UNIT2_BIT_OHM    1    
#define UNIT2_BIT_A      2 
#define UNIT2_BIT_RMT    5     


/*
#define write_REL(r, b) ( bitWrite(r[MODES_BYTE],MODE_BIT_REL,b) )
#define write_dB(r, b)  ( bitWrite(r[MODES_BYTE],3,b) )
#define write_STO(r, b) ( bitWrite(r[MODES_BYTE],2,b) )
#define write_RCL(r, b) ( bitWrite(r[MODES_BYTE],5,b) )

#define toggle_REL(r) ( bitWrite(r[MODES_BYTE],MODE_BIT_REL,!bitRead( bitWrite(r[MODES_BYTE],MODE_BIT_REL))) )
#define toggle_dB(r)  ( bitWrite(r[MODES_BYTE],MODE_BIT_dB,!bitRead( bitWrite(r[MODES_BYTE],MODE_BIT_dB))) )
#define toggle_STO(r) ( bitWrite(r[MODES_BYTE],MODE_BIT_STO,!bitRead( bitWrite(r[MODES_BYTE],MODE_BIT_STO))) )
#define toggle_RCL(r) ( bitWrite(r[MODES_BYTE],MODE_BIT_RCL,!bitRead( bitWrite(r[MODES_BYTE],MODE_BIT_RCL))) )
*/

#define toggle_mode_bit(r, n) ( bitWrite(r[MODES_BYTE],n,!bitRead( r[MODES_BYTE],n)) )
#define toggle_units_bit(r, n) ( bitWrite(r[UNITS_BYTE],n,!bitRead( r[UNITS_BYTE],n)) )
#define toggle_unit2_bit(r, n) ( bitWrite(r[UNIT2_BYTE],n,!bitRead( r[UNIT2_BYTE],n)) )

short digit2index(short segment);

#endif //__K197_SET_H
