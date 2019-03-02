#include <stdio.h>
#include "Consumer.h"

#define KEY1_PINNO    (3)   // 3pin(PD0)
#define KEY2_PINNO    (4)   // 4pin(PD4)
#define KEY3_PINNO    (6)   // 6pin(PD7)
#define KEY4_PINNO    (8)   // 8pin(PB4)
#define KEY5_PINNO    (9)   // 9pin(PB5)
#define KEY6_PINNO    (10)	// 10pin(PB6)
#define KEY7_PINNO    (16)	// 16pin(PB2)
#define KEY8_PINNO    (14)  // 14pin(PB3)
#define KEY9_PINNO    (15)  // 15pin(PB1)

#define KEY1_PORTNO    (0)	// 3pin(PD0)
#define KEY2_PORTNO    (4)  // 4pin(PD4)
#define KEY3_PORTNO    (7)  // 6pin(PD7)
#define KEY4_PORTNO    (4)  // 8pin(PB4)
#define KEY5_PORTNO    (5)  // 9pin(PB5)
#define KEY6_PORTNO    (6)  // 10pin(PB6)
#define KEY7_PORTNO    (2)  // 16pin(PB2)
#define KEY8_PORTNO    (3)  // 14pin(PB3)
#define KEY9_PORTNO    (1)  // 15pin(PB1)

#define KEY1_PORTBIT    (1<<KEY1_PORTNO)
#define KEY2_PORTBIT    (1<<KEY2_PORTNO)
#define KEY3_PORTBIT    (1<<KEY3_PORTNO)
#define KEY4_PORTBIT    (1<<KEY4_PORTNO)
#define KEY5_PORTBIT    (1<<KEY5_PORTNO)
#define KEY6_PORTBIT    (1<<KEY6_PORTNO)
#define KEY7_PORTBIT    (1<<KEY7_PORTNO)
#define KEY8_PORTBIT    (1<<KEY8_PORTNO)
#define KEY9_PORTBIT    (1<<KEY9_PORTNO)

#define KEY1_BIT    (KEY1_PORTBIT << 8)
#define KEY2_BIT    (KEY2_PORTBIT << 8)
#define KEY3_BIT    (KEY3_PORTBIT << 8)
#define KEY4_BIT    (KEY4_PORTBIT)
#define KEY5_BIT    (KEY5_PORTBIT)
#define KEY6_BIT    (KEY6_PORTBIT)
#define KEY7_BIT    (KEY7_PORTBIT)
#define KEY8_BIT    (KEY8_PORTBIT)
#define KEY9_BIT    (KEY9_PORTBIT)

struct KeyDefine{
	word	keybit;
	byte	type;		// 0=Keyboard,1=Consumer/SystemControl
	byte	usage;		// 0=?,1=enum ConsumerKeys
};

const KeyDefine keyTable[] = {
	{ KEY1_BIT, 1, VOLMUTE },
	{ KEY2_BIT, 1, VOLDOWN },
	{ KEY3_BIT, 1, VOLUP },
	{ KEY4_BIT, 1, PLAYPAUSE },
	{ KEY5_BIT, 1, STOP },
	{ KEY6_BIT, 1, PREVIOUSTRACK },
	{ KEY7_BIT, 1, NEXTTRACK },
	{ KEY8_BIT, 1, MEDIA },
	{ KEY9_BIT, 1, CALCULATOR },
};

#define KEYNUM (sizeof(keyTable)/sizeof(keyTable[0]))

word keyBefore;

void setup() {
	pinMode(KEY1_PINNO, INPUT_PULLUP);
	pinMode(KEY2_PINNO, INPUT_PULLUP);
	pinMode(KEY3_PINNO, INPUT_PULLUP);
	pinMode(KEY4_PINNO, INPUT_PULLUP);
	pinMode(KEY5_PINNO, INPUT_PULLUP);
	pinMode(KEY6_PINNO, INPUT_PULLUP);
	pinMode(KEY7_PINNO, INPUT_PULLUP);
	pinMode(KEY8_PINNO, INPUT_PULLUP);
	pinMode(KEY9_PINNO, INPUT_PULLUP);
	
	pinMode(7, OUTPUT);
	
	//Keyboard.begin();
	Consumer.begin(); 
	
	keyBefore = 0xffff;
}

void loop()
{
#if false
	// 20秒を超えたら機能停止
	unsigned long now = millis();
	if(now >= (unsigned long)( 20ul * 1000ul)){
		digitalWrite(7, HIGH);
		return;
	}
#endif
	
    word keyNow = getButton();
    word keyChanged = keyNow ^ keyBefore;
	
	for(byte i = 0; i < KEYNUM; i++){
		// 変化した全てのキーの処理が終わったらループを抜ける
		if(keyChanged == 0){
			break;
		}
		
	    if((keyChanged & keyTable[i].keybit) != 0){
	        if((keyNow & keyTable[i].keybit) == 0){
				keyProc(keyTable[i], false);
	        }else{
				keyProc(keyTable[i], true);
	        }

			// 処理したキーの変化フラグを落とす
	        keyChanged = keyChanged & (~keyTable[i].keybit);
	    }
	}

    keyBefore = keyNow;
}

void keyProc(KeyDefine keyDatam, bool isPress)
{
	if( keyDatam.type == 1){
		Consumer.process(keyDatam.usage, isPress);
	}
}

byte buttonInit = 0;
word pinBefore;
unsigned long millsBefore;

word getButton()
{
    word pinNow = ~((PIND << 8) | PINB);		// pullupされているので反転してActiveHighにする
    unsigned long millsNow = millis();

    // 初回は記録のみでリターン
    if(buttonInit == 0){
        buttonInit = 1;
        pinBefore = pinNow;
        millsBefore = millsNow;
        return pinNow;
    }

    // 経過時間取得
    word millsPass;
    if( millsNow >= millsBefore ){
        millsPass = (word)( millsNow - millsBefore );
    }else{
        millsPass = (word)( ( 4294967295 - millsBefore ) + 1 + millsNow );
    }

    // 10ms以上経っていない場合は前回値をリターン
    if( millsPass < 10 ){
        return pinBefore;
    }

    // 現在時間を記録
    millsBefore = millsNow;

    // 戻り値用に前回値と今回のAND結果を作り、今回値を次回値にする
    word pinWk = pinBefore & pinNow;
    pinBefore = pinNow;

    return pinWk;
}
