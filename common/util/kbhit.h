/**
 * 	Linux (POSIX) implementation of _kbhit()
 * 	William Cheung 08/15/2015
 */

#ifndef __KB_HIT_H__
#define __KB_HIT_H__

#ifdef  __cplusplus
extern "C" {
#endif

/**  exported global functions  **/
extern void _chmod	(int mode);     // mode = 1 : starting kbhit mode, mode = 0 : ending kbhit mode    
extern int  _rdmod	(void);         // this can be used for testing current mode; returns 0 or 1
extern int  _kbhit	(void);         // api for tesing if there exists a pressed key; returns 0 or 1 
extern int  _getch	(void);         // get the pressed key

#ifdef  __cplusplus
}
#endif

#endif // kbhit.h
