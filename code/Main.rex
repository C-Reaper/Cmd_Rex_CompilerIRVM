import System;

global i64 main(){
    write(1,"-- Start --\n",12);
    write(1,"Please type sth: ",17);

    i8* buf = "                                ";
    read(0,buf,12);

    write(1,"Output: ",8);
    write(1,buf,12);
    write(1,"-----------\n",12);
    return 0;
}