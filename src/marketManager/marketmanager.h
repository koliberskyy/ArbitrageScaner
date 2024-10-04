#ifndef MARKETMANAGER_H
#define MARKETMANAGER_H

#include <QObject>
#include <vector>
#include <QMutex>
#include <QFile>
#include "marketapi.h"
#include "bybitapi.h"
#include "mexcapi.h"
#include "bingxapi.h"
#include "gateioapi.h"
#include "htxapi.h"

class MarketManager : public QObject
{
    Q_OBJECT
public:
    explicit MarketManager(std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);
    auto getSpredVec()const{
        return &spredVec;
    }
public slots:
    void updateTokens();
private:
    std::shared_ptr<QNetworkAccessManager> netManager;
    std::vector<std::shared_ptr<MarketApi>> markets;
    int updateCounter{0}; //счетчик оновленных бирж
    QMutex updateCounter_mtx; //мьютекс блокировки инкрементирования счетчика оновленных бирж
    std::vector<Spred> spredVec;
private slots:
    void checkUpdate();
    void findSpred();
signals:
    void tokenUpdatingComplete();
    void spredUpdated();

};

#endif // MARKETMANAGER_H
