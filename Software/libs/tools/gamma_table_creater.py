
''' the c-code looks like this
void init_gamma_table(void) {
    for (int i = 0; i < 256; i++) {
        gamma_table[i] = (uint8_t)(pow(i / 255.0, GAMMA) * 255.0 + 0.5);
    }
}
'''

def init_gamma_table():
    GAMMA = 2.2
    gamma_table = [0]*256
    for i in range(256):
        gamma_table[i] = int(pow(i / 255.0, GAMMA) * 255.0 + 0.5)
    return gamma_table

def create_gamma_table_string(gamma_table:list):
    gamma_string = str(gamma_table)
    gamma_string = gamma_string.replace('[', '')
    gamma_string = gamma_string.replace(']', '')
    return gamma_string
   
gamma_table = init_gamma_table()
gamma_string = create_gamma_table_string(gamma_table)

with open("gamma_table.txt", "w") as f:
  f.write("static uint8_t gamma_table[256] = { ")
  f.write(gamma_string)
  f.write("};")