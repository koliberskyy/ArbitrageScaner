#ifndef BINGXAPI_H
#define BINGXAPI_H

#include <marketapi.h>

class BingXApi : public MarketApi
{
    Q_OBJECT
public:
    explicit BingXApi(std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);
public slots:
    virtual void updateTokenList() final;
    virtual void updateCurrencies() final;
    virtual void updateVolumeMap() final;
protected:
    virtual void parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply) final;

};

#endif // BINGXAPI_H
