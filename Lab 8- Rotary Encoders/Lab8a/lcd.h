/*
  init_lcd - Do various things to initialize the LCD display
*/
void init_lcd();

/*
  stringout - Print the contents of the character string "str"
  at the current cursor position.
*/
void stringout(char *);

/*
  moveto - Move the cursor to the postion "pos"
*/
void moveto(unsigned char);

/*
  writecommand - Output a byte to the LCD display instruction register.
*/
void writecommand(unsigned char);

/*
  writedata - Output a byte to the LCD display data register
*/
void writedata(unsigned char);