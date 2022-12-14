#include "example1.h"
#include "I2C.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t D[7] = {
	0b01111110,
	0b10000100,
	0b10000100,
	0b10000100,
	0b10000100,
	0b10000100,
	0b01111110
};

uint8_t M[7] = {
	0b11000011,
	0b10100101,
	0b10011001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001
};
uint8_t N[7] = {
	0b10000011,
	0b10000101,
	0b10001001,
	0b10011001,
	0b10010001,
	0b10100001,
	0b11000001
};

uint8_t U[7] = {
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b01111110
};

uint8_t L[7] = {
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b11111111
};
uint8_t O[7] = {
	0b01111110,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b01111110
};

uint8_t T[7] = {
	0b11111110,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000
};

uint8_t I[7] = {
	0b11111110,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b11111110
};

uint8_t E[7] = {
	0b01111111,
	0b00000001,
	0b00000001,
	0b00011111,
	0b00000001,
	0b00000001,
	0b01111111
};

uint8_t F[7] = {
	0b01111111,
	0b00000001,
	0b00000001,
	0b00011111,
	0b00000001,
	0b00000001,
	0b00000001
};

uint8_t S[7] = {
	0b11111110,
	0b00000001,
	0b00000001,
	0b01111110,
	0b10000000,
	0b10000000,
	0b01111111
};

uint8_t A[7] = {
	0b01111110,
	0b10000001,
	0b10000001,
	0b11111111,
	0b10000001,
	0b10000001,
	0b10000001
};

uint8_t R[7] = {
	0b01111111,
	0b10000001,
	0b10000001,
	0b01111111,
	0b00001101,
	0b00110001,
	0b11000001
};

uint8_t Light_on[11] = {
	0b10010010,
	0b10010010,
	0b10010010,
	0b01010100,
	0b01010100,
	0b00000000,
	0b11111111,
	0b01111110,
	0b00111100,
	0b00011000,
	0b00011000
};

uint8_t Light_off[8] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b11111111,
	0b01000010,
	0b00100100,
	0b00011000,
	0b00011000
};

int Num[10][10]={
	{0b00111100,
	0b01000010,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b01000010,
	0b00111100} ,
	
	{0b00100000,
	0b00110000,
	0b00101000,
	0b00100110,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b11111110} ,
	
	{0b00111100,
	0b01000010,
	0b10000001,
	0b01000001,
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b11111111} ,
	
	{0b11111111,
	0b10000000,
	0b01000000,
	0b00100000,
	0b00110000,
	0b01000000,
	0b10000001,
	0b10000001,
	0b01000010,
	0b00111100} ,
	
	{0b00100000,
	0b00110000,
	0b00101000,
	0b00100100,
	0b00100010,
	0b11111111,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000} ,
	
	{0b01111111,
	0b00000001,
	0b00110001,
	0b01001101,
	0b10000011,
	0b10000000,
	0b10000001,
	0b10000001,
	0b01000010,
	0b00111100} ,
	
	{0b00111000,
	0b01000100,
	0b10000010,
	0b00000001,
	0b00111001,
	0b01000101,
	0b10000011,
	0b10000001,
	0b01000010,
	0b00111100} ,
	
	{0b11111111,
	0b10000000,
	0b11000000,
	0b00110000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b00000010,
	0b00000010,
	0b00000010} ,
	
	{0b00111100,
	0b01000010,
	0b10000001,
	0b01000010,
	0b00111100,
	0b01000010,
	0b10000001,
	0b10000001,
	0b01000010,
	0b00111100},
	
    {0b00111100,
	0b01000010,
	0b10000001,
	0b11000001,
	0b10100010,
	0b10011100,
	0b10000000,
	0b10000001,
	0b01000010,
	0b00111100}
};


int Auto_page(bool Status) {
	Framebuffer fb;
	fb.clear();
	for(int i=0;i<10;++i){
		for(int j=-i;j<i+1;++j){
			fb.drawPixel(i,32+j);
		}
	}
	for(int i=0;i<10;++i){
		for(int j=-(10-i);j<(10-i)+1;++j){
			fb.drawPixel(i+118,32+j);
		}
	}

	
	for (int i = 0; i < sizeof(A); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((A[i] >> p) & 0x1) {
                fb.drawPixel(2*p+28  ,2*i+10  );
				fb.drawPixel(2*p+1+28,2*i+10  );
				fb.drawPixel(2*p+28  ,2*i+1+10);
				fb.drawPixel(2*p+1+28,2*i+1+10);
            }
        }
    }
	for (int i = 0; i < sizeof(U); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((U[i] >> p) & 0x1) {
                fb.drawPixel(2*p+46  ,2*i+10  );
				fb.drawPixel(2*p+1+46,2*i+10  );
				fb.drawPixel(2*p+46  ,2*i+1+10);
				fb.drawPixel(2*p+1+46,2*i+1+10);
            }
        }
    }
	for (int i = 0; i < sizeof(T); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((T[i] >> p) & 0x1) {
                fb.drawPixel(2*p+64  ,2*i+10  );
				fb.drawPixel(2*p+1+64,2*i+10  );
				fb.drawPixel(2*p+64  ,2*i+1+10);
				fb.drawPixel(2*p+1+64,2*i+1+10);
            }
        }
    }
	for (int i = 0; i < sizeof(O); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((O[i] >> p) & 0x1) {
                fb.drawPixel(2*p+82  ,2*i+10  );
				fb.drawPixel(2*p+1+82,2*i+10  );
				fb.drawPixel(2*p+82  ,2*i+1+10);
				fb.drawPixel(2*p+1+82,2*i+1+10);
            }
        }
    }
	for (int i = 0; i < sizeof(M); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((M[i] >> p) & 0x1) {
                fb.drawPixel(2*p+28  ,2*i+28  );
				fb.drawPixel(2*p+1+28,2*i+28  );
				fb.drawPixel(2*p+28  ,2*i+1+28);
				fb.drawPixel(2*p+1+28,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(O); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((O[i] >> p) & 0x1) {
                fb.drawPixel(2*p+46  ,2*i+28  );
				fb.drawPixel(2*p+1+46,2*i+28  );
				fb.drawPixel(2*p+46  ,2*i+1+28);
				fb.drawPixel(2*p+1+46,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(D); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((D[i] >> p) & 0x1) {
                fb.drawPixel(2*p+64  ,2*i+28  );
				fb.drawPixel(2*p+1+64,2*i+28  );
				fb.drawPixel(2*p+64  ,2*i+1+28);
				fb.drawPixel(2*p+1+64,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(E); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((E[i] >> p) & 0x1) {
                fb.drawPixel(2*p+82  ,2*i+28  );
				fb.drawPixel(2*p+1+82,2*i+28  );
				fb.drawPixel(2*p+82  ,2*i+1+28);
				fb.drawPixel(2*p+1+82,2*i+1+28);
            }
        }
    }
	if(Status){                                                       //To Indicate the ON OFF state of the mode
		for (int i = 0; i < sizeof(O); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((O[i] >> p) & 0x1) {
					fb.drawPixel(2*p+46  ,2*i+49  );
					fb.drawPixel(2*p+1+46,2*i+49  );
					fb.drawPixel(2*p+46  ,2*i+1+49);
					fb.drawPixel(2*p+1+46,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(N); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((N[i] >> p) & 0x1) {
					fb.drawPixel(2*p+64  ,2*i+49  );
					fb.drawPixel(2*p+1+64,2*i+49  );
					fb.drawPixel(2*p+64  ,2*i+1+49);
					fb.drawPixel(2*p+1+64,2*i+1+49);
				}
			}
		}
	}
	else{
		for (int i = 0; i < sizeof(O); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((O[i] >> p) & 0x1) {
					fb.drawPixel(2*p+37  ,2*i+49  );
					fb.drawPixel(2*p+1+37,2*i+49  );
					fb.drawPixel(2*p+37  ,2*i+1+49);
					fb.drawPixel(2*p+1+37,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(F); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((F[i] >> p) & 0x1) {
					fb.drawPixel(2*p+55  ,2*i+49  );
					fb.drawPixel(2*p+1+55,2*i+49  );
					fb.drawPixel(2*p+55  ,2*i+1+49);
					fb.drawPixel(2*p+1+55,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(F); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((F[i] >> p) & 0x1) {
					fb.drawPixel(2*p+73  ,2*i+49  );
					fb.drawPixel(2*p+1+73,2*i+49  );
					fb.drawPixel(2*p+73  ,2*i+1+49);
					fb.drawPixel(2*p+1+73,2*i+1+49);
				}
			}
		}
	}
	
	fb.show();
};

int Manual_page(bool Status) {
	Framebuffer fb;
	fb.clear();
	for(int i=0;i<10;++i){
		for(int j=-i;j<i+1;++j){
			fb.drawPixel(i,32+j);
		}
	}
	for(int i=0;i<10;++i){
		for(int j=-(10-i);j<(10-i)+1;++j){
			fb.drawPixel(i+118,32+j);
		}
	}
	
	for (int i = 0; i < sizeof(M); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((M[i] >> p) & 0x1) {
                fb.drawPixel(2*p+10  ,2*i+7  );
				fb.drawPixel(2*p+1+10,2*i+7  );
				fb.drawPixel(2*p+10  ,2*i+1+7);
				fb.drawPixel(2*p+1+10,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(A); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((A[i] >> p) & 0x1) {
                fb.drawPixel(2*p+28  ,2*i+7  );
				fb.drawPixel(2*p+1+28,2*i+7  );
				fb.drawPixel(2*p+28  ,2*i+1+7);
				fb.drawPixel(2*p+1+28,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(N); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((N[i] >> p) & 0x1) {
                fb.drawPixel(2*p+46  ,2*i+7  );
				fb.drawPixel(2*p+1+46,2*i+7  );
				fb.drawPixel(2*p+46  ,2*i+1+7);
				fb.drawPixel(2*p+1+46,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(U); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((U[i] >> p) & 0x1) {
                fb.drawPixel(2*p+64  ,2*i+7  );
				fb.drawPixel(2*p+1+64,2*i+7  );
				fb.drawPixel(2*p+64  ,2*i+1+7);
				fb.drawPixel(2*p+1+64,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(A); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((A[i] >> p) & 0x1) {
                fb.drawPixel(2*p+82  ,2*i+7  );
				fb.drawPixel(2*p+1+82,2*i+7  );
				fb.drawPixel(2*p+82  ,2*i+1+7);
				fb.drawPixel(2*p+1+82,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(L); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((L[i] >> p) & 0x1) {
                fb.drawPixel(2*p+100  ,2*i+7  );
				fb.drawPixel(2*p+1+100,2*i+7  );
				fb.drawPixel(2*p+100  ,2*i+1+7);
				fb.drawPixel(2*p+1+100,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(M); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((M[i] >> p) & 0x1) {
                fb.drawPixel(2*p+28  ,2*i+28  );
				fb.drawPixel(2*p+1+28,2*i+28  );
				fb.drawPixel(2*p+28  ,2*i+1+28);
				fb.drawPixel(2*p+1+28,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(O); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((O[i] >> p) & 0x1) {
                fb.drawPixel(2*p+46  ,2*i+28  );
				fb.drawPixel(2*p+1+46,2*i+28  );
				fb.drawPixel(2*p+46  ,2*i+1+28);
				fb.drawPixel(2*p+1+46,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(D); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((D[i] >> p) & 0x1) {
                fb.drawPixel(2*p+64  ,2*i+28  );
				fb.drawPixel(2*p+1+64,2*i+28  );
				fb.drawPixel(2*p+64  ,2*i+1+28);
				fb.drawPixel(2*p+1+64,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(E); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((E[i] >> p) & 0x1) {
                fb.drawPixel(2*p+82  ,2*i+28  );
				fb.drawPixel(2*p+1+82,2*i+28  );
				fb.drawPixel(2*p+82  ,2*i+1+28);
				fb.drawPixel(2*p+1+82,2*i+1+28);
            }
        }
    }
	if(Status){                                                       //To Indicate the ON OFF state of the mode
		for (int i = 0; i < sizeof(O); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((O[i] >> p) & 0x1) {
					fb.drawPixel(2*p+46  ,2*i+49  );
					fb.drawPixel(2*p+1+46,2*i+49  );
					fb.drawPixel(2*p+46  ,2*i+1+49);
					fb.drawPixel(2*p+1+46,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(N); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((N[i] >> p) & 0x1) {
					fb.drawPixel(2*p+64  ,2*i+49  );
					fb.drawPixel(2*p+1+64,2*i+49  );
					fb.drawPixel(2*p+64  ,2*i+1+49);
					fb.drawPixel(2*p+1+64,2*i+1+49);
				}
			}
		}
	}
	else{
		for (int i = 0; i < sizeof(O); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((O[i] >> p) & 0x1) {
					fb.drawPixel(2*p+37  ,2*i+49  );
					fb.drawPixel(2*p+1+37,2*i+49  );
					fb.drawPixel(2*p+37  ,2*i+1+49);
					fb.drawPixel(2*p+1+37,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(F); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((F[i] >> p) & 0x1) {
					fb.drawPixel(2*p+55  ,2*i+49  );
					fb.drawPixel(2*p+1+55,2*i+49  );
					fb.drawPixel(2*p+55  ,2*i+1+49);
					fb.drawPixel(2*p+1+55,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(F); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((F[i] >> p) & 0x1) {
					fb.drawPixel(2*p+73  ,2*i+49  );
					fb.drawPixel(2*p+1+73,2*i+49  );
					fb.drawPixel(2*p+73  ,2*i+1+49);
					fb.drawPixel(2*p+1+73,2*i+1+49);
				}
			}
		}
	}
	fb.show();
};

int Timer_page(bool Status) {                                 //Timer Mode page
	Framebuffer fb;
	fb.clear();
	for(int i=0;i<10;++i){
		for(int j=-i;j<i+1;++j){
			fb.drawPixel(i,32+j);
		}
	}
	for(int i=0;i<10;++i){
		for(int j=-(10-i);j<(10-i)+1;++j){
			fb.drawPixel(i+118,32+j);
		}
	}
	for (int i = 0; i < sizeof(T); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((T[i] >> p) & 0x1) {
                fb.drawPixel(2*p+20  ,2*i+7  );
				fb.drawPixel(2*p+1+20,2*i+7  );
				fb.drawPixel(2*p+20  ,2*i+1+7);
				fb.drawPixel(2*p+1+20,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(I); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((I[i] >> p) & 0x1) {
                fb.drawPixel(2*p+38  ,2*i+7  );
				fb.drawPixel(2*p+1+38,2*i+7  );
				fb.drawPixel(2*p+38  ,2*i+1+7);
				fb.drawPixel(2*p+1+38,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(M); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((M[i] >> p) & 0x1) {
                fb.drawPixel(2*p+56  ,2*i+7  );
				fb.drawPixel(2*p+1+56,2*i+7  );
				fb.drawPixel(2*p+56  ,2*i+1+7);
				fb.drawPixel(2*p+1+56,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(E); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((E[i] >> p) & 0x1) {
                fb.drawPixel(2*p+74  ,2*i+7  );
				fb.drawPixel(2*p+1+74,2*i+7  );
				fb.drawPixel(2*p+74  ,2*i+1+7);
				fb.drawPixel(2*p+1+74,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(R); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((R[i] >> p) & 0x1) {
                fb.drawPixel(2*p+92  ,2*i+7  );
				fb.drawPixel(2*p+1+92,2*i+7  );
				fb.drawPixel(2*p+92  ,2*i+1+7);
				fb.drawPixel(2*p+1+92,2*i+1+7);
            }
        }
    }
	for (int i = 0; i < sizeof(M); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((M[i] >> p) & 0x1) {
                fb.drawPixel(2*p+28  ,2*i+28  );
				fb.drawPixel(2*p+1+28,2*i+28  );
				fb.drawPixel(2*p+28  ,2*i+1+28);
				fb.drawPixel(2*p+1+28,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(O); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((O[i] >> p) & 0x1) {
                fb.drawPixel(2*p+46  ,2*i+28  );
				fb.drawPixel(2*p+1+46,2*i+28  );
				fb.drawPixel(2*p+46  ,2*i+1+28);
				fb.drawPixel(2*p+1+46,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(D); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((D[i] >> p) & 0x1) {
                fb.drawPixel(2*p+64  ,2*i+28  );
				fb.drawPixel(2*p+1+64,2*i+28  );
				fb.drawPixel(2*p+64  ,2*i+1+28);
				fb.drawPixel(2*p+1+64,2*i+1+28);
            }
        }
    }
	for (int i = 0; i < sizeof(E); ++i) {
        for (int p = 0; p < 8; ++p) {
            if ((E[i] >> p) & 0x1) {
                fb.drawPixel(2*p+82  ,2*i+28  );
				fb.drawPixel(2*p+1+82,2*i+28  );
				fb.drawPixel(2*p+82  ,2*i+1+28);
				fb.drawPixel(2*p+1+82,2*i+1+28);
            }
        }
    }
	if(Status){                                                       //To Indicate the ON OFF state of the mode
		for (int i = 0; i < sizeof(O); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((O[i] >> p) & 0x1) {
					fb.drawPixel(2*p+46  ,2*i+49  );
					fb.drawPixel(2*p+1+46,2*i+49  );
					fb.drawPixel(2*p+46  ,2*i+1+49);
					fb.drawPixel(2*p+1+46,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(N); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((N[i] >> p) & 0x1) {
					fb.drawPixel(2*p+64  ,2*i+49  );
					fb.drawPixel(2*p+1+64,2*i+49  );
					fb.drawPixel(2*p+64  ,2*i+1+49);
					fb.drawPixel(2*p+1+64,2*i+1+49);
				}
			}
		}
	}
	else{
		for (int i = 0; i < sizeof(O); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((O[i] >> p) & 0x1) {
					fb.drawPixel(2*p+37  ,2*i+49  );
					fb.drawPixel(2*p+1+37,2*i+49  );
					fb.drawPixel(2*p+37  ,2*i+1+49);
					fb.drawPixel(2*p+1+37,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(F); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((F[i] >> p) & 0x1) {
					fb.drawPixel(2*p+55  ,2*i+49  );
					fb.drawPixel(2*p+1+55,2*i+49  );
					fb.drawPixel(2*p+55  ,2*i+1+49);
					fb.drawPixel(2*p+1+55,2*i+1+49);
				}
			}
		}
		for (int i = 0; i < sizeof(F); ++i) {
			for (int p = 0; p < 8; ++p) {
				if ((F[i] >> p) & 0x1) {
					fb.drawPixel(2*p+73  ,2*i+49  );
					fb.drawPixel(2*p+1+73,2*i+49  );
					fb.drawPixel(2*p+73  ,2*i+1+49);
					fb.drawPixel(2*p+1+73,2*i+1+49);
				}
			}
		}
	}
	fb.show();
};

int Display_numbers(int Pos_1,int Pos_2,int Pos_3,int Pos_4,int Cur){     //Function to display the Numbers in timer mode
	Framebuffer fb;
	fb.clear();
	
	for(int i = 0;i<5;++i){                                //These are for the collon marks between the times.
		for(int j = 0;j<5;++j){
			fb.drawPixel(j+80 ,i+12 );
		}
	}
	
	for(int i = 0;i<5;++i){
		for(int j = 0;j<5;++j){
			fb.drawPixel(j+80 ,i+25 );
		}
	}
	
	for(int i = 0;i<5;++i){
		for(int j = 0;j<5;++j){
			fb.drawPixel(j+80 ,i+42 );
		}
	}
	
	for(int i = 0;i<5;++i){
		for(int j = 0;j<5;++j){
			fb.drawPixel(j+80 ,i+55 );
		}
	}
	
	                                                   //Curser drowing codes
	if(Cur ==1){
		for(int j = 0; j<30; ++j){
			fb.drawPixel(j+43,33);
		}
	}
	else if(Cur ==2){
		for(int j = 0; j<30; ++j){
			fb.drawPixel(j+93,33);
		}
	}
	else if(Cur ==3){
		for(int j = 0; j<30; ++j){
			fb.drawPixel(j+43,63);
		}
	}
	else if(Cur ==4){
		for(int j = 0; j<30; ++j){
			fb.drawPixel(j+93,63);
		}
	}
	
	for (int i = 0; i < sizeof(Light_on); ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Light_on[i] >> p) & 0x1) {
				fb.drawPixel(2*p+12  ,2*i+8  );
				fb.drawPixel(2*p+1+12,2*i+8  );
				fb.drawPixel(2*p+12  ,2*i+1+8);
				fb.drawPixel(2*p+1+12,2*i+1+8);
			}
		}
	}
	
	for (int i = 0; i < sizeof(Light_off); ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Light_off[i] >> p) & 0x1) {
				fb.drawPixel(2*p+12  ,2*i+42  );
				fb.drawPixel(2*p+1+12,2*i+42  );
				fb.drawPixel(2*p+12  ,2*i+1+42);
				fb.drawPixel(2*p+1+12,2*i+1+42);
			}
		}
	}
	
	int Val_1;
	int Val_2;
	
	if(Pos_1<10){
		Val_1 = Pos_1%10;
		Val_2 = 0;
	}
	else{
		Val_1 = Pos_1%10;
		Val_2 = ((Pos_1-Val_1)/10)%10;
	}
	for (int i = 0; i < sizeof(Num[Val_2])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_2][i] >> p) & 0x1) {
				fb.drawPixel(2*p+40  ,2*i+10  );
				fb.drawPixel(2*p+1+40,2*i+10  );
				fb.drawPixel(2*p+40  ,2*i+1+10);
				fb.drawPixel(2*p+1+40,2*i+1+10);
			}
		}
	}
	for (int i = 0; i < sizeof(Num[Val_1])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_1][i] >> p) & 0x1) {
				fb.drawPixel(2*p+58  ,2*i+10  );
				fb.drawPixel(2*p+1+58,2*i+10  );
				fb.drawPixel(2*p+58  ,2*i+1+10);
				fb.drawPixel(2*p+1+58,2*i+1+10);
			}
		}
	}
	
	if(Pos_2<10){
		Val_1 = Pos_2%10;
		Val_2 = 0;
	}
	else{
		Val_1 = Pos_2%10;
		Val_2 = ((Pos_2-Val_1)/10)%10;
	}

	for (int i = 0; i < sizeof(Num[Val_2])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_2][i] >> p) & 0x1) {
				fb.drawPixel(2*p+90  ,2*i+10  );
				fb.drawPixel(2*p+1+90,2*i+10  );
				fb.drawPixel(2*p+90  ,2*i+1+10);
				fb.drawPixel(2*p+1+90,2*i+1+10);
			}
		}
	}
	for (int i = 0; i < sizeof(Num[Val_1])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_1][i] >> p) & 0x1) {
				fb.drawPixel(2*p+108  ,2*i+10  );
				fb.drawPixel(2*p+1+108,2*i+10  );
				fb.drawPixel(2*p+108  ,2*i+1+10);
				fb.drawPixel(2*p+1+108,2*i+1+10);
			}
		}
	}
	
	if(Pos_3<10){
		Val_1 = Pos_3%10;
		Val_2 = 0;
	}
	else{
		Val_1 = Pos_3%10;
		Val_2 = ((Pos_3-Val_1)/10)%10;
	}
	for (int i = 0; i < sizeof(Num[Val_2])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_2][i] >> p) & 0x1) {
				fb.drawPixel(2*p+40  ,2*i+40  );
				fb.drawPixel(2*p+1+40,2*i+40  );
				fb.drawPixel(2*p+40  ,2*i+1+40);
				fb.drawPixel(2*p+1+40,2*i+1+40);
			}
		}
	}
	for (int i = 0; i < sizeof(Num[Val_1])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_1][i] >> p) & 0x1) {
				fb.drawPixel(2*p+58  ,2*i+40  );
				fb.drawPixel(2*p+1+58,2*i+40  );
				fb.drawPixel(2*p+58  ,2*i+1+40);
				fb.drawPixel(2*p+1+58,2*i+1+40);
			}
		}
	}
	
	if(Pos_4<10){
		Val_1 = Pos_4%10;
		Val_2 = 0;
	}
	else{
		Val_1 = Pos_4%10;
		Val_2 = ((Pos_4-Val_1)/10)%10;
	}

	for (int i = 0; i < sizeof(Num[Val_2])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_2][i] >> p) & 0x1) {
				fb.drawPixel(2*p+90  ,2*i+40  );
				fb.drawPixel(2*p+1+90,2*i+40  );
				fb.drawPixel(2*p+90  ,2*i+1+40);
				fb.drawPixel(2*p+1+90,2*i+1+40);
			}
		}
	}
	for (int i = 0; i < sizeof(Num[Val_1])/2; ++i) {
		for (int p = 0; p < 8; ++p) {
			if ((Num[Val_1][i] >> p) & 0x1) {
				fb.drawPixel(2*p+108  ,2*i+40  );
				fb.drawPixel(2*p+1+108,2*i+40  );
				fb.drawPixel(2*p+108  ,2*i+1+40);
				fb.drawPixel(2*p+1+108,2*i+1+40);
			}
		}
	}
	fb.show();
}
//.............................................Variables......................
volatile int Page_number=0;

int Button_next = 7;                                    //button pins
int Button_Set_timer = 5;
int Button_Select = 6;

int Button_next_state = 0;                        //Button status
int Button_Set_timer_state = 0;
int Button_Select_state = 0;

bool button_state = 0;                          //debouncing function variables
int Pre_Button_value = 0;

volatile bool Auto_mode = true;                         //To select the modes
volatile bool Timer_mode = false;

volatile bool Data_changed = false;

volatile int Start_time_min = 0;                                 //To store the Start and stop times of the timer mode
volatile int Start_time_hour = 0;
volatile int Stop_time_min = 0;
volatile int Stop_time_hour = 0;

volatile bool Page_changed = true;                        //Display the OLED


int digitalRead(int Pin){                         //Pin read function
	if(PIND & (1<<Pin)){return 1;}
	else{return 0;}
}

int Button(int pin){                              //Debouncing function
	bool state = digitalRead(pin);
	if( Pre_Button_value == 0 & state ==1){
		Pre_Button_value =1;
		while(state ==1){
			state = digitalRead(pin);
			_delay_ms(10);
		}
		return 1;
	}
	else{
		Pre_Button_value =0;
		return 0;
	}
}

//...................................................................EEPROM...................................................................
#define Auto_mode_address 0b0000001010
#define Timer_mode_address 0b0000001011

#define Start_hour_address 0b0000001100
#define Start_min_address 0b0000001101
#define Stop_hour_address 0b0000001110
#define Stop_min_address 0b0000001111


void EEPROM_write(unsigned int uiAddress, unsigned char ucData){

	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress){
	
	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EECR |= (1<<EERE);
	return EEDR;
}

void Save_data(){
	EEPROM_write(Auto_mode_address, Auto_mode);
	EEPROM_write(Timer_mode_address, Timer_mode);
	
	EEPROM_write(Start_hour_address, Start_time_hour);
	EEPROM_write(Start_min_address, Start_time_min);
	EEPROM_write(Stop_hour_address, Stop_time_hour);
	EEPROM_write(Stop_min_address, Stop_time_min);
}

void Read_data(){
	Auto_mode = EEPROM_read(Auto_mode_address);
	Timer_mode = EEPROM_read(Timer_mode_address);
	
	Start_time_hour = EEPROM_read(Start_hour_address);
	Start_time_min = EEPROM_read(Start_min_address);
	Stop_time_hour = EEPROM_read(Stop_hour_address);
	Stop_time_min = EEPROM_read(Stop_min_address);
}

//...........................................Serial(UART).....................................

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define RX_BUFFER_SIZE	128
#define TX_BUFFER_SIZE	128

char rxBuffer[RX_BUFFER_SIZE];
char serialBuffer[TX_BUFFER_SIZE];

uint8_t rxReadPos = 0;
uint8_t rxWritePos = 0;

uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;

void USART_Init( unsigned int ubrr)
{
    UBRR0H = (ubrr >> 8);
	UBRR0L =  ubrr;
	UCSR0B = (1 << RXEN0)  | (1 << RXCIE0)|(1 << TXEN0)  | (1 << TXCIE0);
	UCSR0C = (1 << UCSZ01) | (3 << UCSZ00);
}

char peekChar(void){
	char ret = '\0';
	if(rxReadPos != rxWritePos){
		ret = rxBuffer[rxReadPos];
	}
	return ret;
}

char getChar(void){
	char ret = '\0';
	if(rxReadPos != rxWritePos){
		ret = rxBuffer[rxReadPos];
		rxReadPos++;
		if(rxReadPos >= RX_BUFFER_SIZE){
			rxReadPos = 0;
		}
	}
	return ret;
}

ISR(USART_RX_vect){
	rxBuffer[rxWritePos] = UDR0;
	rxWritePos++;
	if(rxWritePos >= RX_BUFFER_SIZE){
		rxWritePos = 0;
	}
}

void appendSerial(char c){
	serialBuffer[serialWritePos] = c;
	serialWritePos++;
	if(serialWritePos >= TX_BUFFER_SIZE){
		serialWritePos = 0;
	}
}

void serialWrite(char c[]){
	for(uint8_t i = 0; c[i]!='\0'; i++){
		appendSerial(c[i]);
	}
	if(UCSR0A & (1 << UDRE0)){
		UDR0 = 0;
	}
}

ISR(USART_TX_vect){
	if(serialReadPos != serialWritePos){
		UDR0 = serialBuffer[serialReadPos];
		serialReadPos++;
		if(serialReadPos >= TX_BUFFER_SIZE){
			serialReadPos++;
		}
	}
}

//...................................................................Interupt...................................

int Pre_Val;
volatile bool ON = false;
volatile bool Exit = 0;
I2C i2c;

ISR (INT0_vect){
	ON = 1-ON;
	Framebuffer fb;
	Save_data();
	fb.clear();
	fb.show();
	if(ON){
		Page_changed = true;
		Page_number = 0;
		Exit = 1;
		i2c.start();
		i2c.stop();
		Read_data();
	}
	else{
		if(Data_changed){
			if(Auto_mode){serialWrite("1");}  //Transmit auto mode
			else{serialWrite("0");}
			
			if(Timer_mode){serialWrite("1");} //Transmit Timer mode
			else{serialWrite("0");}
			
			char Temp[1];
			
			if(Start_time_hour<10){
				itoa (Start_time_hour,Temp,10);
				serialWrite("0");
				serialWrite(Temp);
			}
			else{
				itoa (Start_time_hour,Temp,10);
				serialWrite(Temp);
			}
			
			if(Start_time_min<10){
				itoa (Start_time_min,Temp,10);
				serialWrite("0");
				serialWrite(Temp);
			}
			else{
				itoa (Start_time_min,Temp,10);
				serialWrite(Temp);
			}
			
			if(Stop_time_hour<10){
				itoa (Stop_time_hour,Temp,10);
				serialWrite("0");
				serialWrite(Temp);
			}
			else{
				itoa (Stop_time_hour,Temp,10);
				serialWrite(Temp);
			}
			
			if(Stop_time_min<10){
				itoa (Stop_time_min,Temp,10);
				serialWrite("0");
				serialWrite(Temp);
			}
			else{
				itoa (Stop_time_min,Temp,10);
				serialWrite(Temp);
			}
		}
		Data_changed = false;
		//SMCR = (0<<3)|(1<<2)|(1<<1)|(1<<0);    //Enter the sleep mode
	}
}

//...............................................................................................................
int main(void){
	DDRD |= (0<<Button_Select)|(0<<Button_Set_timer)|(0<<Button_next);
	DDRD = (0<<2);
	
	EICRA |= (1<<0)|(1<<1);
	EIMSK = (1<<0);
	USART_Init(MYUBRR);
	sei();
	Read_data();
	while(1){
		while(ON){
			Button_next_state = Button(Button_next);
			Button_Set_timer_state = Button(Button_Set_timer);
			Button_Select_state = Button(Button_Select);
			
			if(Button_next_state & Button_Set_timer_state){
				_delay_ms(1);
			}
			else if(Button_next_state){             //Increment the page number
				if(Page_number==2){
					Page_number = 0;
				}
				else{
					Page_number+=1;
				}
				Page_changed = true;
			}
			
			if(Button_Set_timer_state){              //Enter to Set timer Display
				int Curser = 1;
				Page_changed = true;
				Exit = 0;
				while(!Exit){
					if(Page_changed){
						Display_numbers(Start_time_hour,Start_time_min,Stop_time_hour,Stop_time_min,Curser);
						Page_changed = false;
					}
					
					Button_next_state = Button(Button_next);
					Button_Set_timer_state = Button(Button_Set_timer);
					Button_Select_state = Button(Button_Select);
					if(Button_Set_timer_state){
						Exit =1;
						Page_changed = true;
					}
					if(Button_Select_state){                  //select the curser place
						if(Curser==4){
							Exit =1;
							Page_changed = true;
							break;
						}
						else{
							Curser+=1;
						}
						Page_changed = true;
					}
					if(Button_next_state){
						if(Curser ==1){ Start_time_hour+=1; }
						else if (Curser ==2){ Start_time_min+=1; }
						else if (Curser ==3){ Stop_time_hour+=1; }
						else if (Curser ==4){ Stop_time_min+=1; }
						Page_changed = true;
						Data_changed = true;
					}
					if(Start_time_hour > 23){ Start_time_hour = 0;}
					if(Start_time_min > 59){ Start_time_min = 0;}
					if(Stop_time_hour > 23){ Stop_time_hour = 0;}
					if(Stop_time_min > 59){ Stop_time_min = 0;}
				}
				Button_Select_state = 0;
			}
			if(Page_number == 0){
				if(Button_Select_state){
					Auto_mode = 1- Auto_mode;
					Page_changed = true;
					Data_changed = true;
				}
				if(Page_changed){
					Auto_page(Auto_mode);
				}
			}
			else if(Page_number == 1){
				if(Button_Select_state){
					Auto_mode = 1- Auto_mode;
					Page_changed = true;
					Data_changed = true;
				}
				if(Page_changed){
					Manual_page(1-Auto_mode);
				}
			}
			else if(Page_number == 2){
				if(Button_Select_state){
					Timer_mode = 1- Timer_mode;
					Page_changed = true;
					Data_changed = true;
				}
				if(Page_changed){
					Timer_page(Timer_mode);
				}
			}
			if(Page_changed){
				Save_data();
			}
			Page_changed = false;
		}
	}
}