#ifndef BYBITAPI_H
#define BYBITAPI_H

#include "marketapi.h"

class BybitApi : public MarketApi
{
    Q_OBJECT
public:
    explicit BybitApi(std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);
public slots:
    virtual void updateTokenList() final;
    virtual void updatePriceMap() final;
    virtual void updateVolumeMap() final;
protected:
    virtual void parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply) final;

};

#endif // BYBITAPI_H
