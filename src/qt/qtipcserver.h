#ifndef QTIPCSERVER_H
#define QTIPCSERVER_H

// Define SweepstakeCoin-Qt message queue name
#define BITCOINURI_QUEUE_NAME "SweepstakeCoinURI"

void ipcScanRelay(int argc, char *argv[]);
void ipcInit(int argc, char *argv[]);

#endif // QTIPCSERVER_H
