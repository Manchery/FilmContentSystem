#include "common_gui.h"

QString CharString2QString(const CharString &cstr){
    return QString::fromStdWString(cstr.toWString());
}
