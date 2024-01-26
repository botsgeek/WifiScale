// SDCardManager.h
#ifndef SDCARDMANAGER_H
#define SDCARDMANAGER_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>

class SDCardManager {
public:
    SDCardManager();
    void initSDCard();

private:
    bool initialized;
};

#endif

