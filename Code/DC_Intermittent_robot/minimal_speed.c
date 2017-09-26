int main(void) {

    init();
    i2c_init();
    drv_init();

    if(fram.cp != 1){
        enbl_mot();
        drv_mot(-1, 1);

        __delay_cycles(32000000);
        dsbl_mot();
    }
    fram.cp = 1;

    //Enable LED to drain excess energy
    PJOUT |= BIT6;

    return 0;
}
