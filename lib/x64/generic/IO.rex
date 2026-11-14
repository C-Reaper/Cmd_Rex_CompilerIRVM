import System;
import CStr;
import Pointer;
import Parser;

define O_RDONLY     0;        // Datei nur zum Lesen.
define O_WRONLY     1;        // Datei nur zum Schreiben.
define O_RDWR       2;        // Datei sowohl zum Lesen als auch zum Schreiben.
define O_CREAT      64;       // Erzeugt die Datei, falls sie nicht existiert. Wenn dieses Flag gesetzt ist, muss auch ein Berechtigungs-Mode angegeben werden (z. B. 0644 für Lese-/Schreibrechte).
define O_EXCL       128;      // Wird mit O_CREAT verwendet, um sicherzustellen, dass der Syscall fehlschlägt, wenn die Datei bereits existiert.
define O_TRUNC      512;      // Schneidet die Datei auf eine Länge von 0 ab, falls sie existiert und zum Schreiben oder zum Lesen/Schreiben geöffnet wurde.
define O_APPEND     1024;     // Setzt den Schreibzeiger an das Ende der Datei, sodass alle Schreibvorgänge ans Ende der Datei angehängt werden.
define O_NONBLOCK   2048;     // Öffnet die Datei im nicht-blockierenden Modus. Wird häufig für Spezialdateien wie Pipes und FIFO-Dateien verwendet.
define O_DSYNC      4096;     // Sorgt dafür, dass Schreiboperationen direkt auf die Festplatte geschrieben werden, sodass die Daten synchron sind, aber Metadaten (wie Änderungszeit) möglicherweise nicht.
define O_SYNC       1052672;  // Wie O_DSYNC, aber garantiert zusätzlich, dass alle Metadaten aktualisiert werden.
define O_NOFOLLOW   256;      // Verhindert das Folgen von symbolischen Links. Falls die Datei ein symbolischer Link ist, schlägt open mit einem Fehler fehl.
define O_CLOEXEC    524288;   // Setzt das FD_CLOEXEC-Flag, welches den Dateideskriptor automatisch schließt, wenn ein exec-Aufruf ausgeführt wird.
define O_DIRECTORY  65536;    // Öffnet die Datei nur, wenn sie ein Verzeichnis ist; anderenfalls schlägt der Syscall mit einem Fehler fehl.
define O_TMPFILE    4259840;  // Erzeugt eine temporäre Datei, die beim Schließen automatisch gelöscht wird, falls sie keinen Link im Dateisystem hat.

define F_GETFL      3;
define F_SETFL      4;

define SEEK_SET     0;        // Datei Anfang.
define SEEK_CUR     1;        // Datei relativ.
define SEEK_END     2;        // Datei Ende.

namespace io {
    void input(i8* cstr,i64 len){
        sys::read(stdin(),cstr,len);
    }
    void print(i8* cstr){
        i64 len = cstr::len(cstr);
        sys::write(stdout(),cstr,len);
    }
    void error(i8* cstr){
        i64 len = cstr::len(cstr);
        sys::write(stderr(),cstr,len);
    }
    
    void print_char(i8 ch){
        i8 ch_stack = ch;
        sys::write(stdout(),&ch_stack,1);
    }
    void print_int(i64 a){
        i8[24] str;
        ptr::set(str,0,24);
        parse::int::get(str,a);
        i64 len = cstr::len(str);
        sys::write(stdout(),str,len);
    }
    void print_uint(u64 a){
        i8[24] str;
        ptr::set(str,0,24);
        parse::uint::get(str,a);
        i64 len = cstr::len(str);
        sys::write(stdout(),str,len);
    }
    /*
    void print_float(f64 a){
        i8[24] str;
        ptr::set(str,0,24);
        parse::float::get(str,a,6);
        i64 len = cstr::len(str);
        sys::write(1,str,len);
    }
    */
    void print_hex(u64 a){
        i8[24] str;
        ptr::set(str,0,24);
        parse::hex::get(str,a);
        i64 len = cstr::len(str);
        sys::write(stdout(),str,len);
    }
}