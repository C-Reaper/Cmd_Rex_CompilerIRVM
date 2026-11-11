import IO;

global i64 main(){
    i8[32] buffer;
    io::print("-- Start --\n");
    io::print("Pls say ur name: ");
    io::input(buffer,32);
    io::print("Hi ");
    io::print(buffer);
    io::print("-----------\n");
    return 0;
}