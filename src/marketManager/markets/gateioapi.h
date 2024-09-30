#ifndef GATEIOAPI_H
#define GATEIOAPI_H

#include "marketapi.h"

class GateIoApi : public MarketApi
{
    Q_OBJECT
public:
    explicit GateIoApi(std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);
public slots:
    virtual void updateTokenList() final;
    virtual void updateCurrencies() final;
    virtual void updateVolumeMap() final;
protected:
    virtual void parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply) final;

};

#endif // GATEIOAPI_H
