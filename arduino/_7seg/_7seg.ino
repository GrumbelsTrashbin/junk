#include <LiquidCrystal.h>
LiquidCrystal lcd(1,0, 7,6,5,4);

#define MOTOR_PIN_1 A0
#define MOTOR_PIN_2 A1
#define MOTOR_PIN_3 A2
#define MOTOR_PIN_4 A3
const uint8_t MOTOR_PINS[] = { MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4 };
#define NUM_MOTOR_PINS sizeof(MOTOR_PINS)

#define MOTOR_DELAY 10 /* From datasheet, I think this sould be >= 10, but 2 seems to work */

#define SHIFT_PIN_SI 2
#define SHIFT_PIN_SCK 3
#define SHIFT_PIN_RCK 3

#define PATTERN_CHANGE_PIN 13

#define SHIFT_REGISTER 0x20
#define QA (SHIFT_REGISTER | 0)
#define QB (SHIFT_REGISTER | 1)
#define QC (SHIFT_REGISTER | 2)
#define QD (SHIFT_REGISTER | 3)
#define QE (SHIFT_REGISTER | 4)
#define QF (SHIFT_REGISTER | 5)
#define QG (SHIFT_REGISTER | 6)
#define QH (SHIFT_REGISTER | 7)

bool is_shift_pin(int pin)
{
  return (pin & SHIFT_REGISTER);
}

#if 0
/* Mapping between arduino digital pins and display pins: */
#define DISP_PIN_1      7
#define DISP_PIN_2      6
#define DISP_PIN_3      5
#define DISP_PIN_4      4
#define DISP_PIN_5      3
#define DISP_PIN_6_NEG  8 /* GROUND: Connect 1K resistor in series */
#define DISP_PIN_7      0
#define DISP_PIN_8_NEG  9 /* GROUND: Connect 1K resistor in series */
#define DISP_PIN_9_NEG  10 /* GROUND: Connect 1K resistor in series */
#define DISP_PIN_10     1
#define DISP_PIN_11     2
#define DISP_PIN_12_NEG 12 /* GROUND: Connect 1K resistor in series */
#else
/* Mapping between arduino digital pins and display pins: */
#define DISP_PIN_1      QD
#define DISP_PIN_2      QE
#define DISP_PIN_3      QF
#define DISP_PIN_4      QG
#define DISP_PIN_5      QH
#define DISP_PIN_6_NEG  8 /* GROUND: Connect 1K resistor in series */
#define DISP_PIN_7      QA
#define DISP_PIN_8_NEG  9 /* GROUND: Connect 1K resistor in series */
#define DISP_PIN_9_NEG  10 /* GROUND: Connect 1K resistor in series */
#define DISP_PIN_10     QC
#define DISP_PIN_11     QB
#define DISP_PIN_12_NEG 12 /* GROUND: Connect 1K resistor in series */
#endif

/* Definition of segment/digit pins on LED display */

/* LOW to enable digit */
#define DIG_1  DISP_PIN_12_NEG
#define DIG_2  DISP_PIN_9_NEG
#define DIG_3  DISP_PIN_8_NEG
#define DIG_4  DISP_PIN_6_NEG
const uint8_t POS_PINS[] = {DIG_1, DIG_2, DIG_3, DIG_4};

/*
 * SEGMENTS:
 *
 *   __A__
 *  |     |
 *  F     B
 *  |__G__|
 *  |     |
 *  E     C
 *  |__D__| .DP
 */

/* HIGH to enable segment */
#define SEG_PIN_A  DISP_PIN_11
#define SEG_PIN_B  DISP_PIN_7
#define SEG_PIN_C  DISP_PIN_4
#define SEG_PIN_D  DISP_PIN_2
#define SEG_PIN_E  DISP_PIN_1
#define SEG_PIN_F  DISP_PIN_10
#define SEG_PIN_G  DISP_PIN_5
#define SEG_PIN_DP DISP_PIN_3
const uint8_t SEGMENT_PINS[] = {SEG_PIN_A, SEG_PIN_B, SEG_PIN_C, SEG_PIN_D, SEG_PIN_E, SEG_PIN_F, SEG_PIN_G, SEG_PIN_DP};

#define NUM_DIGITS sizeof(POS_PINS)
#define NUM_SEGS   sizeof(SEGMENT_PINS)


#define SEG_A  1 << 0
#define SEG_B  1 << 1
#define SEG_C  1 << 2
#define SEG_D  1 << 3
#define SEG_E  1 << 4
#define SEG_F  1 << 5
#define SEG_G  1 << 6
#define SEG_DP 1 << 7

typedef uint8_t seg_t;
const seg_t ALL_SEGS = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP);
const seg_t BLANK_SEGS = 0;

/* Multiple characters with a common symbols are defined here for convenience */
#define FONT_A (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
#define FONT_b (SEG_F | SEG_G | SEG_C | SEG_D | SEG_E)
#define FONT_C (SEG_A | SEG_F | SEG_E | SEG_D) /* C, [, {, ( */
#define FONT_d (SEG_B | SEG_C | SEG_D | SEG_E | SEG_G)
#define FONT_E (SEG_A | SEG_F | SEG_G | SEG_E | SEG_D)
#define FONT_F (SEG_A | SEG_F | SEG_G | SEG_E)
#define FONT_H (SEG_F | SEG_E | SEG_G | SEG_B | SEG_C) /* H, X, x */
#define FONT_I (SEG_F | SEG_E) /* I, l, | */
#define FONT_J (SEG_B | SEG_C | SEG_D | SEG_E)
#define FONT_k (SEG_F | SEG_E | SEG_G | SEG_D)
#define FONT_O (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F) /* 0, o */
#define FONT_P (SEG_E | SEG_F | SEG_A | SEG_B | SEG_G)
#define FONT_q (SEG_A | SEG_F | SEG_G | SEG_B | SEG_C)
#define FONT_r (SEG_E | SEG_G)
#define FONT_S (SEG_A | SEG_F | SEG_G | SEG_C | SEG_D) /* s, S, 5 */
#define FONT_y (SEG_F | SEG_G | SEG_B | SEG_C | SEG_D)
#define FONT_Z (SEG_A | SEG_B | SEG_G | SEG_E | SEG_D) /* z, Z, 2 */
#define FONT_CLOSE_BRACKET (SEG_A | SEG_B | SEG_C | SEG_D) /* ), ], } */
#define FONT_DOT SEG_DP /* . , */

#define ASCII_OFF 0x20 /* Skip the first 32 characters */
/* Font table, starting with ASCII 0x20 */
const seg_t font[] = {
  /* SPACE */ BLANK_SEGS,
  /* ! */ SEG_B | SEG_DP,
  /* " */ SEG_F | SEG_B,
  /* # */ ALL_SEGS,
  /* $ */ 0,
  /* % */ SEG_B | SEG_G | SEG_E | SEG_DP,
  /* & */ SEG_D | SEG_E | SEG_G | SEG_B | SEG_A | SEG_F,
  /* ' */ SEG_F,
  /* ( */ FONT_C,
  /* ) */ FONT_CLOSE_BRACKET,
  /* * */ SEG_A | SEG_B | SEG_G | SEG_F,
  /* + */ SEG_B | SEG_C | SEG_G,
  /* , */ FONT_DOT,
  /* - */ SEG_G,
  /* . */ FONT_DOT,
  /* / */ SEG_E | SEG_G | SEG_B,
  /* 0 */ FONT_O,
  /* 1 */ SEG_B | SEG_C,
  /* 2 */ FONT_Z,
  /* 3 */ SEG_A | SEG_B | SEG_G | SEG_C | SEG_D,
  /* 4 */ SEG_F | SEG_G | SEG_B | SEG_C,
  /* 5 */ FONT_S,
  /* 6 */ SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,
  /* 7 */ SEG_A | SEG_B | SEG_C,
  /* 8 */ SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
  /* 9 */ SEG_C | SEG_B | SEG_A | SEG_F | SEG_G,
  /* : */ SEG_A | SEG_D,
  /* ; */ SEG_A | SEG_DP,
  /* < */ SEG_A | SEG_F | SEG_G,
  /* = */ SEG_G | SEG_D,
  /* > */ SEG_A | SEG_B | SEG_G,

  /* ? */ SEG_A | SEG_B | SEG_G | SEG_E | SEG_DP,
  /* @ */ SEG_B | SEG_A | SEG_F | SEG_E | SEG_D,
  /* A */ FONT_A,
  /* B */ FONT_b,
  /* C */ FONT_C,
  /* D */ FONT_d,
  /* E */ FONT_E,
  /* F */ FONT_F,
  /* G */ SEG_A | SEG_F | SEG_E | SEG_D | SEG_C,
  /* H */ FONT_H,
  /* I */ FONT_I,
  /* J */ FONT_J,
  /* K */ FONT_k,
  /* L */ SEG_F | SEG_E | SEG_D,
  /* M */ 0,
  /* N */ SEG_E | SEG_F | SEG_A | SEG_B | SEG_C,
  /* O */ FONT_O,
  /* P */ FONT_P,
  /* Q */ FONT_q,
  /* R */ FONT_r,
  /* S */ FONT_S,
  /* T */ SEG_A | SEG_F | SEG_E,
  /* U */ SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,
  /* V */ SEG_F | SEG_E | SEG_D | SEG_B,
  /* W */ 0,
  /* X */ FONT_H,
  /* Y */ FONT_y,
  /* Z */ FONT_Z,
  /* [ */ FONT_C,
  /* \ */ SEG_F | SEG_G | SEG_C,
  /* ] */ FONT_CLOSE_BRACKET,
  /* ^ */ SEG_F | SEG_A | SEG_B,
  /* _ */ SEG_D,

  /* ` */ SEG_B,
  /* a */ FONT_A,
  /* b */ FONT_b,
  /* c */ SEG_G | SEG_E | SEG_D,
  /* d */ FONT_d,
  /* e */ FONT_E,
  /* f */ FONT_F,
  /* g */ SEG_A | SEG_F | SEG_G | SEG_B | SEG_C | SEG_D,
  /* h */ SEG_F | SEG_E | SEG_G | SEG_C,
  /* i */ SEG_E,
  /* j */ FONT_J,
  /* k */ FONT_k,
  /* l */ FONT_I,
  /* m */ 0,
  /* n */ SEG_E | SEG_G | SEG_C,
  /* o */ SEG_G | SEG_C | SEG_D | SEG_E,
  /* p */ FONT_P,
  /* q */ FONT_q,
  /* r */ FONT_r,
  /* s */ FONT_S,
  /* t */ SEG_F | SEG_E | SEG_G,
  /* u */ SEG_E | SEG_D | SEG_C,
  /* v */ SEG_E | SEG_D,
  /* w */ 0,
  /* x */ FONT_H,
  /* y */ FONT_y,
  /* z */ FONT_Z,
  /* { */ FONT_C,
  /* | */ FONT_I,
  /* } */ FONT_CLOSE_BRACKET,
  /* ~ */ SEG_A,
  /* DEL */ 0,
};

const seg_t spin[] = {
  SEG_A,
  SEG_B,
  SEG_C,
  SEG_D,
  SEG_E,
  SEG_F,
};

const seg_t spin2[][NUM_DIGITS] = {
  {SEG_A, 0, 0, 0},
  {SEG_B, 0, 0, 0},
  {0, SEG_E, 0, 0},
  {0, SEG_D, 0, 0},
  {0, SEG_C, 0, 0},
  {0, 0, SEG_F, 0},
  {0, 0, SEG_A, 0},
  {0, 0, SEG_B, 0},
  {0, 0, 0, SEG_E},
  {0, 0, 0, SEG_D},
  {0, 0, 0, SEG_C},
  {0, 0, 0, SEG_B},
  {0, 0, 0, SEG_A},
  {0, 0, 0, SEG_F},
  {0, 0, SEG_C, 0},
  {0, 0, SEG_D, 0},
  {0, 0, SEG_E, 0},
  {0, SEG_B, 0, 0},
  {0, SEG_A, 0, 0},
  {0, SEG_F, 0, 0},
  {SEG_C, 0, 0, 0},
  {SEG_D, 0, 0, 0},
  {SEG_E, 0, 0, 0},
  {SEG_F, 0, 0, 0},
};

const seg_t spin3[][NUM_DIGITS] = {
  {SEG_A|SEG_DP, 0,            0,            SEG_A    },
  {SEG_B,        0|SEG_DP,     0,            SEG_F    },
  {0,            SEG_E|SEG_DP, SEG_C,        0        },
  {0,            SEG_D,        SEG_D|SEG_DP, 0        },
  {0,            SEG_C,        SEG_E|SEG_DP, 0        },
  {0,            SEG_B,        SEG_F,        0|SEG_DP },

  {0,            SEG_A,        SEG_A,        0|SEG_DP },
  {0,            SEG_F,        SEG_B|SEG_DP, 0        },
  {SEG_C,        0,            0|SEG_DP,     SEG_E    },
  {SEG_D,        0|SEG_DP,     0,            SEG_D    },
  {SEG_E,        0|SEG_DP,     0,            SEG_C    },
  {SEG_F|SEG_DP, 0,            0,            SEG_B    },
  };

const int SEGS_ALL[]   = {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_DP};


void pos_state(int pos, int state)
{
  digitalWrite(POS_PINS[pos], !state);
}

void all_pos_state(int state)
{
  int i;

  for (i = 0; i < NUM_DIGITS; i++)
    digitalWrite(POS_PINS[i], !state);
}

void segs(seg_t segs)
{
  int i, state;
  uint8_t shift = 0;

  /* I could skip this by making SEG_X match the shift register outputs */
  for (i = 0; i < NUM_SEGS; i++) {
    state = ((1<<i) & segs ? HIGH : LOW);
    if (is_shift_pin(SEGMENT_PINS[i]))
      if (state)
        shift |= 1 << (SEGMENT_PINS[i] & ~SHIFT_REGISTER);
    else
      digitalWrite(SEGMENT_PINS[i], state);
  }

  digitalWrite(SHIFT_PIN_SCK, LOW);
  digitalWrite(SHIFT_PIN_RCK, LOW);
  for (i = 0; i < 8; i++) {
    digitalWrite(SHIFT_PIN_SI, (shift & (0x80 >> i) ? HIGH : LOW));
    digitalWrite(SHIFT_PIN_SCK, HIGH);
    digitalWrite(SHIFT_PIN_SCK, LOW);
  }
  digitalWrite(SHIFT_PIN_RCK, HIGH);
  digitalWrite(SHIFT_PIN_RCK, LOW);
}

void pr_array(const seg_t array[NUM_DIGITS])
{
  int i;

  for (i = 0; i < NUM_DIGITS; i++) {
    segs(array[i]);
    pos_state(i, 1);
    delay(4);
    pos_state(i, 0);
  }
}

void pr_text(const seg_t array[NUM_DIGITS])
{
  int i;

  for (i = 0; i < NUM_DIGITS; i++) {
    segs(font[array[i] - ASCII_OFF]);
    pos_state(i, 1);
    delay(4);
    pos_state(i, 0);
  }
}

void reset_disp()
{
  int i;

  segs(BLANK_SEGS);

  for (i = 0; i < NUM_SEGS; i++) {
    if (!is_shift_pin(SEGMENT_PINS[i]))
      pinMode(SEGMENT_PINS[i], OUTPUT);
  }
  for (i = 0; i < NUM_DIGITS; i++) {
    pinMode(POS_PINS[i], OUTPUT);
    pos_state(i, 1);
  }
}

void reset_motor()
{
  int i;

  for (i = 0; i < NUM_MOTOR_PINS; i++) {
    pinMode(MOTOR_PINS[i], OUTPUT);
    digitalWrite(MOTOR_PINS[i], LOW);
  }
}

void setup()
{
  int i;

  lcd.begin(16, 2);
  lcd.print("setup");

  reset_motor();

  reset_disp();

  pinMode(SHIFT_PIN_SI, OUTPUT);
  digitalWrite(SHIFT_PIN_SI, LOW);
  pinMode(SHIFT_PIN_SCK, OUTPUT);
  digitalWrite(SHIFT_PIN_SCK, LOW);
  pinMode(SHIFT_PIN_RCK, OUTPUT);
  digitalWrite(SHIFT_PIN_RCK, LOW);

  pinMode(PATTERN_CHANGE_PIN, INPUT);
  digitalWrite(PATTERN_CHANGE_PIN, HIGH);
}

void num2array(long num, seg_t array[4])
{
  int i, neg = 0, sig = 0;
  
  if (num > 9999 || num < -999) {
    array[0] = 'F';
    array[1] = 'A';
    array[2] = 'I';
    array[3] = 'L';
    return;
  }
  
  if (num < 0) {
    neg = 1;
    num = num * -1;
  }
  array[0] = '0' + (num / 1000) % 10;
  array[1] = '0' + (num /  100) % 10;
  array[2] = '0' + (num /   10) % 10;
  array[3] = '0' + (num /    1) % 10;

  for (i = 0; i < 4; i++) {
    if (!sig && array[i] != '0')
      sig = i+1;
    if (!sig && i < 3 && array[i] == '0')
      array[i] = ' ';
  }
  
  if (neg && sig > 1)
    array[sig-2] = '-';
}

void pattern0()
{
  all_pos_state(0);
  segs(font['0' + (millis()/1000%10) - ASCII_OFF]);
  all_pos_state(1);
  delay(100);
}

void pattern1()
{
  all_pos_state(0);
  segs(font[millis()/500 % (sizeof(font) / sizeof(font[0]))]);
  all_pos_state(1);
  delay(100);
}

void pattern2()
{
  static int i = 0;

  segs(spin[i]);
  i = (i + 1) % (sizeof(spin) / sizeof(spin[0]));
  delay(50);
}

void pattern3()
{
  pr_array(spin2[(millis() / 40) % (sizeof(spin2) / sizeof(spin2[0]))]);
}

void pattern4()
{
  pr_array(spin3[(millis() / 40) % (sizeof(spin3) / sizeof(spin3[0]))]);
}

void pattern5()
{
  int j, max = sizeof(font) / sizeof(font[0]);
  uint8_t i = millis() / 250 % max;
  seg_t array[NUM_DIGITS];

  for (j = 0; j < NUM_DIGITS; j++)
    array[j] = font[(i+j) % max];

  pr_array(array);
}

void pattern6()
{
  seg_t array[NUM_DIGITS];
  int i;
  
  num2array(2500 - (millis() / 10 % 4500), array);
  
  pr_text(array);
}

void pattern7()
{
  seg_t array[NUM_DIGITS];
  array[0] = 'a';
  array[1] = 'b';
  array[2] = 'c';
  array[3] = 'd';
  
  pr_text(array);
}

void stepper_drive()
{
  int i;

  for (i = 0; i < NUM_MOTOR_PINS; i++) {
    digitalWrite(MOTOR_PINS[i], HIGH);
    delay(MOTOR_DELAY);
    digitalWrite(MOTOR_PINS[(i+4-1)%4], LOW);
  }
}

void (*patterns[])() = {
  pattern0,
  stepper_drive,
  pattern1,
  pattern2,
  pattern3,
  pattern4,
  pattern5,
  pattern6,
  pattern7,
};

void loop()
{
  static int pattern = 0;
  static int last_state = HIGH;
  int state = digitalRead(PATTERN_CHANGE_PIN);

  if (state != last_state) {
    last_state = state;
    if (state == LOW) {
      reset_disp();
      pattern = (pattern + 1) % (sizeof(patterns) / sizeof(void*));

      lcd.clear();
      lcd.print("pattern ");
      lcd.print(pattern);
    }
  }

  patterns[pattern]();
}

// vim:filetype=c:sw=2:ts=2:expandtab
