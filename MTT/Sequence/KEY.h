#ifndef KEY_H
#define KEY_H

enum KEY { _coarse_left,_coarse_right,
           _fine_left,_fine_right,_enter,
            _TEST,  _STOP,
            _NOP
         };

KEY GetKey(int fd);
#endif // KEY_H
