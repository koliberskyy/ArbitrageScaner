#ifndef HTXAPI_H
#define HTXAPI_H

#include "marketapi.h"

class HtxApi : public MarketApi
{
    Q_OBJECT
public:
    explicit HtxApi(std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);
public slots:
    virtual void updateTokenList() final;
    virtual void updateCurrencies() final;
    virtual void updateVolumeMap() final;
protected:
    virtual void parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply) final;

};

#endif // HTXAPI_H
