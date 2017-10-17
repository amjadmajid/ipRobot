int main(void) {

    motor_calib mc = {0, 0, 200};

    init();
    drv_init(mc);

    if(fram.cp != 1){
        enbl_mot();
        drv_mot(90, 80);

        __delay_cycles(32000000);
        dsbl_mot();
    }
    //fram.cp = 1;

    //Enable LED to drain excess energy
    PJOUT |= BIT6;

    return 0;
}
